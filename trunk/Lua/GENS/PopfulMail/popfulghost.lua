
-- savestate comparison display
-- (racing ghost script)
-- and velocity trails
-- for Popful Mail (SegaCD)

-- special controls:
-- Y and U on the keyboard adjust the "frame fudge" amount
-- so that if you get ahead of a ghost you want to compare with
-- you can use those to cancel out the frames you're already ahead by.
-- Y+U at the same time resets the fudge amount to 0 frames.


-- define which variables should live across multiple runs of the script
gens.persistglobalvariables{
	currentstatenumber = 0,
}

 print("currentstatenumber = " .. tostring(currentstatenumber))


-- record of frame information, initially empty
-- (the key is an instantid, the value is a perframeinfo)
frameinforecords = {} -- a table that contains one table per savestate

-- a helper function to get the info we want to store per in-game frame
-- (for now this is only the player position)
-- TODO: pack it in an integer to save on table garbage collecting time
function getperframeinfo()
	local x = memory.readlong(0xFFA024) / 65536
	local y = memory.readlong(0xFFA020) / 65536
	return {x,y}
end

-- a helper function to combine the current in-game time
-- and current level into a single unique number
function getinstantid()
	--local level = 1 -- + memory.readword(0xfffe10)

	local curhp = memory.readword(0xFFA056)
	local gold = memory.readword(0xFFEBF2)
	local hasboomerang = AND(memory.readbyte(0xFFEBCC), 4) ~= 0
	local hasfirebreath = AND(memory.readbyte(0xFFEBEC), 8) ~= 0
	local desiredgold = 0
	if not hasboomerang then desiredgold = desiredgold + 2000 end
	if not hasfirebreath then desiredgold = desiredgold + 3000 end
	if desiredgold < gold then desiredgold = gold end
	local value = gold/1 + curhp -- estimate 1 frame per HP lost and 1 frame per gold missing
	local basevalue = desiredgold/1 + 100
	local debtframes = basevalue - value
	local totaltime = gens.framecount()+debtframes

	local id = totaltime
--[[
	-- unfortunately the frame number stored at 0xfffe25
	-- will sometimes fail to increment evenly (1,2,3,3,5,6,7,7,9,...)
	-- so pretend it got incremented if the value stays the same for 2 frames
	if lastidvalid and id == lastid then
		id = id + 1
	end
]]
--	id = gens.framecount() -- FIXME

	return id
end


function renderpasttrajectory(records, id, tracknumber)

--	if tracknumber == -1 then return end

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

	local camx = -memory.readwordsigned(0xFFE600) + 128
	local camy = memory.readwordsigned(0xFFE380) + 128
	
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
		
	for i=1,j-2 do
	
		local a = points[i]
		local b = points[i+1]
		local ax = a[1] - camx - 0.49999
		local ay = a[2] - camy - 17.49999
		local bx = b[1] - camx - 0.49999
		local by = b[2] - camy - 17.49999
		
		local transp = 255 * (j-i) / j
		local color = OR(AND(outlinecolor, 0xFFFFFF00), transp)

--		gui.drawline(ax,ay,bx,by, color, skipfirst)

		local dx = bx - ax
		local dy = by - ay
		local di = ids[i] - ids[i+1]

--		local speed = math.max(math.abs(dx),math.abs(dy)) / di
		local xspeed = math.abs(dx) / di
		local speed = xspeed
		
		if math.abs(dx) < 320 and math.abs(dy) < 224 then
		
			if speed < 2 then    color = OR(alt and 0x00000000 or 0xFF7F0000, transp)
			elseif speed < 2.99999 then    color = OR(0xFF000000, transp)
			elseif xspeed >= 8 then    color = OR(0x00FF0000, transp)
			elseif xspeed > 3.00001 then    color = OR(0x00FFFF00, transp)
			end
		
			if dx < 0 or dy < 0 then
				gui.drawline(ax-12,ay-15,bx-12,by-15, color, skipfirst)
			end
			if dx < 0 or dy > 0 then
				gui.drawline(ax-12,ay+16,bx-12,by+16, color, skipfirst)
			end
			if dx > 0 or dy < 0 then
				gui.drawline(ax+11,ay-15,bx+11,by-15, color, skipfirst)
			end
			if dx > 0 or dy > 0 then
				gui.drawline(ax+11,ay+16,bx+11,by+16, color, skipfirst)
			end

			skipfirst = true
		end		

	end

end

function tablecontentsdifferat (table1, table2, id)
	if (table1 == nil) ~= (table2 == nil) then return true end
	if table1 == nil then return false end
	return table1[id] ~= table2[id] and (not table1[id] or not table2[id] or table1[id][1] ~= table2[id][1] or table1[id][2] ~= table2[id][2])
end

-- a helper function to draw the info of the given frame
function renderframeinfo(records, id, tracknumber)

	if records == nil then return end

