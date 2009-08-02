
K_Hp = 0xFF34FC;
Z_Sleep = 0xFF1566;
M_Sleep = 0xFF1576;
E_Sleep = 0xFF3512;

RNG = 0xFF2008;


 
local buttonmap = {[1]='up',[2]='down',[4]='left',[8]='right',[16]='B',[32]='C',[64]='A',[128]='start'} 
startmov = movie.framecount()	
CHECK = savestate.create();
savestate.save(CHECK);
K_Start = memory.readword(K_Hp);

for i = 0,20,1 do
	for j = 1,255,1 do
		savestate.load(CHECK);
		for x = 1,i,1 do
			control = 255;
			for bit,button in pairs(buttonmap) do
    			if AND(control,bit) ~= 0  then
      				joypad.set({[button]=true})      
	    		end    
  			end
			gens.emulateframeinvisible();
		end;
		gens.emulateframeinvisible();
		control = j
			for bit,button in pairs(buttonmap) do
    			if AND(control,bit) ~= 0  then
      				joypad.set({[button]=true})      
	    		end    
  			end;
			gens.emulateframeinvisible();
			gens.emulateframeinvisible();
			gens.emulateframeinvisible();
	while (memory.readbyte(Z_Sleep) == 3) do
				gens.emulateframeinvisible();
	end;
	if (memory.readbyte(Z_Sleep) == 0) and (memory.readbyte(E_Sleep) == 3) and (memory.readbyte(0xFF1716)==0) then
		while gens.lagged() do
				gens.emulateframeinvisible();
		end;
		K_Chng = K_Start - memory.readword(K_Hp);    
		if memory.readword(0xFF3354) == 5 then
			outs = string.format("ADV: %d Control: %d  Damage: %d RNG: %d",i,j,K_Chng,memory.readword(RNG));
			print(outs);
		end;
	end;
end;
end;