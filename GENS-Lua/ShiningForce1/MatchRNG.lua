 
Combo = io.open("C:\\Bot.txt", "w");
 
CHECK = savestate.create();


RNG = 0xFF0EC2
gens.speedmode("maximum")
 savestate.save(CHECK);
cont1 = 1;
cont2 = 1;
 pass = 0
 while pass == 0 do
 


  
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
		outs = string.format("%d, %d:RNG - %d\n",cont1, cont2, memory.readword(RNG))
    	Combo:write(outs);
	if memory.readword(RNG) == 50184 then
		outs = string.format("%d, %d: SUCCESS!\n",cont1, cont2)
    	Combo:write(outs);
    	pass = 1;
    	end
    	cont2 = cont2 +1;
    	if cont2==256 then
    	cont2 = 1;
    	cont1 = cont1+1;
    	end
    	if cont1==256 then
    	outs = string.format("FAIL!")
    	Combo:write(outs);
    	break
    	end
    	
end
Combo:close();