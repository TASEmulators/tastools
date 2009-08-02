
-- enemy and weapon hitbox and stats display script
-- for Popful Mail (SegaCD)

-- the displayed hitboxes are VERY accurate.

-- NOTE: currently, to make it perfectly accurate,
-- you must choose "Graphics > Latency Compensation > 1" from the menu in Gens.


msgYoff = 50
function message(msg, bright)
	if bright then
		if bright == 0 then
			gui.text(10, msgYoff, msg, 0xFFFFFF3F, 0x0000007F)
		else
			gui.text(10, msgYoff, msg, 0xFFFF00FF, 0xFF0000FF)
		end
	else
		gui.text(10, msgYoff, msg, 0xFFFF007F, 0x000000FF)
	end
	msgYoff = msgYoff + 8
end

gens.registerbefore( function()
	lastleveltime = memory.readlong(0xFFEAFE)
	inactive = nil
end)
gens.registerafter( function()
	leveltime = memory.readlong(0xFFEAFE)
	inactive = (leveltime == lastleveltime)
	lastleveltime = leveltime
end)


-- register a function to run when the screen gets updated
gui.register( function ()

--memory.writelong(0xFFA028, -200000)

	local xvel = memory.readlongsigned(0xFFA02C) / 256
	local yvel = memory.readlongsigned(0xFFA028) / 256
	local xpos = memory.readlong(0xFFA024) / 256
	local ypos = memory.readlong(0xFFA020) / 256
	local xcam = -memory.readwordsigned(0xFFE600) + 128
	local ycam = memory.readwordsigned(0xFFE380) + 128
	local ingameframes = memory.readlong(0xFFEAFE)
	local lostframes = gens.framecount() - ingameframes
	local curhp = memory.readword(0xFFA056)
	local gold = memory.readword(0xFFEBF2)
	local atk = memory.readword(0xFFA058)
	local defense = memory.readbyte(0xFFA05B)
	local arms = memory.readbyte(0xFFEC1B)
	local curchar = memory.readword(0xFFEBF0)
	local worldpc = memory.readlong(0x2001A2) -- when the PC is at 21:XXXX it's probably relative to this worldpc value


	hasboomerang = AND(memory.readbyte(0xFFEBCC), 4) ~= 0
	hasfirebreath = AND(memory.readbyte(0xFFEBEC), 8) ~= 0
	desiredgold = 0
	if not hasboomerang then desiredgold = desiredgold + 2000 end
	if not hasfirebreath then desiredgold = desiredgold + 3000 end
	if desiredgold < gold then desiredgold = gold end
	value = gold/1 + curhp -- estimate 1 frame per HP lost and 1 frame per gold missing
	basevalue = desiredgold/1 + 100
	debtframes = basevalue - value
	totaltime = ingameframes+lostframes+debtframes

	local onscreengoldtimeestimate = 0
	local onscreengoldamount = 0
	

	inp = input.get()

	for layer=0,2 do
	for i=0,63 do
		local addr = 0xFFA000+(i*0x80)
		local hp = memory.readword(addr+0x56)
		local type = memory.readword(addr)
		local armor = memory.readbyte(addr+0x5B)

		local hittable = AND(memory.readbyte(addr + 0x53), 0x4) ~= 0 and AND(memory.readbyte(addr + 0x53), 0x18) ~= 0x18 and hp>0
		local hitting = AND(memory.readbyte(addr + 0x51), 0x30) ~= 0
		local unfriendly = AND(memory.readbyte(addr + 0x51), 0x28) ~= 0
		local recovering = memory.readword(addr + 0x38) ~= 0

--		if (type ~= 0) and (hittable) then
--		if (type ~= 0) and (hittable or hitting) then
		if (type ~= 0) then
			local xpos = memory.readword(addr+0x24)
			local ypos = memory.readword(addr+0x20)
			--local xpos = memory.readlong(addr+0x24) / 65536
			--local ypos = memory.readlong(addr+0x20) / 65536

			local xposrel = xpos - xcam
			local yposrel = ypos - ycam

--			message( string.format("pos: %d,%d   hp: %d   type: %X", xpos,ypos, hp, type) )
--			message( string.format("pos: %d,%d   hp: %d", xpos,ypos, hp) )
--			message( string.format("pos: %d,%d   hp: %d   unk: %X", xpos,ypos, hp, unk) )

			if xposrel >= -16 and yposrel >= -32 and xposrel < 320+32 and yposrel < 224+16 then

				-- hitbox
				if not inp.capslock then
					local function drawhitbox(addr, addroff, color, fillcolor)

						local coladdr = memory.readlong(addr + addroff)
						local miny = yposrel + memory.readwordsigned(coladdr)
						local maxy = miny + memory.readwordsigned(coladdr+2)
						local minx = xposrel + memory.readwordsigned(coladdr+4)
						local maxx = minx + memory.readwordsigned(coladdr+6)
						
						if memory.readwordsigned(coladdr+2) ~= -768 and memory.readwordsigned(coladdr+6) ~= 1070 then
							gui.box(minx,miny,maxx,maxy, fillcolor, color)
						end
						
					end
										
					local fillcolor = 0
					if addr == AND(0xFFFFFF, memory.readlong(0xFFA000 + 0x4C)) then
						fillcolor = 0xFFFFFF3F
						fillcolor = 0xFF00005F
					end
					
					if layer == 0 then
						if not hittable and not hitting then
							local color = 0x7F7F7F7F
							if (memory.readbyte(addr+0x51) == 8) then
								if (memory.readword(memory.readlong(addr+0x1C)+8) ~= 0) then
									color = 0xFFFF00FF
								else
									color = 0x9FFF00FF
								end
							end
							drawhitbox(addr, 0x44, color, fillcolor)
						end
					end

					if layer == 1 then
						if hitting then
							if (not hittable) or (memory.readlong(addr + 0x40)  ~= memory.readlong(addr + 0x44)) then
								local color = unfriendly and 0xFF7F00FF or 0x00FF00FF
								if recovering then
									color = 0x7F00FFFF
								end
								drawhitbox(addr, 0x40, color, fillcolor)
							end
						end
					end

					if layer == 2 then
						if hittable then
							local color = unfriendly and 0xFF0000FF or 0x00FFFFFF
							if unfriendly and not hitting then
								color = 0xFF00FFFF
							end
							if recovering then
								if memory.readword(addr + 0x38) > 60
								or memory.readword(addr + 0x38) <= 24
								then
									color = 0x000000FF
								else
									color = 0xFFFFFFFF
								end
							end
							drawhitbox(addr, 0x44, color, fillcolor)
						end
					end
					
				end

				if layer == 2 then
					local yoff = 0

					local function getdrawpos(addr, addroff)

						local coladdr = memory.readlong(addr + addroff)
						local miny = yposrel + memory.readwordsigned(coladdr)
						local maxy = miny + memory.readwordsigned(coladdr+2)
						local minx = xposrel + memory.readwordsigned(coladdr+4)
						local maxx = minx + memory.readwordsigned(coladdr+6)
						
						return (minx+maxx)/2-2.5, maxy+1
						
					end

					local atkdrawx
					local atkdrawy
					local defdrawx
					local defdrawy
					atkdrawx,atkdrawy = getdrawpos(addr,0x40)
					defdrawx,defdrawy = getdrawpos(addr,0x44)
					
					if hittable then
						local hits
						if unfriendly then
							hits = math.ceil(hp / math.max(1, atk - armor))
						else
							hits = hp
						end

						gui.text(defdrawx, defdrawy+yoff,
							string.format("%d", hits),
							0x00FFFF7F, 0x000000FF)
						yoff = yoff + 7
					end

					if hitting and ((not recovering) or (not unfriendly)) then
						local enatk = memory.readword(addr+0x58)
						if unfriendly then
							enatk = enatk - defense
						else
							enatk = atk==0 and 0 or math.ceil(enatk / atk) -- because player's atk is already factored out of enemy HP display
						end

						gui.text(atkdrawx, atkdrawy+yoff,
							string.format("%d", enatk),
							0xFF00007F, 0x000000FF)
						yoff = yoff + 7
					end

					if recovering then
						local timer = memory.readword(addr + 0x38)
						gui.text(defdrawx, defdrawy+yoff,
							string.format("%d", timer),
							0x7F00FF7F, 0x000000FF)
						yoff = yoff + 7
					end

					if (unfriendly and hittable) or (not hitting and memory.readbyte(addr+0x51) == 8) then -- show gold amount
	--				if memory.readbyte(addr+0x51) == 8 then -- show gold amount
	--				if memory.readword(addr+0x52) ~= 4 then
						amount = memory.readword(memory.readlong(addr+0x1C)+8)
						if amount ~= 0 then
							gui.text(defdrawx, defdrawy+yoff,
								string.format("%d", amount ),
								0xFFFF007F, 0x000000FF)
							yoff = yoff + 7
						end
					end

	--[[
					--if memory.readbyte(addr+0x51) == 8 then
					do
						gui.text(defdrawx, defdrawy+yoff,
	--						string.format("%d", memory.readword(addr+0x14) ),
							string.format("%08X", memory.readlong(addr+4*input.get().ymouse) ),
							0xFFFF007F, 0x000000FF)
						yoff = yoff + 7
					end
	]]
	--[[
					do
						gui.text(defdrawx, defdrawy+yoff,
							string.format("%06X", addr ),
							0xFFFFFF7F, 0x000000FF)
						yoff = yoff + 7
					end
	]]

	--[[
					do
						gui.text(defdrawx, defdrawy+yoff,
							string.format("%06X", memory.readlong(addr + 0x4) ),
							0xFFFF7F7F, 0x000000FF)
						yoff = yoff + 7
					end
	]]
--	--[[
					do
						gui.text(defdrawx, defdrawy+yoff,
							string.format("%02X", memory.readbyte(addr + 0x53) ),
							0xFFFF7F7F, 0x000000FF)
						yoff = yoff + 7
					end
--	]]
				
					-- gold speed estimation
					if xposrel >= -16+80 and yposrel >= -32+20 and xposrel < 320+32-80 and yposrel < 224+16-80 then
						if (unfriendly and hittable) or (not hitting and memory.readbyte(addr+0x51) == 8) then -- show gold amount
							amount = memory.readword(memory.readlong(addr+0x1C)+8)
							if amount ~= 0 then

								onscreengoldamount = onscreengoldamount + amount

								local hits = 0
								if hittable then
									hits = math.ceil(hp / math.max(1, atk - armor))
								end
								
								local hittime = (hits * 40)
								local goldappeartime = 36
								local goldfalltime = math.ceil(math.max(0, (memory.readword(0xFFA020) - ypos) / 7))
								local fudgetime = -22

								onscreengoldtimeestimate = math.max(onscreengoldtimeestimate, hittime + goldappeartime + goldfalltime + fudgetime)

							end
						end
					end
				
				end
			end
		end
	end
	end

	msgYoff = 32

	message( string.format("vel: %g, %g", xvel, yvel), (xvel == 0 and yvel ~= 0) or (yvel == 0 and (math.abs(xvel)==696 or math.abs(xvel)==760)) )
	message( string.format("pos: %g, %g", xpos, ypos) )
--	message( string.format("time: %d (%d + %d + %d)", totaltime, ingameframes, lostframes, debtframes) )

--	message( string.format(" leveltime: %d (%d:%02d:%02d)", ingameframes, ingameframes/60/60, ingameframes/60%60, ingameframes%60) )
--	message( string.format(" othertime: %d (%d:%02d:%02d)", lostframes, lostframes/60/60, lostframes/60%60, lostframes%60) )
--	message( string.format("actualtime: %d (%d:%02d:%02d)", (ingameframes+lostframes), (ingameframes+lostframes)/60/60, (ingameframes+lostframes)/60%60, (ingameframes+lostframes)%60) )
--	message( string.format("predictime: %d (%d:%02d:%02d)", totaltime, totaltime/60/60, totaltime/60%60, totaltime%60) )

	local ingameframes = memory.readbyte(0xFFEBA4)
	local ingameseconds = memory.readbyte(0xFFEBA5)
	local ingameminutes = memory.readbyte(0xFFEBA6)
	local ingamehours = memory.readbyte(0xFFEBA7)
	local ingamehundredhours = memory.readbyte(0xFFEBA8)
	local ingametotalframes = ingameframes + 60*(ingameseconds + 60*(ingameminutes + 60*(ingamehours + 100*ingamehundredhours)))
	local actualframes = gens.framecount()
	local lostframes = actualframes - ingametotalframes
--	message( string.format("   losttime: %d (%d:%02d:%02d:%02d)", lostframes, lostframes/60/60/60, lostframes/60/60%60, lostframes/60%60, lostframes%60), 0 )
--	message( string.format(" ingametime: %d (%d:%02d:%02d:%02d)", ingametotalframes, ingamehundredhours*100+ingamehours, ingameminutes, ingameseconds, ingameframes), 0 )
--	message( string.format(" actualtime: %d (%d:%02d:%02d:%02d)", actualframes, actualframes/60/60/60, actualframes/60/60%60, actualframes/60%60, actualframes%60), 0 )
--	if actualframes ~= totaltime then
--		message( string.format("predicttime: %d (%d:%02d:%02d:%02d)", totaltime, totaltime/60/60/60, totaltime/60/60%60, totaltime/60%60, totaltime%60), 0 )
--	end
		message( string.format("time: %d (%d:%02d:%02d)", totaltime, totaltime/60/60, totaltime/60%60, totaltime%60) )


--	message( string.format("debt:  %d", basevalue - value) )
--	message( string.format("gold:  %d", gold) )
--	message( string.format("item:  %d, %d", hasboomerang and 1 or 0, hasfirebreath and 1 or 0) )
--	message( string.format("cam: %d, %d", xcam, ycam) )
--	message( string.format("char: %X", curchar) )
--	message( string.format("hex: %X", (gens.framecount()-1) * 3 + 64) )

--	message( string.format("%X", memory.readword(worldpc+0x2380+0x1C + (curchar+1)*2) / 1))

		if movie.framecount() < 750 then
			message( movie.name() )
		end

--	leveltime = memory.readlong(0xFFEAFE)
	if inactive then
		message( "INACTIVE" )
	elseif lastleveltime and math.abs(leveltime - lastleveltime) > 1 then
		message( leveltime - lastleveltime , 1)
	end
--	lastleveltime = leveltime

--[[	
	if memory.readbytesigned(0xFFFCA8) == 0
	or memory.readbytesigned(0xFFFCA8) == -1
	or memory.readbytesigned(0xFFFCB2) == 0
	or memory.readbytesigned(0xFFFCB2) == -1
	or memory.readword(0xFFA000) == 0
	then
		message( "INACTIVE" )
	end
]]
	gui.text(138,218, arms, 0xFFFFFFCF, 0x00003FCF)
	gui.text(260,218, gold, 0xFFFF00CF, 0x3F1F00CF)
	--inp = input.get()
	--gui.text(inp.xmouse,inp.ymouse, {x=inp.xmouse,y=inp.ymouse}, 0xFFFFFFCF, 0x00003FCF)

	if onscreengoldamount > 0 then
		gui.text(20,218,
			string.format("%g (%d gold, %d frames)",
				onscreengoldamount / onscreengoldtimeestimate,
				onscreengoldamount, onscreengoldtimeestimate
			), 0xFFFF00CF, 0x3F1F00CF
		)
	end


end)



