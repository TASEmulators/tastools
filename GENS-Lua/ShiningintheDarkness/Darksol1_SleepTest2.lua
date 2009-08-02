startmov = movie.framecount()	
K_Hp = 0xFF34FC;
Z_Sleep = 0xFF1566;
P_Sleep = 0xFF1586;
P_Hp = 0xFF16AC;
E_Sleep = 0xFF3512;
RNG = 0xFF2008;

Sleeper =  io.open("C:\\SitD_Darksol_SleepFuckthis1.txt", "w");
RNGc = memory.readword(RNG);
local buttonmap = {[1]='up',[2]='down',[4]='left',[8]='right',[16]='B',[32]='C',[64]='A',[128]='start'} 
startmov = movie.framecount()	
CHECK = savestate.create();
savestate.save(CHECK);
K_Start = memory.readword(K_Hp);
for Cnt= 0,65535,1 do
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
	while (memory.readbyte(P_Sleep) == 3) and memory.readword(P_Hp) > 0 do
				gens.emulateframeinvisible();
	end;
	if (memory.readbyte(P_Sleep) == 0) and memory.readbyte(E_Sleep)== 2 then
		while gens.lagged() do
				gens.emulateframeinvisible();
		end;
		K_Chng = K_Start - memory.readword(K_Hp);    
		 mov =  movie.framecount() - startmov;
		if memory.readword(0xFF3354) == 5 and K_Chng > 200 then
			outs = string.format("%d	%d	%d %d\n",Cnt,K_Chng,mov,memory.readbyte(0xFF1702));
			print(outs);
			 Sleeper:write(outs);  
		end;
	end;
end;
