startmov = movie.framecount()	
K_Hp = 0xFF34FC;
Z_Sleep = 0xFF1566;
P_Sleep = 0xFF1586;
P_Hp = 0xFF16AC;
E_Sleep = 0xFF3512;
RNG = 0xFF2008;

Sleeper =  io.open("C:\\SitD_Darksol_Sleep3_2.txt", "w");
RNGc = memory.readword(RNG);
local buttonmap = {[1]='up',[2]='down',[4]='left',[8]='right',[16]='B',[32]='C',[64]='A',[128]='start'} 
startmov = movie.framecount()	
CHECK = savestate.create();
savestate.save(CHECK);
K_Start = memory.readword(K_Hp)+100;
for Cnt= 29120,65535,1 do
		savestate.load(CHECK);
		memory.writeword(RNG,Cnt);
		gens.emulateframeinvisible();	 
		joypad.set(1,{right=1});   
		gens.emulateframeinvisible();	 
		gens.emulateframeinvisible();	  
			while(gens.lagged()) do
  	  			joypad.set(1,{C=1});    	  		   	  		   
	      		gens.emulateframeinvisible();
	    	
	  		end
	  		gens.emulateframeinvisible();
  	  		gens.emulateframeinvisible();  	  		
  	  		while(gens.lagged()) do
  	  			joypad.set(1,{up=1});    	  		   	  		   
	      		gens.emulateframeinvisible();

	  		end	  		
  	  		 gens.emulateframeinvisible()
  	  		  while(gens.lagged()) do
  	  			joypad.set(1,{C=1});    	  		   	  		   
	      		gens.emulateframeinvisible();
	    	
	  		end
	  		gens.emulateframeinvisible();
	while (memory.readbyte(E_Sleep) == 2) do
				gens.emulateframeinvisible();
	end;
	if memory.readbyte(E_Sleep)== 1 then
		while gens.lagged() do
				gens.emulateframeinvisible();
		end;
		K_Chng = K_Start - memory.readword(K_Hp);    
		 mov =  movie.framecount() - startmov;
		if K_Chng > 200 and memory.readbyte(E_Sleep)== 1 and memory.readword(0xFF3354) == 5 and memory.readbyte(0xFF1702) == 0 and memory.readbyte(0xFF1712) ==0 then
			outs = string.format("%d	%d	%d\n",Cnt,K_Chng,mov);
			print(outs);
			 Sleeper:write(outs);  
		end;
	end;
end;
