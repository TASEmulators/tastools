Z_Hp = 	0xFF16A8;
K_Hp = 0xFF34FC;
M_Hp = 0xFF16AA;
P_Hp = 0xFF16AC;
Z_Sleep = 0xFF1566;
P_Sleep = 0xFF1586;

E_Sleep = 0xFF3512;
RNG = 0xFF2008;

--Finish Darksol, shortest frames
startmov = movie.framecount()
RNGc = memory.readword(RNG);
local buttonmap = {[1]='up',[2]='down',[4]='left',[8]='right',[16]='B',[32]='C',[64]='A',[128]='start'} 
startmov = movie.framecount()	
CHECK = savestate.create();
savestate.save(CHECK);
Z_Start = memory.readword(Z_Hp);
M_Start = memory.readword(M_Hp);
P_Start = memory.readword(P_Hp);
K_Start = memory.readword(K_Hp);

for Cnt= 0,5000,1 do
		savestate.load(CHECK);
		memory.writeword(RNG,RNGc+Cnt);
		gens.emulateframeinvisible();
		joypad.set(1,{C=1});   
		gens.emulateframeinvisible();	 
		gens.emulateframeinvisible();	  		
  	  	joypad.set(1,{C=1});    	  		   	  		   
	    gens.emulateframeinvisible();
	     gens.emulateframeinvisible();
	      gens.emulateframeinvisible();
	while (gens.lagged()) do
				gens.emulateframeinvisible();
	end;
	    
	K_Chng = K_Start - memory.readword(K_Hp);	
       mov =  movie.framecount() - startmov;
      P_Chng = P_Start - memory.readword(P_Hp);
	if memory.readword(K_Hp)==0  and P_Chng == 0 then
			outs = string.format("Cnt: %d, Frame: %d",RNGc+Cnt, mov);
			print(outs);
		end;
	end;

