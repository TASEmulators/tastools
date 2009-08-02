 
Combo = io.open("C:\\Bot.txt", "w");
 
CHECK = savestate.create();

At=0xFF9D66
Df = 0xFF9D67
Ag = 0xFF9D68
Hp = 0xFF9D6D
gens.speedmode("turbo")
 savestate.save(CHECK);
 
for cont1 = 1,255,1 do
 for cont2 = 1,255,1 do

  
	savestate.load(CHECK);
	
	
    ctemp = cont2;    
    if math.fmod(ctemp,2)==1 then
		joypad.set(1,{up=true});
	end;
	ctemp = math.floor(ctemp/2);
   if math.fmod(ctemp,2)==1 then
		joypad.set(1,{down=true});
	end;
	ctemp = math.floor(ctemp/2);
	if math.fmod(ctemp,2)==1 then
		joypad.set(1,{left=true});
	end;
	ctemp = math.floor(ctemp/2);
	if math.fmod(ctemp,2)==1 then
		joypad.set(1,{right=true});
	end;
	ctemp = math.floor(ctemp/2);
	if math.fmod(ctemp,2)==1 then
		joypad.set(1,{A=true});
	end;
	ctemp = math.floor(ctemp/2);
	if math.fmod(ctemp,2)==1 then
		joypad.set(1,{B=true});
	end;
	ctemp = math.floor(ctemp/2);
	if math.fmod(ctemp,2)==1 then
		joypad.set(1,{C=true});
	end;
	ctemp = math.floor(ctemp/2);
	if math.fmod(ctemp,2)==1 then
		joypad.set(1,{start=true});
	end;
	gens.frameadvance()
	
   ctemp = cont1;    
    if math.fmod(ctemp,2)==1 then
		joypad.set(1,{up=true});
	end;
	ctemp = math.floor(ctemp/2);
   if math.fmod(ctemp,2)==1 then
		joypad.set(1,{down=true});
	end;
	ctemp = math.floor(ctemp/2);
	if math.fmod(ctemp,2)==1 then
		joypad.set(1,{left=true});
	end;
	ctemp = math.floor(ctemp/2);
	if math.fmod(ctemp,2)==1 then
		joypad.set(1,{right=true});
	end;
	ctemp = math.floor(ctemp/2);
	if math.fmod(ctemp,2)==1 then
		joypad.set(1,{A=true});
	end;
	ctemp = math.floor(ctemp/2);
	if math.fmod(ctemp,2)==1 then
		joypad.set(1,{B=true});
	end;
	ctemp = math.floor(ctemp/2);
	if math.fmod(ctemp,2)==1 then
		joypad.set(1,{C=true});
	end;
	ctemp = math.floor(ctemp/2);
	if math.fmod(ctemp,2)==1 then
		joypad.set(1,{start=true});
	end;
	gens.frameadvance()
	gens.frameadvance()	
	gens.frameadvance()
	gens.frameadvance()	
	gens.frameadvance()
	gens.frameadvance()	
	gens.frameadvance()
 	outs = string.format("%d, %d: At-%d  Df-%d  Ag-%d  Hp-%d\n",cont1,cont2,memory.readbyte(At),memory.readbyte(Df),memory.readbyte(Ag), memory.readbyte(Hp)); 		
    Combo:write(outs);
end
end

Combo:close();