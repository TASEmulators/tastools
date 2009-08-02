startmov = movie.framecount()	
Z_Hp = 	0xFF16A8;
K_Hp = 0xFF34FC;
M_Hp = 0xFF16AA;
P_Hp = 0xFF16AC;
Z_Sleep = 0xFF1566;
P_Sleep = 0xFF1586;
P_Hp = 0xFF16AC;
E_Sleep = 0xFF3512;
RNG = 0xFF2008;

Sleeper =  io.open("C:\\SitD_Darksol_Sleep3_Fuckthis3.txt", "w");
RNGc = memory.readword(RNG);
local buttonmap = {[1]='up',[2]='down',[4]='left',[8]='right',[16]='B',[32]='C',[64]='A',[128]='start'} 
startmov = movie.framecount()	
CHECK = savestate.create();
savestate.save(CHECK);
Z_Start = memory.readword(Z_Hp);
M_Start = memory.readword(M_Hp);
P_Start = memory.readword(P_Hp);
K_Start = memory.readword(K_Hp)+100;
for Cnt= 39701,65535,1 do
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
			if (memory.readbyte(E_Sleep) == 1) then			 
			 Sleeps = 1
			else
			 Sleeps = 0;
			end;
			while gens.lagged() do
				gens.emulateframeinvisible();
			end;
	Z_Chng = Z_Start - memory.readword(Z_Hp);
	K_Chng = K_Start - memory.readword(K_Hp);
    M_Chng = M_Start - memory.readword(M_Hp);
    P_Chng = P_Start - memory.readword(P_Hp);
		 mov =  movie.framecount() - startmov;
		 dabreak = memory.readbyte(0xFF1702) + memory.readbyte(0xFF1712);
		if K_Chng > 200 and P_Chng == 0 and Z_Chng == 0 and M_Chng == 0 and memory.readword(0xFF3354) == 5  then
			outs = string.format("%d	%d	%d	%d	%d\n",Cnt,K_Chng,mov, dabreak, Sleeps);
			print(outs);
			 Sleeper:write(outs);  
		end;
	end;

