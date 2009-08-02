--------------------  ----------------  ----------------------  ---------------
-- yoshi's island --  -- lua script --  -- work in progress --  -- (nitsuja) --
--------------------  ----------------  ----------------------  ---------------

-- configuration
-- these describe (and determine) the basic capabilities of this script:
drawobjectnames        = true -- draws name if known, or object ID if not
drawobjecthitboxes     = true -- note: hitboxes are not 100% accurate yet
drawobjectcenterpoints = true -- draws a little dot at objects' reported centers
drawplayerhitbox       = true -- currently only correct if player is yoshi
drawsavestatehitboxes  = true -- player hitbox for other savestates (GHOST DISPLAY)
drawsavestatenumbers   = true -- draws the slot number next to them
drawplayertrail        = true -- shows 90 frames or so of speed/position info
drawsavestatetrails    = true -- show player trail for other savestates
drawspeedometer        = true -- draws xvel, yvel numbers constantly

-- if you're comparing against another movie,
-- save before the end of that movie onto this slot number
-- (you might have to rename the .luasav file to match your new movie)
specialslot = 10

-- as you get ahead in each room or level,
-- increase this by the number of frames you're ahead of the other movie
-- to resynchronize ongoing comparisons with the special slot
adjustframeaheadby = 0

-- I recommend leaving these on
smartmatchguiobjects   = true -- takes drawing time into account so GUI matches
smartmatchguicamera    = true -- takes camera update time into account for GUI
smartsyncframeupdate   = true -- sync to YI's frame counter update, things go a bit wacky otherwise (especially trails)

--------------------------------------------------------------------------------
--------------------------------------------------------------------------------



-- TODO: save this to a file so it doesn't reset to 0 every time the script restarts
currentstatenumber = 0

emu = emu or snes9x or gens gens = emu or snes9x or gens snes9x = emu or snes9x or gens

-- prevent text from clipping off the screen edges
local oldguitext = gui.text
gui.text = function(x,y,text)
	if type(text) ~= 'string' then text = tostring(text) end
	local width = 4 * #text
	if x+width > 256 then x=256-width end
	if x < 0 then x = 0 end
	if y > 216 then y = 216 end
	if y < 0 then y = 0 end
	oldguitext(x,y,text)
end


--memory.writeword(0x70008C, memory.readword(0x70008C) + 100)

--	gui.text(30,30,memory.readword(0x7001b4)) -- standing on object flag!


-- because snes9x require numbered savestates to be 'created' to use them
local saveslot = {}
for n = 1,12 do
	saveslot[n] = savestate.create(n)
end


local copytable = function(t)
	if t == nil then return nil end
	local c = {}
	for k,v in pairs(t) do
		c[k] = v
	end
--	setmetatable(c,debug.getmetatable(t))
	return c
end

-- for copying a table in frameinforecords while discarding non-recent entries
local copytablepartial = function(t)
	if t == nil then return nil end
	local abs = math.abs
	local now = getinstantid()
	local c = {}
	for k,v in pairs(t) do
		if type(k) ~= 'number' or abs(k-now) < 3600 then
			c[k] = v
		end
	end
	return c
end






-- a record of frame information
-- looks like: {savenumber1={time1={x1,y1},time2={x2,y2},...},savenumber2=...}
frameinforecords = {} -- initially it's an empty table

-- a helper function to get the info we want to store per in-game frame
-- (for now this is only the player position)
function getperframeinfo()
	local x = memory.readword(0x70008C) + (memory.readbyte(0x70008A) / 256)
	local y = memory.readword(0x700090) + (memory.readbyte(0x70008E) / 256)
--	local y2 = memory.readword(0x700011E)
--	local yext = memory.readword(0x7000112)
	local info = {x,y}
	return info
end

-- time function, might want to use in-game time here in some games
function getinstantid()
--	return memory.readword(0x7E0030)
	return emu.framecount()
end


-- if we draw with the current camera position things will look a little off
-- when the camera is moving, so use the camera positon slightly in the past
-- (do this by keeping with a circular buffer of x and y camera positions)

