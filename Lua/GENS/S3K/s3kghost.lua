-- this is sort of a "racing ghost" display for comparing savestates
-- 
-- it draws a rough hitbox over your position and more importantly
-- draws the same hitbox over where the other savestates were
-- at the current in-game time
-- thus you can easily judge whether you're faster or slower
-- than you were the last few times
-- 
-- additionally, you can watch someone else's movie and then save on a slot you won't overwrite
-- then play from the start and you can tell whether you're faster than they were
-- (this can be quite useful for making TASes)
-- note that you must save and keep a savestate around for its ghost to be drawn
-- you might find the "Save Backup State" hotkey useful for this purpose (it saves to state #10)
-- you can also rename the .luasav files to be #11 through #14 to compare against multiple other movies.
-- (note that the luasav filename must be correct, as in "YOURROMNAME - YOURMOVIENAME.gs14.luasav") 
-- 
-- this has been modified from the sample script that came with Gens11
-- to work for S3&K instead of Sonic 1, and to have smoother rendering.

-- KNOWN ISSUES: causes huge obvious desyncs if used at the same time as
--               certain other scripts such as the "movie watcher" script.


-- define which variable live across multiple runs of the script
gens.persistglobalvariables{
	currentstatenumber = 0,	

	-- buffers for retrieving frame info.
	-- we could calculate this info on the fly instead of buffering it,
	-- but then there would be some small camera offset problems
	-- and occasional glitches with the hitbox rendering.
	-- we fix this by using the buffer to maintain a continuous sequence
	-- even when the elements in that sequence come in bursts.
	-- (so we're compensating for the fact that the game logic updates
	-- are not exactly synced to emulation frames.)
	bufferindex = 1,
	idbuffer = {},
	camxbuffer = {},
	camybuffer = {},
	lastrenderid = 0,

}

-- a record of frame information... because it's per-savestate,
-- we use registersave instead of persistglobalvariables to save it.
-- looks like: {savenumber1={time1=x1y1,time2=x2y2,...},savenumber2=...}
frameinforecords = {} -- initially it's an empty table





-- a helper function to convert from unpacked x,y position to combined xy
local function packxy(x,y)
	return OR(x, SHIFT(y, -16))
end
-- a helper function to convert from packed xy position to separate x,y
local function unpackxy(xy)
	local x = AND(xy, 0xFFFF)
	local y = SHIFT(xy, 16)
	return x,y
end




local readwordsigned = memory.readwordsigned

-- a helper function to get current player position info
-- (packed into a single int, since we're going to store a LOT of this data...)
--
-- objindex says which object to get the position of.
-- player 1 always is at index 0 (the default), and player 2 is at index 1 (if present).
--
local function getperframeinfo(objindex)
	local addrplus = (objindex or 0) * 0x4a
	local x = readwordsigned(0xffb010 + addrplus)
	local y = readwordsigned(0xffb014 + addrplus)
	return packxy(x,y)
end



-- a helper function to combine the current in-game time
-- and current level into a single unique number
-- most games could just return gens.getframecount() instead
-- but in-level time tends to be better in the case of a Sonic game
-- and this has the extra benefit of making everyone match up at the start of each act
function getinstantid()
	local level = 1 + memory.readword(0xffee4e)
	local frames = memory.readbyte(0xfffe25)
	local seconds = memory.readbyte(0xfffe24)
	local minutes = memory.readbyte(0xfffe23)
	local id = frames + 60*(seconds + 60*(minutes + 10*(level))) -- I use 10 here because the timer can't exceed 9:59
	return id
end





-- a helper function to draw the info of the given frame
function renderframeinfo(records, id,camx,camy, tracknumber)

	if not records then return end

	local packedinfo = records[id]
	if not packedinfo then return end

	local xpos,ypos = unpackxy(packedinfo)

	local notcurrent = (tracknumber ~= currentstatenumber)

	-- don't draw anything if it's after the last frame
	if notcurrent and (id > (records.endid or id)) then return end

	local othermovie = (tracknumber >= 10) -- different color for special external movies

	-- draw a box where the player is
	local x = xpos - camx
	local y = ypos - camy
	local hwidth = 10
	local hheight = 16
	local color        = notcurrent and (othermovie and 0xFF7F7F7F or 0x7F7FFF7F) or 0xFF7FFF3F
	local outlinecolor = notcurrent and 0x000000FF or 0xFFFFFFFF
	gui.drawbox(x-hwidth, y-hheight, x+hwidth, y+hheight, color, outlinecolor)

	-- draw the savestate number somewhere in the box
	if notcurrent then
		x = x + (tracknumber%6) * 4 - 10
		y = y + (tracknumber/6) * 6 - 15
		local message = string.format("%d", tracknumber)
		gui.text(x, y, message, "#FFFFFF")
	end

end





-- register a function to run when gens draws the screen
gui.register( function ()

	-- don't draw if there's no player (this check could be better)
	if memory.readword(0xffb000) == 0 then return end

	-- calculate/retrieve the frame info
	local id,camx,camy
	if #idbuffer > 0 then
		local idindex = 1
		repeat
			id = idbuffer[idindex]
			camx = camxbuffer[idindex]
			camy = camybuffer[idindex]
			idindex = idindex + 1
		until not id or id > lastrenderid
	end
	id = id or getinstantid()+1
	camx = camx or readwordsigned(0xffee78)
	camy = camy or readwordsigned(0xffee7c)
  
  -- reset buffer for next update pass
	bufferindex = 1
	lastrenderid = id

	-- draw the information for this frame FOR ALL STATES
	for statenumber,records in pairs(frameinforecords) do
		renderframeinfo(records, id,camx,camy, statenumber)
	end

end)


-- register a function to get called every time the game changes its frame counter.
-- we do this to stay more precisely synced with the game's logic than a builtin framecounter could.
memory.register( 0xfffe0f, function()

	-- don't update if there's no player (this check could be better)
	if memory.readword(0xffb000) == 0 then return end

	local id = getinstantid()

	-- just in case...
	if not frameinforecords[currentstatenumber] then frameinforecords[currentstatenumber] = {} end

	-- store the new information for this frame
	frameinforecords[currentstatenumber][id] = getperframeinfo()

	if bufferindex < 10 then -- (arbitrary cutoff on array length in case my other code somehow goes horribly wrong)

		idbuffer[bufferindex] = id
		camxbuffer[bufferindex] = readwordsigned(0xffee78)
		camybuffer[bufferindex] = readwordsigned(0xffee7c)
		bufferindex = bufferindex + 1

	end

end)


-- register a function to run whenever the game boots up or resets
-- (this also runs once immediately if the game is already running)
gens.registerstart( function ()

	-- run some startup code: retrieve the ghost info stored in each savestate
	sound.clear() -- as a courtesy, clear the sound before operations like this that can take more than 0.5 seconds or so
	frameinforecords = {}
	for n = 0,14 do -- (1-9, 0, and also the hidden #10-#14 if they're there)
		local miscdata,records = savestate.loadscriptdata(n)
		frameinforecords[n] = records or {}
		gens.wait()
	end

end)


local saveversion = 3
-- saveversion exists in case we change miscdata 
-- enough to want to not load it from existing savestates in a future version of this script.


-- register a function that gets called when a numbered savestate is saved.
-- the return value(s) becomes the savestate's scriptdata saved alongside it.
savestate.registersave( function (statenumber)

	-- if the user switched which state they're saving to,
	-- replace that state's info table with the current one
	if statenumber ~= currentstatenumber then
		frameinforecords[statenumber] = copytable(frameinforecords[currentstatenumber])
		currentstatenumber = statenumber
	end

	-- keep track of the last frame reached that's been saved
	frameinforecords[currentstatenumber].endid = getinstantid()

	-- some data we'll always save/load, since it's small enough
	local miscdata = {
		saveversion,
		bufferindex,
		idbuffer,
		camxbuffer,
		camybuffer,
		lastrenderid,
	}

	-- save that data and all the perframeinfo records for this state
	return miscdata, frameinforecords[currentstatenumber]

end)


-- register a function that gets called when a numbered savestate is loaded.
savestate.registerload( function (statenumber, miscdata)

	-- just switch the info record track to that state's table
	currentstatenumber = statenumber

	-- some data we'll always save/load, since it's small enough
	if miscdata and miscdata[1] == saveversion then
		local loadversion
		loadversion,bufferindex,idbuffer,camxbuffer,camybuffer,lastrenderid = unpack(miscdata)
	end
	
	-- note that we don't load frameinforecords from file every time.
	-- we have it in memory already and keep using it from there as an optimization.

end)





-- this is roughly equivalent to enabling "latency compensation 2"
-- except this way I actually have full script control over it...
-- I use this because my logic above actually relies on it to have enough sample points.
-- (I could remove that reliance most of the time by keeping a history, but this way seems better.)
local tempsave = savestate.create()
local inrb = false
gens.registerbefore(function()
	if inrb then return else inrb=true end
	gens.emulateframeinvisible()
	savestate.save(tempsave)
	gens.emulateframeinvisible()
	inrb=false
end)
gens.registerafter(function()
	savestate.load(tempsave)
end)
