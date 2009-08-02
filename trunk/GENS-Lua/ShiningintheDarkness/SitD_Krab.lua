
CHECK = savestate.create();
RNG = 0xFF2008;
Dmg = 0xFF3A54;
Z_Hp = 	0xFF16A9;
K_Hp = 0xFF34FC;
M_Hp = 0x16AA;
P_Hp = 0x16AC;

gens.speedmode("nothrottle")
 savestate.save(CHECK);
--SitD Bot
Max = 999;
last = 0;
j = 0;
Z_Start = memory.readbyte(Z_Hp);
K_Start = memory.readword(K_Hp);
P_Start = memory.readword(P_Hp);
savestate.save(CHECK);
 for i = 0,65536,1 do
	savestate.load(CHECK);	
	memory.writeword(RNG,i);
	gui.text(10,50,string.format('%d %d',j,Max));
	gens.frameadvance()	
	K_Chng = memory.readword(K_Hp);	
	if K_Chng < Max then
		Max = K_Chng;
	    j = i;
	 end;	
end;