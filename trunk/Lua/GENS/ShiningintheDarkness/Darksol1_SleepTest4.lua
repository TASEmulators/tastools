Z_Hp = 	0xFF16A8;
K_Hp = 0xFF34FC;
M_Hp = 0xFF16AA;
P_Hp = 0xFF16AC;
Z_Sleep = 0xFF1566;
P_Sleep = 0xFF1586;

E_Sleep = 0xFF3512;
RNG = 0xFF2008;


RNGc = memory.readword(RNG);
local buttonmap = {[1]='up',[2]='down',[4]='left',[8]='right',[16]='B',[32]='C',[64]='A',[128]='start'} 
startmov = movie.framecount()	
CHECK = savestate.create();
savestate.save(CHECK);
Z_Start = memory.readword(Z_Hp);
M_Start = memory.readword(M_Hp);
P_Start = memory.readword(P_Hp);

for Cnt= 1,5000,1 do
		savestate.load(CHECK);
		memory.writeword(RNG,RNGc+Cnt);
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
  	  		 gens.emulateframeinvisible();   	  
  	  		 while(gens.lagged()) do
  	  			joypad.set(1,{C=1});    	  		   	  		   
	      		gens.emulateframeinvisible();	    		
	  		end
  	  		 gens.emulateframeinvisible(); 
	while (gens.lagged()) do
				gens.emulateframeinvisible();
	end;
	Z_Chng = Z_Start - memory.readword(Z_Hp);
	P_Chng = P_Start - memory.readword(P_Hp);
	M_Chng = M_Start - memory.readword(M_Hp);	
	if (P_Chng == 0) and memory.readbyte(E_Sleep)== 3 then
			outs = string.format("Cnt: %d RNG: %d",RNGc+Cnt, memory.readword(RNG));
			print(outs);
		end;
	end;

