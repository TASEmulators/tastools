-- hitbox display script for Metal Combat - Falcon's Revenge
-- also displays health amounts, weakpoint/status, and bullet depth positions.
-- and usually shows a big obvious warning message if one of your bullets completely misses.
-- and makes it possible to fire shots at the very edge of the screen.
-- 
-- and has a "prediction mode" (hold Z) that shows hitboxes in their future positions
-- where they will be by the time your bullet reaches them if you were to fire now,
-- so you can click on a hitbox in that mode and have a very high chance of hitting.
-- (it's sometimes off a little due to unpredictable lag but is usually correct)
-- 
-- and does some funky stuff with the cursor position sometimes when you're not firing,
-- for no real reason besides input display shenanigans while making a TAS.
-- 
-- TODO:
-- - display enemy bullet hitboxes
-- - display tutorial fish hitboxes

do
	local scope = joypad.get(2) -- super scope plugged into port #2
	assert(scope and scope.x, "You must be running Snes9x 1.51 with superscope enabled.")
end

emu = emu or snes9x or gens gens = emu or snes9x or gens snes9x = emu or snes9x or gens

local readbyte = memory.readbyte
local readword = memory.readwordsigned -- signed! (since I use that a lot more)
local readlong = memory.readlong


-- the hitboxes in metal combat are lots of little rectangles
-- which would look terrible if drawn individually,
-- so here are some functions that combine adjacent or overlapping rectangles
-- so that I can draw all rectangles of a single part lumped together
-- with only one outline around the edges of the entire combined region.
-- 
-- to use, call addtoregion() for each little rectangle,
-- and call drawregion() to draw the resulting combined region as a whole.


local remove = table.remove

local function initregion()
	return { edges={}, rects={} }
end

local function pointinrects(x,y,x2,y2,rects)
	if x ~= x2 then x2 = x + ((x2 > x) and 1 or -1) end
	if y ~= y2 then y2 = y + ((y2 > y) and 1 or -1) end
	for i,rect in ipairs(rects) do
		if x >= rect[1] and y >= rect[2] and x <= rect[3] and y <= rect[4] then
			if x == rect[1] or y == rect[2] or x == rect[3] or y == rect[4] then
				if x2 >= rect[1] and y2 >= rect[2] and x2 <= rect[3] and y2 <= rect[4] then
					return true
				end
			else
				return true
			end
		end
	end
	return false
end

local function getedgeintersectiontimes(edge, edges2)
	local result = {}
	for i,edge2 in ipairs(edges2) do
		local x1 = edge[1]
		local y1 = edge[2]
		local x2 = edge[3]
		local y2 = edge[4]
		local x3 = edge2[1]
		local y3 = edge2[2]
		local x4 = edge2[3]
		local y4 = edge2[4]
		local denom = (y4-y3)*(x2-x1)-(x4-x3)*(y2-y1)
		if denom > 0.001 or denom < -0.001 then
			local t =  ((x4-x3)*(y1-y3)-(y4-y3)*(x1-x3))/denom
			if t > 0 and t < 1 then
				local t2 = ((x2-x1)*(y1-y3)-(y2-y1)*(x1-x3))/denom
				if t2 >= 0 and t2 <= 1 then
					result[#result+1] = t
				end
			end
		end
	end
	return result
end

local function clipedge(edge, edges2, inside)
	local result = {}
	local ts = getedgeintersectiontimes(edge, edges2)
	ts[#ts+1] = 1
	table.sort(ts)
	local prevx = edge[1]
	local prevy = edge[2]
	for i,t in ipairs(ts) do
		local x = edge[1] + t * (edge[3] - edge[1])
		local y = edge[2] + t * (edge[4] - edge[2])
		if not inside then
			result[#result+1] = {prevx,prevy,x,y}
		end
		inside = not inside
		prevx = x
		prevy = y
	end
	return result
end

local function clipedges(edges, edges2, rects, cornerbiased)
	local result = {}
	
	for i,edge in ipairs(edges) do
		local startinside = pointinrects(edge[1],edge[2],edge[3],edge[4],rects)
		local clippededges = clipedge(edge, edges2, startinside)
		for j,clippededge in ipairs(clippededges) do
			result[#result+1] = clippededge
		end
	end
	
	return result
end

local function addtoregion(region, x1,y1,x2,y2)

	local rects = region.rects
	local newrect = {x1,y1,x2,y2}

	local edges = region.edges
	local newedges = { {x1,y1,x2,y1}, {x2,y1,x2,y2}, {x2,y2,x1,y2}, {x1,y2,x1,y1} }

	local newedgesclipped = clipedges(newedges, edges, rects, false)
	local edgesclipped = clipedges(edges, newedges, {newrect}, true)
	
	--edgesclipped = {}
	for i,v in ipairs(newedgesclipped) do
		edgesclipped[#edgesclipped+1] = v
	end
	region.edges = edgesclipped
	rects[#rects+1] = newrect
end

local function drawregion(region, color)
	for i,edge in ipairs(region.edges) do
		gui.drawline(edge[1],edge[2],edge[3],edge[4],color)
	end
end

local function drawpartinfo(v)
	local color1 = v[4]
	local color2 = 0
	if AND(color1,0x80FF0000) == 0 then color2 = 0xFFFFFF00 end
	color1 = OR(AND(color1,0xFFFFFF00),0xaf)
	color2 = OR(AND(color2,0xFFFFFF00),0x7f)
	gui.drawtext(v[1],v[2],tostring(v[3]), color1,color2)
end

local function clamp(x,min,max)
	if x < min then return min end
	if x > max then return max end
	return x
end

local function clamp128(x)
	if AND(0x80,x) == 0 then
		x = AND(x,0x7F)
	else
		x = OR(x,0xFF80)
		x = -AND(0xFFFF,-x) -- for Lua, convert signed 16-bit int to signed number
	end
	return x
end

--local abs = math.abs
local distsqfromcenter = function(x,y)
	return (x-128)*(x-128)+(y-112)*(y-112)
end

local segcolors =
{
	0xFFFFFFFF,
	0xFFFF00FF,
	0xFF7F00FF,
	0xFF0000FF,
	0x00FF00FF,
	0x00FFFFFF,
	0x007FFFFF,
	0x0000FFFF,
	0x7F00FFFF,
	0xFF00FFFF,
	0x7F7F7FFF,
	0x000000FF,
}

local function calculatedisplay()
	local xadjust = readword(0x7e025d) + 40
	local yadjust = readword(0x7e025f) + 1
	local gamestate = memory.readbyte(0x7e026d)

	local hitboxregioninfo = {}
	local parthealthinfo = {}

	local maxpart = 31


	for ii=0,maxpart do

		-- try not to show hitbox junk when not actually in gameplay mode
		-- this might not be 100% accurate but it works well enough for me.
		if (gamestate < 19 or gamestate > 28) and (gamestate < 64 or gamestate == 67) then
			break
		end

		local i = ii

		if readword(0x971726 + ii*2) >= 0 then -- check if part is valid


		-- oh boy, reverse engineering collision checking logic sure is fun...

		local x = readword(0x7e15a6 + i*2) - xadjust
		local y = readword(0x7e15e6 + i*2) - yadjust
		local register0 = AND(0xFFFF, readword(0x9714D3))
		local register1 = AND(0xFFFF, readword(0x9714D4))
		local registerX = i*2
		local y112 = 2 * readword(0x7e1666 + registerX)
		local ldayc77 = memory.readword(OR(0x7e0000,register0+y112))
		register0 = register0 + ldayc77
		local hitboxaddr = OR(0x7e0000,register0)
		local ldayed06 = memory.readword(hitboxaddr)
		local register22 = AND(0xFF,ldayed06)


		local partflags = memory.readword(0x971726 + registerX)
		
		local partenabled = AND(partflags, 0x10) ~= 0
		local partdamaged = AND(partflags, 0x01) ~= 0
		local partshielded = AND(partflags, 0x2000) ~= 0
		local partvital = AND(partflags, 0x4000) ~= 0
		local partexposed = AND(partflags, 0x400) ~= 0
		local partprimary = AND(partflags, 0x20) ~= 0

		local xpart,ypart = x,y
	
		local region = initregion()
		
		for segment=1,register22 do
		
			hitboxaddr = hitboxaddr + 1
			local xd = clamp128(readword(hitboxaddr))
			local xleft = x + xd

			hitboxaddr = hitboxaddr + 1
			local yd = clamp128(readword(hitboxaddr))
			local ytop = y + yd

			hitboxaddr = hitboxaddr + 1
			local xd2 = clamp128(readword(hitboxaddr))
			local xright = x + xd2

			hitboxaddr = hitboxaddr + 1
			local yd2 = clamp128(readword(hitboxaddr))
			local ybottom = y + yd2

			if not partenabled and xright <= 0 then break end


			if partenabled then
				local color = 0x0000003F
				if partprimary then color = OR(color, 0x00000070) end
				if partexposed then color = OR(color, 0x00FF0070) end
				if partshielded then color = OR(color, 0x0000FF70) end
				if partvital then color = OR(color, 0xFF000070) end
				gui.fillbox(xleft,ytop,xright-1,ybottom-1,color)
			end

			
			addtoregion(region, xleft,ytop,xright,ybottom)

			if segment == 1 then
				xpart,ypart = xleft+2,ytop+2
			end
		end


		local foocolor = segcolors[i%#segcolors+1]

		if not partenabled then foocolor = AND(foocolor, 0xFFFFFF3F) end

		hitboxregioninfo[#hitboxregioninfo+1] = {region,foocolor}

		
		local parthealth = readword(0x7E1866 + registerX)		
		
		parthealthinfo[#parthealthinfo+1] = {xpart,ypart,parthealth,foocolor}

		end
	end

	return hitboxregioninfo,parthealthinfo

end


local function drawhudinfo()
	-- "adjust aim" setting compensation
	local xadjust = readword(0x7e025d) + 40
	local yadjust = readword(0x7e025f) + 1


--	gui.drawtext(50,20,xadjust..', '..yadjust)


	local something = 0
--	local something = string.format("%X", readword(0x97FC33)) -- last attack power
--	local something = readword(0x7e1ab7) -- last attack power
--	local something = readword(0x7e1004) -- last X attack coordinate (in screen coords)
--	local something = readword(0x7e1062) -- last Y attack coordinate (in screen coords)
--	local something = string.format("%X", readword(0x7e1826)) -- changes per hit, not sure...
--	local something = string.format("%X", readword(0x7e15F2)) -- monkey rightmost (left) arm piece X coord
--	local something = string.format("%d", readword(0x7EEDC2 + 20))
--	local something = string.format("%X", readword(0x9717E6)) -- not sure, seems important (maybe number of body parts to check collision?)
--	local something = string.format("%X", readword(readword(0)))
--	local something = string.format("%X", AND(0xFFFF, readword(0x9714D3)))
--	local something = string.format("%X", AND(0xFFFF, readword(0x9714D3)))

--	something = string.format("%X", readword(0x7EE25C))
		--something = readword(0x7E0257) -- shot discharge timer??
		
--	gui.text(100,10,tostring(something), 0xffffffff, 255)


	local shotdists = {readword(0x970CB6), readword(0x970CB8), readword(0x970CBA)}

	-- draw shot tracking status
	for i,v in ipairs(shotdists) do
		local x = 100+i*4
		local y = 2 + v/8
		gui.drawrect(x-1,y-1,x+1,y+1,0xFFFFFFFF) 
		local color
		if v > 256 then color = 0xFF0000FF end
		if v < 256 then color = 0x000000FF end
		if v == 256 then color = 0x00FFFFFF end
		gui.drawpixel(x,y,color) 
		if v > 256 then
			gui.drawtext(100,0+i*6,"MISSED", 'red')
		end
	end


	-- draw last-attack cursor
	local atkx = readword(0x7e1004) -- last X attack coordinate (in screen coords (already adjusted))
	local atky = readword(0x7e1062) -- last Y attack coordinate (in screen coords (already adjusted))
	gui.drawline(atkx-3,atky-3,atkx+3,atky+3,0xFFFFFF7F)
	gui.drawline(atkx+3,atky-3,atkx-3,atky+3,0xFFFFFF7F)
	gui.drawpixel(atkx,atky,0xFF0000FF)



	-- draw cursor-adjusted screen boundaries
	--gui.text(100,10,inp.xmouse..', '..inp.ymouse)
	local xminshot,xmaxshot = 0+xadjust,255+xadjust
	local yminshot,ymaxshot = 0+yadjust,223+yadjust
	gui.drawrect(xminshot-1,yminshot-1,xmaxshot+1,ymaxshot+1, 0xFF0000FF)
	gui.drawrect(xminshot-2,yminshot-2,xmaxshot+2,ymaxshot+2, 0xFF0000AF)
	gui.drawrect(xminshot-3,yminshot-3,xmaxshot+3,ymaxshot+3, 0xFF00007F)

	inp = input.get()
	if inp.xmouse < 0 then inp.xmouse = 0 end
	if inp.xmouse > 255 then inp.xmouse = 255 end
	if inp.ymouse < 0 then inp.ymouse = 0 end
	if inp.ymouse > 223 then inp.ymouse = 223 end

		
	-- draw adjusted cursor
	local xcurs = inp.xmouse + xadjust 
	local ycurs = inp.ymouse + yadjust 
	gui.drawbox(xcurs-3,ycurs-1,xcurs+3,ycurs+1,0x000000FF)
	gui.drawbox(xcurs-1,ycurs-3,xcurs+1,ycurs+3,0x000000FF)
	gui.drawline(xcurs-3,ycurs,xcurs+3,ycurs,0xFFFF00FF)
	gui.drawline(xcurs,ycurs-3,xcurs,ycurs+3,0xFFFF00FF)

	-- draw non-adjusted cursor
	local xcurs = inp.xmouse 
	local ycurs = inp.ymouse 
	gui.drawbox(xcurs-3,ycurs-1,xcurs+3,ycurs+1,0x000000FF)
	gui.drawbox(xcurs-1,ycurs-3,xcurs+1,ycurs+3,0x000000FF)
	gui.drawline(xcurs-3,ycurs,xcurs+3,ycurs,0x00FFFFFF)
	gui.drawline(xcurs,ycurs-3,xcurs,ycurs+3,0x00FFFFFF)

	-- draw enemy center xpos
	local xcen = readword(0x7e00b0) - (inp.xmouse + xadjust)
	gui.drawbox(xcen+104-1, 34-1, xcen+104+1, 34+1, 0x7F00FFFF)




	local playerhealth2 = memory.readword(0x7e1966)
	local playerhealth = readword(0x7e1968) + math.floor(100*playerhealth2/65536)/100
	local message = tostring(playerhealth)
	if playerhealth < 0 then message = message..' !!!' end
	gui.text(50+clamp(playerhealth,0,114),23,message, 0, 255)
	
	local enemyhealth2  = memory.readword(0x7e1826)
	local enemyhealth  = readword(0x7e1866) + math.floor(100*enemyhealth2/65536)/100
	local message = tostring(enemyhealth)
	if enemyhealth < 0 then message = message..' !!!' end
	gui.text(50+clamp(enemyhealth,0,114),31,message, 0, 255)


	local playercharge = memory.readbyte(0x7e0393)
	gui.text(80+playercharge,168,playercharge)
end


guiregister = (function()

	local inp = input.get()
	if inp.tab or inp.capslock or (inp.Z and not joypad.get(2).fire) then return end

	local scope = joypad.get(2) -- super scope plugged into port #2

	gui.clearuncommitted()



	drawhudinfo()



	hitboxregioninfo,parthealthinfo = calculatedisplay()

	for i,v in ipairs(hitboxregioninfo) do
		drawregion(v[1],v[2])
	end
	for i,v in ipairs(parthealthinfo) do
		drawpartinfo(v)
	end






--	local inp = input.get()
--	gui.drawline(0,0, inp.xmouse,inp.ymouse, 0xff00007f)
--	gui.drawline(0,224, inp.xmouse,inp.ymouse, 0xff00007f)
--	gui.drawline(255,0, inp.xmouse,inp.ymouse, 0xff00007f)
--	gui.drawline(255,224, inp.xmouse,inp.ymouse, 0xff00007f)
	
end)



local xpaddle1,ypaddle1 = 1,112
local xpaddle2,ypaddle2 = 255,112
local xball,yball = 1,112
local xballvel,yballvel = 4,3

-- just some code to do funny things with the cursor position
local function pong()
	xball = xball + xballvel
	yball = yball + yballvel
	if yball < 0 or yball > 223 then
		yballvel = -yballvel
		yball = yball + yballvel*2
	end
	if xball < 0 or xball > 255 then
		xballvel = -xballvel
		if (xball < 128 and math.abs(yball - ypaddle1) > 10) or (xball > 128 and math.abs(yball - ypaddle2) > 10) then
--			gui.popup(tostring(math.abs(yball - ypaddle1)))
--			gui.popup(tostring(math.abs(yball - ypaddle2)))
			xball = 128 -- paddle missed
--			xball = xball + xballvel*2 -- bounce off of paddle
			yballvel = 0
		else
			xball = xball + xballvel*2 -- bounce off of paddle
		end
		yballvel = yballvel + math.random(-1,1)
	end
	
	if ypaddle1 > yball then
		ypaddle1 = ypaddle1 - 3
	end
	if ypaddle1 < yball then
		ypaddle1 = ypaddle1 + 3
	end
	if ypaddle2 > yball then
		ypaddle2 = ypaddle2 - 3
	end
	if ypaddle2 < yball then
		ypaddle2 = ypaddle2 + 3
	end
	
	local i = 1 + math.floor(emu.framecount() % 4)
	
	local positions = { {x=xball,y=yball}, {x=xpaddle1,y=ypaddle1}, {x=xball,y=yball}, {x=xpaddle2,y=ypaddle2}, }
	
	joypad.set(2, positions[i])
	
	joypad.set(2, {x=math.random(100,150), y=math.random(100,150)})

--[[
	local xadjust = readword(0x7e025d) + 40
	local yadjust = readword(0x7e025f) + 1
	local x = readword(0x7e15a6 + 2*2) - xadjust
	local y = readword(0x7e15e6 + 2*2) - yadjust
	local xp = readword(0x7ee676)
	local yp = readword(0x7ee677)
	x = x + xp
	y = y + yp
	gui.drawtext(x,y,'HI')
	gui.drawtext(100,10,xp..', '..yp)]]
end









-- draws too soon, need 1-frame delay for best alignment
--gui.register(guiregister)

--local lastautofireframe = 0

local tempstate = savestate.create()
local simulating = false

local prevjoy = {x=0,y=0}

emu.registerbefore(function()
	local inp = input.get()
	if inp.E then return end
	if not simulating then
		if inp.J then -- shoot on half-charge boundary (only useful for 0x charge level)
			if memory.readbyte(0x7e0393) >= 44 then
				joypad.set(2, {fire=true})
			end
		end
		if inp.K or inp.L then -- shoot on next full charge boundary
			if memory.readbyte(0x7e0393) >= 92 then
				joypad.set(2, {fire=true})
				--gui.popup(tostring(memory.readbyte(0x7e0393)))
			end
		end
	end
	local joy = joypad.get(2) -- super scope plugged into port #2
	if inp.R then
		pong() -- temporary input display testing
	else
		if not (inp.leftclick or inp.H or inp.K or inp.J or inp.L or (inp.xmouse > 255 and inp.ymouse > 223) or inp.scrolllock) then
		
			-- some code for making the cursor aim look "elastic" for input display fun
			-- it snaps to the right position when firing a shot or if you turn on scroll lock
		
			local xvel = joy.x - prevjoy.x
			local yvel = joy.y - prevjoy.y
			local xvel2 = inp.xmouse - joy.x
			local yvel2 = inp.ymouse - joy.y

			if xvel > 90 then yvel = yvel*90/xvel xvel = 90 end
			if xvel < -90 then yvel = yvel*90/-xvel xvel = -90 end
			if yvel > 90 then xvel = xvel*90/yvel yvel = 90 end
			if yvel < -90 then xvel = xvel*90/-yvel yvel = -90 end
			if xvel2 > 90 then yvel2 = yvel2*90/xvel2 xvel2 = 90 end
			if xvel2 < -90 then yvel2 = yvel2*90/-xvel2 xvel2 = -90 end
			if yvel2 > 90 then xvel2 = xvel2*90/yvel2 yvel2 = 90 end
			if yvel2 < -90 then xvel2 = xvel2*90/-yvel2 yvel2 = -90 end

			xvel = (xvel*7/8 + xvel2*1/8)
			yvel = (yvel*7/8 + yvel2*1/8)
--[[			
			if math.abs(xvel) < 1 and math.abs(yvel) < 1 then
				if xvel == 0 then
					yvel = yvel / math.abs(yvel)
				elseif yvel == 0 then
					xvel = xvel / math.abs(xvel)
				else
					div = math.min(math.abs(xvel),math.abs(yvel))
					xvel = xvel / div
					yvel = yvel / div
				end
			end
]]
			
--			if math.floor(math.abs(xvel)) > 0 or math.floor(math.abs(yvel)) > 0 then
				--joypad.set(2, {x = (joy.x + inp.xmouse)/2})
				joypad.set(2, {x = joy.x + xvel, y = joy.y + yvel})
--			else
--				joypad.set(2, {x = inp.xmouse, y = inp.ymouse})
--				joy.x = inp.xmouse
--				joy.y = inp.ymouse
--			end
		end
	end
	prevjoy = joy
--	if inp.delete then emu.pause() end

	-- always allow firing (snes9x assumes x=0 or y=0 means 'offscreen' as a convenience for games that require shooting offscreen, but we don't want that for this game)
	joypad.set(2, {offscreen=false})

	if inp.X or (inp.xmouse > 255 and inp.ymouse > 223 and not (inp.leftclick or inp.H)) then
		joypad.set(2, {offscreen=true, X=256,Y=224})
		
--				joypad.set(2, {x = joy.x + xvel, y = joy.y + yvel})
	end

end)

local userloaded = false

savestate.registersave(function(slotnum)
--	return lastautofireframe
end)
savestate.registerload(function(slotnum,autofireframe)
--	lastautofireframe = autofireframe or 0
	if slotnum >= 0 and slotnum <= 12 then userloaded = true end

	-- jolt elastic position for input display again
	local inp = input.get()
	local joy = joypad.get(2)
	if inp.x ~= joy.x and inp.y ~= joy.y then
		prevjoy.x = joy.x + math.random(-64,64)
		prevjoy.y = joy.y + math.random(-64,64) 
	end
end)


while true do
	
	guiregister()
	
	-- hack to disable player 1's Y button so I don't have to reconfigure my regular controls to use it for something else
	local t = joypad.get(1)
	t.Y = nil
	joypad.set(1, t)

	
	emu.frameadvance()


	local inp = input.get()
	if inp.Z and not joypad.get(2).fire then -- prediction
		simulating = true
		savestate.save(tempstate)

		local numframes = 28 -- about how long it takes for a shot to hit the enemy

		if inp.K or inp.L then -- if waiting until charge, compensate for the remaining charge time
			local charge = memory.readbyte(0x7e0393)
			if charge < 94 then
				numframes = numframes + ((100 - charge) * 2/3)
			end
		end
		if inp.J then -- if waiting until half-charge, compensate for the remaining charge time
			local charge = memory.readbyte(0x7e0393)
			if charge < 44 then
				numframes = numframes + ((50 - charge) * 2/3)
			end
		end
		
		for i=1,numframes-1 do
			emu.speedmode("maximum")
			emu.frameadvance()
			if userloaded or not input.get().Z or joypad.get(2).fire then break end
--			if userloaded or not input.get().Z then break end
		end
		if not (userloaded or not input.get().Z or joypad.get(2).fire) then
--		if not (userloaded or not input.get().Z) then
			hitboxregioninfo,parthealthinfo = calculatedisplay()
			emu.frameadvance()
		end
		if not (userloaded or not input.get().Z or joypad.get(2).fire) then
--		if not (userloaded or not input.get().Z) then
			savestate.load(tempstate)
			emu.speedmode("normal")
			drawhudinfo()
			for i,v in ipairs(hitboxregioninfo) do
				drawregion(v[1],v[2])
			end
			for i,v in ipairs(parthealthinfo) do
				drawpartinfo(v)
			end
		else
			if not userloaded then
				savestate.load(tempstate)
			end
		end
	end
	emu.speedmode("normal")
	simulating = false

	userloaded = false
end