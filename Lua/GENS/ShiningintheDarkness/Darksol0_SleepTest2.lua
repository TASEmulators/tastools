
K_Hp = 0xFF34FC;
Z_Sleep = 0xFF1566;
P_Sleep = 0xFF1586;

E_Sleep = 0xFF3512;
RNG = 0xFF2008;


RNGc = memory.readword(RNG);
local buttonmap = {[1]='up',[2]='down',[4]='left',[8]='right',[16]='B',[32]='C',[64]='A',[128]='start'} 
startmov = movie.framecount()	
CHECK = savestate.create();
savestate.save(CHECK);
K_Start = memory.readword(K_Hp);
for Cnt= 1,5000,1 do
		savestate.load(CHECK);
		memory.writeword(RNG,RNGc+Cnt);
		gens.emulateframeinvisible();
		joypad.set(1,{C=1});   
	while (memory.readbyte(P_Sleep) == 3) do
				gens.emulateframeinvisible();
	end;
	if (memory.readbyte(P_Sleep) == 0) and memory.readbyte(E_Sleep)== 2 then
		while gens.lagged() do
				gens.emulateframeinvisible();
		end;
		K_Chng = K_Start - memory.readword(K_Hp);    
		if memory.readword(0xFF3354) == 5 then
			outs = string.format("Cnt: %d  Damage: %d",RNGc+Cnt,K_Chng);
			print(outs);
		end;
	end;
end;
