-- feos, 2012
-- Autohold toggles allow to avoid pressing all necessary buttons while frame advancing
-- Specify your toggles for each button, add new ones for different platforms
-- "start" and "select" may be deleted to save on buttons ;)

lastkeys, keys = input.get(), input.get()

switches1 = {
	-- Pad 1
	A      = {toggle="Q", on=false, val={A=1}     },
	B      = {toggle="W", on=false, val={B=1}     },
	left   = {toggle="E", on=false, val={left=1}  },
	right  = {toggle="R", on=false, val={right=1} },
	up     = {toggle="T", on=false, val={up=1}    },
	down   = {toggle="Y", on=false, val={down=1}  },
	start  = {toggle="U", on=false, val={start=1} },
	select = {toggle="I", on=false, val={select=1}}
}

switches2 = {
	-- Pad 2
	A      = {toggle="A", on=false, val={A=1}     },
	B      = {toggle="S", on=false, val={B=1}     },
	left   = {toggle="D", on=false, val={left=1}  },
	right  = {toggle="F", on=false, val={right=1} },
	up     = {toggle="G", on=false, val={up=1}    },
	down   = {toggle="H", on=false, val={down=1}  },
	start  = {toggle="J", on=false, val={start=1} },
	select = {toggle="K", on=false, val={select=1}}
}

function InputToggles()
	keys = input.get()
	
	for name, button in pairs(switches1) do
		if keys[button.toggle] and not lastkeys[button.toggle] then button.on = not button.on end	
		if button.on then joypad.set(1, button.val) end
	end
	
	for name, button in pairs(switches2) do
		if keys[button.toggle] and not lastkeys[button.toggle] then button.on = not button.on end	
		if button.on then joypad.set(2, button.val) end
	end
	
	lastkeys = keys
end

emu.registerbefore(InputToggles);