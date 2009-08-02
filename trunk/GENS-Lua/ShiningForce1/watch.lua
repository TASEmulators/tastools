
CHECK = savestate.create();

gui.register( function ()

if memory.readbyte(0xFFF009)==0 then
savestate.save(CHECK);
if (memory.readbyte(0xFF0FFD) ==  16) then
while (memory.readbyte(0xFF0FFD) ==  16) do
	  gens.emulateframefast();	
	 end;
else
   gens.emulateframeinvisible();	
   gens.emulateframeinvisible();	 
if (memory.readbyte(0xFF0002) < 12) then
 for i = 1,85,1 do	
   gens.emulateframeinvisible();	
end;
	if (memory.readbytesigned(0XFF00F7) == -84) then
	 	savestate.load(CHECK);
	else	
		while (memory.readbytesigned(0XFF00F7) ~= -84) do
	   	gens.emulateframeinvisible();
		end;
		for i = 1,85,1 do
		gens.emulateframeinvisible();
		end;
	end;
else
savestate.load(CHECK);
end;
end;
end;										
end)
	