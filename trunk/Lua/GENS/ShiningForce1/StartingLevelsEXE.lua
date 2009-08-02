 
cont1 = 231
cont2 = 1



	

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

