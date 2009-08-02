
x = io.open("C:\\SITD_Target.txt", "w");

--         HP  At Df   Ag,  Luck,  IQ    Mp
Targets = {22, -1,  9,  17 ,  13,    17,  17}
CHECK = savestate.create();
At=0xFF16C1
Df = 0xFF1501
Ag = 0xFF1513
Hp = 0xFF16B5
Luck = 0xFF1519
IQ = 0xFF16D3

At=0xFF16C1+4
Df = 0xFF1501+4
Ag = 0xFF1513+4
Hp = 0xFF16B5+4
Luck = 0xFF1519+4
IQ = 0xFF16D3+4

Mp = 0xFF16BF;
RNG = 0xFF2008;
c = 0;
outs = "";
 savestate.save(CHECK);
 for i = 0,65536,1 do
    j = math.fmod(i+65536+224,65536);
	savestate.load(CHECK);
	joypad.set(1,{C=1});  
	memory.writeword(RNG,j);
	gens.emulateframefast();
	gens.emulateframefast();
	if ((memory.readbyte(Hp)==Targets[1]) or (Targets[1] == -1)) and ((memory.readbyte(At)==Targets[2]) or (Targets[2] == -1))  and ((memory.readbyte(Df)==Targets[3]) or (Targets[3] == -1)) then
	  if ((memory.readbyte(Ag)==Targets[4]) or (Targets[4] == -1)) and ((memory.readbyte(Luck)==Targets[5]) or (Targets[5] == -1)) and ((memory.readbyte(IQ)==Targets[6]) or (Targets[6] == -1)) and ((memory.readbyte(Mp)==Targets[7]) or (Targets[7] == -1)) then	
          outs = string.format("%d   %d\n",i,j);
          x:write(outs);
          end;
          end;
end;





x:close();