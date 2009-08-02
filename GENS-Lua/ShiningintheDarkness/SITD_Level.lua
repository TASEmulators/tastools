Combo = io.open("C:\\SitD_Level.txt", "w");
 
CHECK = savestate.create();
At=0xFF16C1
Df = 0xFF1501
Ag = 0xFF1513
Hp = 0xFF16B5
Luck = 0xFF1519
IQ = 0xFF16D3

Mp = 0xFF16BF;
RNG = 0xFF2008;


 savestate.save(CHECK);
 for i = 0,1024,1 do
	savestate.load(CHECK);
	
	memory.writeword(RNG,i);
	gens.emulateframefast();
	gens.emulateframefast();
	
 	outs = string.format("%d: At-%d  Df-%d  Ag-%d  Hp-%d  Luck-%d  IQ-%d  MP-%d\n",i,memory.readbyte(At),memory.readbyte(Df),memory.readbyte(Ag), memory.readbyte(Hp),memory.readbyte(Luck),memory.readbyte(IQ),memory.readbyte(Mp)); 		
    Combo:write(outs);
end;