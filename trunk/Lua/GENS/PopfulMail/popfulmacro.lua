
-- script to enable some macros I'm using for Popful Mail (SegaCD)
-- 
-- current controls:
-- Control+Right = jump trick to accelerate right from a standstill faster than normal
-- Control+Left = jump trick to accelerate left from a standstill faster than normal
-- Control+Up = fast ladder climbing, but needs tweaking per ladder for frame-optimal results
-- B on the keyboard = jump and attack at the last moment that doesn't stop forward movement with the sword



startjumprightseq = {
	{B=false,right=true},
	{B=false,right=true},
	{B=true},
	{right=false},
	{right=false},
	{right=false},
	{right=false},
	{right=false},
}

startjumpleftseq = {
	{B=false,left=true},
	{B=false,left=true},
	{B=true},
	{left=false},
	{left=false},
	{left=false},
	{left=false},
	{left=false},
}

hitwhilelandseq = {
	{B=true,A=false},
	{B=true,A=false,left=false,right=false},
	{B=true,A=false,left=false,right=false},
	{B=true,A=false,left=false,right=false},
	{B=true,A=false,left=false,right=false},
	{B=true,A=false,left=false,right=false},
	{B=true,A=false,left=false,right=false},
	{B=true,A=false,left=false,right=false},
	{B=true,A=false,left=false,right=false},
	{B=true,A=false,left=false,right=false},
	{B=true,A=false,left=false,right=false},
	{B=true,A=false,left=false,right=false},
	{B=true,A=false,left=false,right=false},
	{B=true,A=false,left=false,right=false},
	{B=true,A=false,left=false,right=false},
	{B=true,A=false,left=false,right=false},
	{B=true,A=false,left=false,right=false},
	{B=true,A=false,left=false,right=false},
	{B=true,A=false,left=false,right=false},
	{B=false,A=false,left=false,right=false},
	{B=false,A=false,left=false,right=false},
	{B=false,A=false,left=false,right=false},
	{B=false,A=false,left=false,right=false},
	{B=false,A=false,left=false,right=false},
	{B=false,A=true},
}

lastbuttons = {}
function doframewith (buttons)
	samebuttons = (buttons.B == lastbuttons.B) and
	              (buttons.A == lastbuttons.A) and
	              (buttons.right == lastbuttons.right) and
	              (buttons.left == lastbuttons.left) and
	              (buttons.up == lastbuttons.up)
	repeat
		local lagc = gens.lagcount()
		joypad.set(buttons)
		gens.emulateframe()
		gens.wait()
--		gens.frameadvance()
	until samebuttons or (lagc == gens.lagcount())
	lastbuttons = buttons
end

function domacro (buttonseq)
	lastbuttons = {}
	abortmacro = false
	for i=1,#buttonseq do
		if abortmacro then return end
		doframewith(buttonseq[i])
	end
end

function ladderjump (jumplens, flip)
	lastbuttons = {}
	abortmacro = false
	local alt = AND(memory.readbyte(0xFFA031),1) == 0
	if flip then alt = not alt end
	for i=1,#jumplens do
		for j=1,jumplens[i] do
			if abortmacro then return end
			if j == 1
			--and ((i%2)==0 or i == 2)
			then
				alt = not alt
			end
			if j == 1 or i == #jumplens then
				if alt then
					doframewith({B=true,left=true})
				else
					doframewith({B=true,right=true})
				end
			else
				doframewith({B=true})
			end
		end
		if i ~= #jumplens then
			if abortmacro then return end
			doframewith({up=true,left=false,right=false,B=false})
		end
	end
end

savestate.registerload( function(statenumber)
	if statenumber then
		abortmacro = true
	end
end )

tempslot = savestate.create()

depth = 0
gens.registerbefore( function ()

	if depth > 0 then return end
	depth = depth + 1

	local inp = input.get()

	-- TODO: fast dialog advancement macro, not working yet
--[[
	if memory.readbytesigned(0xFFFCA8) == 0
	or memory.readbytesigned(0xFFFCA8) == -1
	or memory.readbytesigned(0xFFFCB2) == 0
	or memory.readbytesigned(0xFFFCB2) == -1
	or memory.readword(0xFFA000) == 0
	then
--		if memory.readbyte(0xFFFC2C) ~= 0 or memory.readbyte(0xFFF416) == 0 then
--			joypad.set({A=true})
--		end
		if inp.capslock then
			savestate.save(tempslot)
			local foo = memory.readbyte(0x224F2D)
			joypad.set({A=true})
			gens.emulateframeinvisible()
			joypad.set({A=true})
			gens.emulateframeinvisible()
			joypad.set({A=true})
			gens.emulateframeinvisible()
			local foo2 = memory.readbyte(0x224F2D)
			savestate.load(tempslot)
			if foo ~= foo2 then
				joypad.set({A=true})
				--gens.emulateframe()
				--gens.frameadvance()
			end
		--depth = depth - 1
		--return
		end
	end
]]

	gens.speedmode("nothrottle")
		
	peek = joypad.peekdown()

	if inp.control then
		xvel = memory.readwordsigned(0xFFA02D)
		yvel = memory.readwordsigned(0xFFA029)
		if peek.right and xvel <= 512 and yvel == 0 then
			domacro(startjumprightseq)
		end
		if peek.left and xvel >= -512 and yvel == 0 then
			domacro(startjumpleftseq)
		end
		if peek.up and not peek.left and not peek.right then
			--ladderjump({4,4,4,4,3})
			--ladderjump({5,5,5,5})
			--ladderjump({7,7,8})
			--ladderjump({5,5,4,8},1)
			--ladderjump({7,7,9})
			--ladderjump({7,7,6,6,5,5,5,5,3})
			--ladderjump({5,5,5,4,7})

			--ladderjump({6,6,6})
			--ladderjump({5,5,5,4,3})
			--ladderjump({,2})
			--ladderjump({5,5,3,7,2})
			--ladderjump({4,7,4,5,3})

			--ladderjump({6,7,7})
			--ladderjump({4,5,6,3}, 1)
			--ladderjump({5,5,5,5,5,5,48,5,5,5})
			--ladderjump({7,7,7,7,7,42,7,7,4}, 1)
			
			--ladderjump({5,5,5,6,3})
			--ladderjump({5,5,5,5,5,5,5,5,3})
			--ladderjump({5,5,4,3},1)
			--memory.writeword(0xFFA029, 2048)
			ladderjump({5,5,5})
		end
--[[
		if inp.D then
			local f = io.open("C:\\temp.txt", "w")
			local addr = 0xFFA980
			for i=0,(0x80-1) do
				f:write(string.format("%02X ", memory.readbyte(addr+i)))
			end
			f:close()
		end
]]
	end
	if inp.B then
		domacro(hitwhilelandseq)
	end
--[[
	if inp.capslock then
			local timer = memory.readbyte(0xffa880 + 0x39)
			if timer == 56 then
				joypad.set({A=true})
			end
	end
]]
	depth = depth - 1

	gens.speedmode("normal")

end)

--input.registerhotkey(1, function()
--	domacro(hitwhilelandseq)
--end)
