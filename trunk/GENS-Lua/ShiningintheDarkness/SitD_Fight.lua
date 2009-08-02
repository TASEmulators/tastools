Combo = io.open("C:\\SitD.txt", "w");
 
CHECK = savestate.create();

RNG = 0xFF2008;
Dmg = 0xFF3A54;
Z_Hp = 	0xFF16A9;
K_Hp = 0xFF34FC;




 savestate.save(CHECK);
--SitD Bot

Z_Start = memory.readbyte(Z_Hp);
K_Start = memory.readword(K_Hp);

for i = 4000,65536,1 do
	if math.fmod(i*13 + 7,65536) < 12000 then
		savestate.load(CHECK);
		memory.writeword(RNG,i);
		for z = 1,20,1 do
			gui.text(1,50,string.format('Searching... %d',i));
			gens.emulateframefast();
		end;
	
	   if memory.readword(0xFF334A) == 0 and memory.readbyte(0xFF125A) == 1 and memory.readbyte(0xFF1259) == 99 then
		while(gens.lagged()) do
			gui.text(1,50,string.format('RNG: %d',i));	
	         gens.frameadvance();
	      end;
		end;   	
	end;
end;