xcamhist = {0,0,0,0} -- don't need so many but whatever...
ycamhist = {0,0,0,0}
local function tickcam()
	xcamhist[1 + (getinstantid() % 4)] = memory.readword(0x7E0039)
	ycamhist[1 + (getinstantid() % 4)] = memory.readword(0x7E003B)
end
local function resetcam()
	local xcam = memory.readword(0x7E0039)
	local ycam = memory.readword(0x7E003B)
	for k,v in pairs(xcamhist) do xcamhist[k] = xcam end
	for k,v in pairs(ycamhist) do ycamhist[k] = ycam end
end
resetcam()

local getxcam
local getycam

if smartmatchguicamera then

	getxcam = function()
		return xcamhist[1 + ((4-1 + getinstantid()) % 4)]
	end
	getycam = function()
		return ycamhist[1 + ((4-1 + getinstantid()) % 4)]
	end

else
	getxcam = function()
		return memory.readword(0x7E0039)
	end
	getycam = function()
		return memory.readword(0x7E003B)
	end
end









-- the game takes 2 frames to render the current game state,
-- so we have to buffer our rendering by 2 frames to make it match up.
-- it's a little more complicated than that though.
-- 
-- to make a long story short,
-- what you have to do is NOT call the functions like "gui.box",
-- and instead call the functions named like "drawboxinworld"
-- (using x,y coordinates in WORLD coordinates, don't subtract camera pos),
-- then also call drawdeferred at the start of each gui render.
-- that way your gui drawing should match up perfectly with the game.


local function clearoldhist(hist,now)
	for k,v in pairs(hist) do
		if k > now or k < now-4 then
			hist[k] = nil
		end
	end
end

local function gethistitem(hist,now)
	for i=-3,0 do
		local histindex = 1 + (now + i)
		items = hist[histindex]
		if items then return items end
	end
end

local boxhist = {}
local texthist = {}
local linehist = {}
local pixhist = {}

local function clearalloldhist(now)
	clearoldhist(boxhist,now)
	clearoldhist(texthist,now)
	clearoldhist(linehist,now)
	clearoldhist(pixhist,now)
end

function drawdeferred()
	local now = getinstantid()
	
	-- clear out old entries
	clearalloldhist(now)

	local camx = getxcam()
	local camy = getycam()

	local lineitems = gethistitem(linehist,now)
	if lineitems then
		local drawline = gui.drawline
		for i,args in ipairs(lineitems) do
			local x1,y1,x2,y2,color = unpack(args)
			drawline(x1-camx,y1-camy,x2-camx,y2-camy,color)
		end
	end

	local boxitems = gethistitem(boxhist,now)
	if boxitems then
		local drawbox = gui.drawbox
		for i,args in ipairs(boxitems) do
			local x1,y1,x2,y2,color = unpack(args)
			drawbox(x1-camx,y1-camy,x2-camx,y2-camy,color)
		end
	end

	local textitems = gethistitem(texthist,now)
	if textitems then
		local drawtext = gui.text
		for i,args in ipairs(textitems) do
			local x,y,msg = unpack(args) -- TODO: support passing through color
			drawtext(x-camx,y-camy,msg)
		end
	end

	local pixitems = gethistitem(pixhist,now)
	if pixitems then
		local drawpixel = gui.drawpixel
		for i,args in ipairs(pixitems) do
			local x,y,color = unpack(args)
			drawpixel(x-camx,y-camy,color)
		end
	end
end

local drawboxinworld
local drawtextinworld
local drawlineinworld
local drawpixelinworld

if smartmatchguiobjects then 
	drawboxinworld = function (...)
		local now = getinstantid()
		local items = boxhist[now]
		if not items then items = {} boxhist[now] = items end
		items[#items+1] = {...}
	end
	drawtextinworld = function(...)
		local now = getinstantid()
		local items = texthist[now]
		if not items then items = {} texthist[now] = items end
		items[#items+1] = {...}
	end
	drawlineinworld = function(...)
		local now = getinstantid()
		local items = linehist[now]
		if not items then items = {} linehist[now] = items end
		items[#items+1] = {...}
	end
	drawpixelinworld = function(...)
		local now = getinstantid()
		local items = pixhist[now]
		if not items then items = {} pixhist[now] = items end
		items[#items+1] = {...}
	end
else
	drawlineinworld = function (x1,y1,x2,y2,color)
		local camx = getxcam()
		local camy = getycam()
		gui.drawline(x1-camx,y1-camy,x2-camx,y2-camy,color)
	end
	drawtextinworld = function (x,y,msg)
		gui.text(x-getxcam(),y-getycam(),msg) -- TODO: support passing through color
	end
	drawboxinworld = function (x1,y1,x2,y2,color)
		local camx = getxcam()
		local camy = getycam()
		gui.drawbox(x1-camx,y1-camy,x2-camx,y2-camy,color)
	end
	drawpixelinworld = function (x,y,color)
		gui.drawpixel(x-getxcam(),y-getycam(),color)
	end
end














local function renderpasttrajectory(records, id, tracknumber)

	if (tracknumber == currentstatenumber and not drawplayertrail) or (tracknumber ~= currentstatenumber and not drawsavestatetrails) then
		return
	end

	local points = {}
	local ids = {}

	local lastx
	local j = 1
	for i=id,id-90,-1 do
		local info = records[i]
		if info then
			points[j] = info
			ids[j] = i
			j = j + 1
		end
	end

	local actualcamx = getxcam()
	local actualcamy = getycam()

	local camx = 0 --getxcam()
	local camy = 0 --getycam()
	
	local width = 16
	local height = 32

	-- adjust the player hitbox to match up better with the world... (HACK)
	camy = camy - 3  -- y = y + 3
	height = height - 5
	camx = camx - 1  -- x = x + 1
	width = width - 2
	
	
	local outlinecolor
	if tracknumber == -1 then
		outlinecolor = 0xFFFFFFFF
	elseif tracknumber == 0 then
		outlinecolor = 0x0000FFFF
	else
		outlinecolor = 0x7F7FFFFF
	end
	
	local alt = math.floor(id/4)%2==0
	
	local skipfirst = false
	
	local floor = math.floor
	
	for i=1,j-2 do
	
		local a = points[i]
		local b = points[i+1]
		local ax = a[1] - camx
		local ay = a[2] - camy
		local bx = b[1] - camx
		local by = b[2] - camy
		
--		local transp = 255 * (j-i) / j
--		local color = OR(AND(outlinecolor, 0xFFFFFF00), transp)

		local color = '#FFFFFF'

		local dx = bx - ax
		local dy = by - ay

		
		if math.abs(dx) < 256 and math.abs(dy) < 208
		and math.max(ax,bx) - actualcamx < 256
		and math.min(ax,bx) - actualcamx >= 0
		and math.max(ay,by) - actualcamy < 200
		and math.min(ay,by) - actualcamy >= 0
		then

			local di = ids[i] - ids[i+1]
--		local speed = math.max(math.abs(dx),math.abs(dy)) / di
			local xspeed = math.abs(dx / di)
	
			local xspeeddiff = xspeed - 2.9375
			if xspeeddiff > 0 then
				local good = math.min(255,math.max(0, xspeeddiff * 255 / 2))
				local good2 = math.min(255,math.max(0, xspeeddiff * 255 / 2 - 255))
				color = string.format('#%02XFF%02X', 255-good, 255-(good-good2))
			else
				local bad = math.min(255,math.max(0, -xspeeddiff * 512))
				if tracknumber == currentstatenumber then
					color = string.format('#FF%02X%02X', 255-bad, 255-bad)
				else
					color = string.format('#%02X%02XFF', 255-bad, 255-bad)
				end
			end
		
			ax = floor(ax)
			bx = floor(bx)
			ay = floor(ay)
			by = floor(by)
		
			if dx < 0 or dy < 0 then
				drawlineinworld(ax,ay,bx,by, color)
			end
			if dx < 0 or dy > 0 then
				drawlineinworld(ax,ay+height,bx,by+height, color)
			end
			if dx > 0 or dy < 0 then
				drawlineinworld(ax+width,ay,bx+width,by, color)
			end
			if dx > 0 or dy > 0 then
				drawlineinworld(ax+width,ay+height,bx+width,by+height, color)
			end

			skipfirst = true
		end		

	end

end


-- a helper function to draw the info of the given frame
function renderframeinfo(records, id, tracknumber)

	if records == nil then return end

	local info = records[id]
	local notcurrent = tracknumber ~= currentstatenumber

	-- don't draw anything if there's no info for this frame or it's after the last frame
	if info == nil then return end
	if notcurrent and records.endid and (id > records.endid) then return end


	renderpasttrajectory(records, id, tracknumber)


	-- draw a box where the player is
--	local camx = getxcam()
--	local camy = getycam()
	local x = math.floor(info[1]) -- - camx
	local y = math.floor(info[2]) -- - camy
	local width = 16
	local height = 32

	-- adjust the player hitbox to match up better with the world... (HACK)
	y = y + 3
	height = height - 5
	x = x + 1
	width = width - 2
	
	if (drawsavestatehitboxes and notcurrent) or (drawplayerhitbox and not notcurrent) then
		
		--local color        = notcurrent and "#7F7FFF" or "#FF7FFF"
		local outlinecolor = notcurrent and "#0000FF" or "#FF0000"

		drawboxinworld(x, y, x+width, y+height, outlinecolor)

	end

	if drawsavestatenumbers then

		-- draw the savestate number somewhere in the box
		if notcurrent then
			x = x + (tracknumber%6) * 4 - 10
			y = y + (tracknumber/6) * 6 - 15
			local message = string.format("%d", tracknumber)
			drawtextinworld(x, y, message)
		end

	end

end




local objectinfotable =
{
	-- note: default width and height is 16
	[0x061] = {name='babymario', color='#FF7F7F', width=function(i) return memory.readbyte(0x700F00+4*(i-1))==0xA and 16 or 32 end, height=function(i) return memory.readbyte(0x700F00+4*(i-1))==0x10 and 32 or 16 end, showname=false},
	[0x08C] = {name='yoshi', height=32, color='#00FF00'},
	[0x042] = {name='entrance', width=32, textyoff=8, color='#BFFF00'}, -- pipe
	[0x0D1] = {name='entrance', width=32, textyoff=8},
	[0x084] = {name='warp', width=40, height=28},
	[0x0D0] = {name='entrance', width=22, height=40},
	[0x147] = {name='entrance', width=22, height=40},
	[0x04E] = {name='door', width=11, height=32, xoff=1, yoff=8},
	[0x131] = {name='door', width=11, height=32, xoff=1, yoff=8},
	[0x093] = {name='door', width=11, height=32, xoff=1, yoff=8},
	[0x012] = {name='bossdoor', width=12, height=32, yoff=8},
	[0x001] = {name='shut', width=11, height=32, xoff=1, yoff=8},
	[0x00D] = {name='goal', height=64, yoff=-64, xoff=35, width=8},
	[0x00E] = {name='goal!', width=8, height=8},
	[0x00F] = {name='bonus', width=8, height=8},
	[0x01E] = {name='shyguy'},
	[0x0F3] = {name='jumpingshyguy'},
	[0x192] = {name='flowershyguy'},
	[0x0F2] = {name='stiltshyguy', height=48, yoff=-16},
	[0x066] = {name='pirahnaplant', width=8, height=0, guessscaling=true},
	[0x054] = {name='pirahnaplant', width=8, height=0, guessscaling=true, upsidedown=true},
	[0x09E] = {name='chomprock', width=22, height=24},
	[0x065] = {name='redcoin', color='#FF0000'},
	[0x1AF] = {name='coin', color='#FFFF00'},
	[0x115] = {name='coin'},
	[0x025] = {name='egg', color='#7FFF7F'},
	[0x024] = {name='yellowegg', color='#FFFF7F'},
	[0x023] = {name='redegg', color='#FF7F7F'},
	[0x0A0] = {name='starflower', width=32},
	[0x0AD] = {name='helpbox', scaled=true},
	[0x12C] = {name='redcoin'}, -- shyguy
	[0x08D] = {name='flyguy'}, -- stars or 1up
	[0x178] = {name='1upballoon'},
	[0x17A] = {name='yellowcoinballoon'},
	[0x1A2] = {name='star'},
	[0x100] = {name='1up'},
	[0x0BE] = {name='1up'},
	[0x0B7] = {name='1up'},
	[0x0BF] = {name='key'},
	[0x0B6] = {name='coins'},
	[0x0BC] = {name='bandit'},
	[0x0BD] = {name='coin'},
	[0x0CB] = {name='coin'},
	[0x0CC] = {name='switch'},
	[0x09D] = {name='switch', width=26, xoff=1, height=20, scaled=true, yscaled=true},
	[0x0C2] = {name='door'},
	[0x0B0] = {name='tank'},
	[0x0B1] = {name='copter'},
	[0x098] = {name='yoshiblock', width=32,height=32,yoff=-8},
	[0x06A] = {name='eggblock'}, -- yellow
	[0x06B] = {name='eggblock'}, -- green
	[0x0FA] = {name='flower', width=24, height=24},
	[0x110] = {name='flower', width=24, height=24},
	[0x064] = {name='pivot', width=8, height=8},
	[0x055] = {name='pivot', width=8, height=8},
	[0x03D] = {name='pivot', width=8, height=8},
	[0x0BA] = {name='stairs'}, -- wingedcloud
	[0x0BB] = {name='bridge'}, -- wingedcloud
	[0x0C7] = {name='beanstalk'}, -- wingedcloud
	[0x0C8] = {name='beanstalk'}, -- wingedcloud
	[0x0C0] = {name='stars'}, -- wingedcloud
	[0x0C1] = {name='stars'}, -- wingedcloud
	[0x0B7] = {name='1up'}, -- wingedcloud
	[0x0B8] = {name='flower'}, -- wingedcloud
	[0x0B5] = {name='secret'},
	[0x067] = {name='chomprocksecret', textyoff=-8},
	[0x161] = {name='bonus', width=8, height=8},
	[0x181] = {name='crazydaisy', height=24, yoff=-5},
	[0x094] = {name='growblock', width=14, height=14, scaled=true, yoff=function(i) return (memory.readword(0x701A36+4*(i-1))-256)/-32 end},
	[0x0D8] = {name='chompsign', width=24, height=24},
	[0x0A6] = {name='chomp', width=32, height=32, scaled=true},
	[0x0A9] = {name='chompshadow', width=32, height=8, scaled=true},
	[0x144] = {name='flippergate', width=16, height=64},
	[0x13C] = {name='flippergate', width=64, height=16},
	[0x04F] = {name='midring', width=16, height=44, yoff=2, xoff=function(i) return memory.readbyte(0x701400+4*(i-1)) == 2 and -14 or 16 end},
	[0x06C] = {name='bouncyarrow', width=10, xoff=1, yoff=-7, height=function(i) return 24 * 256 / memory.readword(0x701A36+4*(i-1)) end},
	[0x148] = {name='bouncyarrow', width=10, xoff=1, yoff=-7, height=function(i) return 24 * 256 / memory.readword(0x701A36+4*(i-1)) end},
	[0x06F] = {name='bouncyarrow', width=10, xoff=1, yoff=0, height=function(i) return 16 * 256 / memory.readword(0x701A36+4*(i-1)) end},
	[0x197] = {name='sign'},
	[0x198] = {name='sign'},
	[0x0EA] = {name='gusty'}, -- updown
	[0x0EB] = {name='gusty'}, -- leftright
	[0x10E] = {name='stars', width=20, height=24, scaled=true, yoff=function(i) return (memory.readword(0x701A36+4*(i-1))-256)/-6-8 end}, -- crate
	[0x17E] = {name='stars', width=20, height=20}, -- crate, balloons
	[0x177] = {name='bouncyarrow', width=20, height=20, yoff=2}, -- balloon
	[0x164] = {name='nipper'},
	[0x165] = {name='nipper'}, -- from seed
	[0x0F4] = {name='eggplant', height=24, yoff=-5, color='#7FFF7F'},
	[0x133] = {name='lanternghost'},
	[0x179] = {name='key'},
	[0x027] = {name='key'},
	[0x011] = {name='1up'},
	[0x022] = {name='egg'},
	[0x123] = {name='bucket', yoff=4},
	[0x231] = {name='babymario', width=32, height=32, color='#FF7F7F'},
	[0x230] = {name='toady', width=32, height=32},
	[0x22F] = {name='toady', width=32, height=32},
	[0x091] = {name='toadies'},
	[0x036] = {name='crusher', width=96},
	[0x051] = {name='roller', width=96, height=32, yoff=-16},
	[0x101] = {name='spikepivot'},
	[0x102] = {name='spikepivot'},
	[0x08A] = {name='platform', width=40, height=12},
	[0x089] = {name='platform', width=40, height=12},
	[0x185] = {name='platform', width=40, height=12},
	[0x186] = {name='platform', width=40, height=12},
	[0x187] = {name='platform', width=40, height=12},
	[0x188] = {name='platform', width=40, height=12},
	[0x189] = {name='platform', width=40, height=12},
	[0x18A] = {name='platform', width=40, height=12},
	[0x18B] = {name='platform', width=40, height=12},
	[0x18C] = {name='platform', width=40, height=12},
	[0x18D] = {name='platform', width=40, height=12},
	[0x18E] = {name='platform', width=40, height=12},
	[0x103] = {name='spikewidget', width=32},
	[0x0E7] = {name='burt', width=24, height=24, yoff=-4, scaled=true},
	[0x080] = {name='fireball'},
	[0x018] = {name='fire'},
	[0x194] = {name='blargg', width=32, height=32, yoff=-8},
	[0x0A5] = {name='giantblargg', width=96, height=96, yoff=-96},
	[0x076] = {name='sparky', scaled=true},
	[0x077] = {name='sparky', scaled=true},
	[0x0DA] = {name='vase', width=14, height=20, yoff=-2},
	[0x048] = {name='kamek', width=32, height=24, yoff=-4},
	[0x046] = {name='burt', width=24, height=24, xoff=-8, yoff=-18, width=128, height=function(i) return 128 - memory.readword(0x701900+4*(i-1)) end, yoff=function(i) return -72 + memory.readword(0x701900+4*(i-1))/2 end},
	[0x013] = {name='supernova', width=8, height=8},
	[0x117] = {name='donutblock'},
	[0x13D] = {name='bat'},
	[0x13E] = {name='bat'},
	[0x08B] = {name='1upbomb'},
	[0x092] = {name='rollbug'},
	[0x109] = {name='taptap'},
	[0x10A] = {name='taptap'},
	[0x10B] = {name='taptap'},
	[0x007] = {name='watermelon'},
	[0x107] = {name='seed'},
	[0x1B1] = {name='cannon', width=32, height=32, xoff=8, yoff=8},
	[0x1B2] = {name='coin'},
	[0x1B3] = {name='bandit', height=24, yoff=-6},
	[0x129] = {name='fuzzy'},
	[0x0B3] = {name='fart'},
	[0x108] = {name='milde'},
	[0x03E] = {name='weakplatform', width=64, xoff=-8},
	[0x182] = {name='dragonfly', width=8, height=8},
	[0x183] = {name='butterfly', width=8, height=8},
	[0x0F0] = {name='petal'},
	[0x0EE] = {name='petalshooter'},
	[0x143] = {name='fish', width=32},
	[0x057] = {name='platformghost', width=32},
	[0x15C] = {name='flipperswitch'},
	[0x15D] = {name='flipperswitch'},
	[0x15F] = {name='spikeplatform', width=48, height=12},
	[0x160] = {name='spikeplatform', width=48, height=12},
	[0x020] = {name='bandit', height=24, yoff=-4},
	[0x12D] = {name='yoshi', height=32, yoff=8, color='#00FF00'},
	[0x11E] = {name='spinarrow', width=28,height=28},
	[0x02D] = {name='salvo'},
	[0x02E] = {name='eyes'},
	[0x132] = {name='unshaven'},
	[0x014] = {name='bigkey', width=24, height=24},
	[0x400] = {showname=false, width=0, height=0, color='#00000000'},
	-- TODO: play through more of the game to assign the rest of these...
	-- (or discover a way to auto-calculate them)
}


local function evalnumber(x, ...)
	if type(x) == 'number' then return x
	elseif type(x) == 'function' then return x(...)
	end
end



local function drawobject(i)
	local off = 4 * (i-1)

	local status = memory.readword(0x700F00 + off)

	if status == 0 then return end -- has "doesn't exist" status flag

	local objecttype = memory.readword(0x701360 + off)
	local objectinfo = objectinfotable[objecttype] or {}

	local color = objectinfo.color or '#FFFF00'	

	if status == 0x0A then --color = '#00FF00' -- has "saddled" status flag
	elseif status == 0x0E then color = '#7F7F7F' -- has "dead" status flag
	elseif status == 0x08 then color = '#007FFF' -- has "in mouth" status flag
--	else emu.message(string.format("%X %d",status,i)) color = '#000000'
	end



	local x = memory.readword(0x7010E2 + off) + (memory.readbyte(0x7010E1 + off) / 256)
	local y = memory.readword(0x701182 + off) + (memory.readbyte(0x701181 + off) / 256)

	if objectinfo.xoff then x = x + evalnumber(objectinfo.xoff, i) end
	if objectinfo.yoff then y = y + evalnumber(objectinfo.yoff, i) end

	local width = evalnumber(objectinfo.width, i) or 16
	local height = evalnumber(objectinfo.height, i) or 16

	if objectinfo.scaled then
		local scaling = memory.readwordsigned(0x701A36 + off)
		local yscaling = objectinfo.yscaled and memory.readwordsigned(0x701A38 + off) or scaling
		width = width * scaling / 256
		height = height * yscaling / 256
	end

	local origwidth = width
	local origheight = height
	
	local xcam = getxcam()
	local ycam = getycam()

	local y1 = y
	local y2 = memory.readword(0x701CD8 + off) -- y center
	
--	x = x - xcam
--	y = y - ycam

	local xcenter = memory.readword(0x701CD6 + off) -- - xcam
	local ycenter = memory.readword(0x701CD8 + off) -- - ycam

	if objectinfo.guessscaling then
		-- I don't know how to find the size of object...
		-- but this hack method gets pretty close for dynamic objects like the pirahna plants
		local xcenterguess = x+8
		local ycenterguess = y+8
		local xguessoffby = xcenterguess - xcenter
		local yguessoffby = ycenterguess - ycenter
		width = width + math.abs(xguessoffby)
		height = height + math.abs(yguessoffby)
		if xguessoffby > 0 then x = x - xguessoffby end
		if yguessoffby > 0 then y = y - yguessoffby end
	end

	-- adjust for the objectinfo width
	x = x - (origwidth - 16)/2
	y = y - (origheight - 16)/2



	if x-xcam > 256 + 32 or y-ycam > 208 + 32 or x-xcam+width < -32 or y-ycam+height < -32 then
		-- too far offscreen to draw
		return
	end


	

	
	if height < 8 then
		if not objectinfo.upsidedown then
			y = y+height-8
		end
		height = 8
	end
	if width < 8 then
		x = x+width-8
		width = 8
	end

	if height > 511 then height = 511 end
	if width > 511 then width = 511 end

	width = math.floor(width)
	height = math.floor(height)


	-- hmm, this one is in screen-space
--	local x2 = memory.readword(0x701680 + off)
--	local y2 = memory.readword(0x701682 + off)

--	local y2 = memory.readword(0x700158 + (i-1)*2)
	

	if drawobjectcenterpoints then
		drawboxinworld(xcenter-1,ycenter-1,xcenter+1,ycenter+1,'#000000')
		drawpixelinworld(xcenter, ycenter, color)
	end
	
	if drawobjecthitboxes then
		drawboxinworld(x,y,x+width-1,y+height-1,color)
	end



	-- only draw text after this point
	if objectinfo.textyoff then y = y + objectinfo.textyoff end
	if objectinfo.textxoff then x = x + objectinfo.textxoff end	

	if drawobjectnames then
		if objectinfo.showname ~= false then -- here we want nil -> true
			local name = objectinfo.name
			if name then
				drawtextinworld(x,y-10,name)
			else
				drawtextinworld(x,y-10,'id='..string.format("%X",objecttype))
			end
		end
	end
	
--	drawtextinworld(x,y-40,'y='..string.format("%X",memory.readword(0x701182 + off)))
--	drawtextinworld(x,y-50,string.format("%08X", memory.readdword(0x701360 + 4 * (i-1))))
end






-- register a function to run when snes9x draws the screen
gui.register( function ()

	drawdeferred()

	for i = 1,24 do
		drawobject(i)
	end


	-- draw the information for this frame FOR ALL STATES
	local id = lastidvalid and lastid or getinstantid()
	for statenumber = 1,12 do
		if statenumber ~= currentstatenumber then
			local iddraw = statenumber==specialslot and id+adjustframeaheadby or id
			renderframeinfo(frameinforecords[statenumber], iddraw, statenumber)
		end
	end
	-- render the current state last so it draws on top
	renderframeinfo(frameinforecords[currentstatenumber], id, currentstatenumber)


	if drawspeedometer then

		local xvel = memory.readwordsigned(0x7000A8)
		local yvel = memory.readwordsigned(0x7000AA)
		local slope = memory.readbyte(0x7000B6) * 360 / 255
		if slope > 180 then slope = slope - 360 end
		gui.text(5,40,"xvel: "..xvel)
		gui.text(5,50,"yvel: "..yvel)
		if slope ~= 0 then
			gui.text(5,60,"angle: "..string.format("%d",math.floor(slope<0 and slope-1 or slope)))
		end

	end

end)


-- a function to run after the emulation of every frame
registeredafter = ( function ()

	local id = getinstantid()

	-- sort of a hack...
	if id == lastid then
		lastidvalid = false
	else
		lastid = id
		lastidvalid = true
	end

	-- just in case...
	if frameinforecords[currentstatenumber] == nil then frameinforecords[currentstatenumber] = {} end

	-- keep track of the last frame reached that's been saved
	frameinforecords[currentstatenumber].endid = id

	-- store the new information for this frame
	local nowinfo = getperframeinfo()
	frameinforecords[currentstatenumber][id] = nowinfo

end)


	-- run some startup code: retrieve the ghost info stored in each savestate
--	sound.clear()
	frameinforecords = {}
	for n = 1,12 do
		frameinforecords[n] = savestate.loadscriptdata(saveslot[n])
--		emu.wait()
	end


-- register a function that gets called when a numbered savestate is saved.
-- the return value(s) becomes the savestate's scriptdata saved alongside it.
savestate.registersave( function (statenumber)

	-- check for invalid save slot
	if not frameinforecords[currentstatenumber] then return end

	-- if the user switched which state they're saving to,
	-- replace that state's info table with the current one
	if statenumber ~= currentstatenumber then
		if statenumber == specialslot then
			-- F10 state is special, keep all info for it
			frameinforecords[statenumber] = copytable(frameinforecords[currentstatenumber])
		else
			-- for other states, only keep the last 1 minute of perframeinfo
			frameinforecords[statenumber] = copytablepartial(frameinforecords[currentstatenumber])
		end
		currentstatenumber = statenumber
	end

	-- save all the perframeinfo records for this state
	return frameinforecords[currentstatenumber]

end)


-- register a function that gets called when a numbered savestate is loaded.
savestate.registerload( function (statenumber)

	-- check for invalid save slot
	if not frameinforecords[currentstatenumber] then return end

	-- just switch the info record track to that state's table
	currentstatenumber = statenumber

	-- also clear rendering buffers since the time (or state) has suddenly changed
	clearalloldhist(-1)
	resetcam()
	lastidvalid = false

	-- maybe this helps
	collectgarbage()

end)

if smartsyncframeupdate then
	memory.register(0x7E0030, function()
		registeredafter()
		tickcam()
	end)
end

-- finally, the main loop
while true do
	emu.frameadvance()

	if not smartsyncframeupdate then
		registeredafter()
		tickcam()
	end
end