--	if tracknumber == -1 or tablecontentsdifferat(frameinforecords[tracknumber], frameinforecords[currentstatenumber], id) then
	if tracknumber == -1 or (tracknumber == 0 and tablecontentsdifferat(frameinforecords[tracknumber], frameinforecords[currentstatenumber], id)) then
		renderpasttrajectory(records, id, tracknumber)
	end

	info = records[id]

	if info == nil then return end

	if (currentstatenumber ~= tracknumber) and records.endid and (id > records.endid) then return end

	-- draw a box where the player is
	camx = -memory.readwordsigned(0xFFE600) + 128
	camy = memory.readwordsigned(0xFFE380) + 128
	x = info[1] - camx
	y = info[2] - camy
	y = y - 17.49999
	x = x - 0.49999
	if tracknumber == -1 then
		color = 0x7F7FFF00
	else
		color = 0xFF7FFF00
	end
	if tracknumber == -1 then
		outlinecolor = 0xFFFFFFFF
	elseif tracknumber == 0 then
		outlinecolor = 0x0000FFFF
	else
		outlinecolor = 0x7F7FFFFF
	end
	local textcolor = 0xFFFFFFFF
		
	gui.drawbox(x-12, y-15, x+11, y+16, color, outlinecolor)
	
	-- draw the savestate number somewhere in the box
	if tracknumber > 0 then
		x = x + (tracknumber%6) * 4 - 10
		y = y + (tracknumber/6) * 6 - 15
		message = string.format("%d", tracknumber)
		gui.text(x, y, message, textcolor)
	end

end

-- register a function to run after the emulation of every frame
gens.registerafter( function ()

	-- don't update if there's no player (this check could be better)
--[[	if memory.readword(0xFFA000) == 0 then return end
	if memory.readbytesigned(0xFFFCA8) == 0
	or memory.readbytesigned(0xFFFCA8) == -1
	or memory.readbytesigned(0xFFFCB2) == 0
	or memory.readbytesigned(0xFFFCB2) == -1
	then
		return
	end
]]
	-- store the new information for this frame
	id = getinstantid()

	if id == lastid then
		lastidvalid = false
	else
		lastid = id
		lastidvalid = true
	end

	if frameinforecords[currentstatenumber] == nil then
		frameinforecords[currentstatenumber] = {}
	end
	frameinforecords[currentstatenumber][id] = getperframeinfo()

	inp = input.get()
	if inp.Y then
		idfudge = idfudge + 1
	end
	if inp.U then
		idfudge = idfudge - 1
	end
	if inp.U and inp.Y then
		idfudge = 0
	end

end)

idfudge = 0

-- register a function to run when gens draws the screen
gui.register( function ()

	-- don't draw if there's no player (this check could be better)
	if memory.readword(0xFFA000) == 0 then return end
--[[	if memory.readbytesigned(0xFFFCA8) == 0
	or memory.readbytesigned(0xFFFCA8) == -1
	or memory.readbytesigned(0xFFFCB2) == 0
	or memory.readbytesigned(0xFFFCB2) == -1
	then
		return
	end
]]
	inp = input.get()
	if inp.capslock then return end

	if idfudge ~= 0 then
		gui.text(10, 24, string.format("fudge: %d", idfudge), 0xFFFF003F, 0x0000007F)
	end

	local id = getinstantid()

	if id == nil then return end

--[[
	-- check if all states are at the same place on this frame, if so, don't draw them
	local foundDisparity = false
	local foo = frameinforecords[currentstatenumber][id]
	if foo then
		for statenumber,records in pairs(frameinforecords) do
			local id1 = id + (statenumber~=0 and 0 or idfudge)
			local bar = records[id1]
			if bar then
				if foo[1] ~= bar[1] or foo[2] ~= bar[2] then
					foundDisparity = true
				end
			end
		end
]]
--		if foundDisparity then
			-- draw the information for this frame FOR ALL STATES
			for statenumber,records in pairs(frameinforecords) do
				if statenumber ~= currentstatenumber then
					renderframeinfo(records, id + (statenumber~=0 and 0 or idfudge), statenumber)
				end
			end
			renderframeinfo(frameinforecords[currentstatenumber], id, -1)
--		end
--	end

end)


function copytablepartial (table, evalfunc)
	local rv = {}
	for key,value in pairs(table) do
		if evalfunc(key,value) then
			rv[key]=value
		end
	end
	return rv
end


-- register a function that gets called when a numbered savestate is made.
-- the return value(s) becomes the savestate's scriptdata saved alongside it.
savestate.registersave( function (statenumber)

	if statenumber ~= currentstatenumber then
		local id = getinstantid()
		frameinforecords[statenumber] = copytablepartial(frameinforecords[currentstatenumber], function(key) return type(key) ~= 'number' or key-id<=3600 end)
		currentstatenumber = statenumber
	end

	frameinforecords[currentstatenumber].lastid = getinstantid()
	
	return frameinforecords[currentstatenumber]

end)

-- register a function that gets called when a numbered savestate is loaded
-- you can add arguments after the statenumber to get the savestate's scriptdata in them,
-- but I don't do that here since I don't always want to load the scriptdata from file
savestate.registerload( function (statenumber)

	currentstatenumber = statenumber

end)



-- register a function to run when the game boots up or resets
-- (this code runs once immediately if the game is already running)
gens.registerstart( function ()

	-- run some startup code: retrieve the ghost info stored in each savestate
	sound.clear() -- as a courtesy, clear the sound before operations like this that can take more than 0.5 seconds or so
	frameinforecords = {}
	for n = 0,10 do -- (1-9, 0, and also the hidden #10 if it's there)
		frameinforecords[n] = savestate.loadscriptdata(n)
		gens.wait()
	end

end)
