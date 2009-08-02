Combo = io.open("C:\\SitD.txt", "w");
 
CHECK = savestate.create();

RNG = 0xFF2008;
Dmg = 0xFF3A54;
Z_Hp = 	0xFF16A9;
K_Hp = 0xFF34FC;
P_Hp = 0xFF16AC;



 savestate.save(CHECK);
--SitD Bot
startmov = movie.framecount()

for i = 0,65536,1 do
	if math.fmod(i*13 + 7,65536) < 13000 then
		savestate.load(CHECK);
		memory.writeword(RNG,i);
		while gens.lagged() do
			gui.text(1,50,string.format('Searching... %d',i));
			gens.emulateframefast();
		end;
	   mov =  movie.framecount() - startmov;
	   if memory.readword(Z_Hp) == 0 and memory.readword(P_Hp) == 0 then
			print(string.format('RNG: %d, FC: %d',i, mov));	
		end;   	
	end;
end;