Z_Hp = 	0xFF16A8;
K_Hp = 0xFF34FC;
M_Hp = 0xFF16AA;
P_Hp = 0xFF16AC;
Z_Sleep = 0xFF1566;
P_Sleep = 0xFF1586;

E_Sleep = 0xFF3512;
RNG = 0xFF2008;

Combo = io.open("C:\\SitD_DS_F2-5.txt", "w");
RNGc = memory.readword(RNG);
local buttonmap = {[1]='up',[2]='down',[4]='left',[8]='right',[16]='B',[32]='C',[64]='A',[128]='start'} 
startmov = movie.framecount()	
CHECK = savestate.create();
savestate.save(CHECK);
Z_Start = memory.readword(Z_Hp);
M_Start = memory.readword(M_Hp);
P_Start = memory.readword(P_Hp);
K_Start = memory.readword(K_Hp)+100;

for Cnt= 27049,65536,1 do
		savestate.load(CHECK);
		memory.writeword(RNG,Cnt);
		gens.emulateframeinvisible();
		joypad.set(1,{C=1});   
		gens.emulateframeinvisible();	 
		gens.emulateframeinvisible();	  		
  	  	joypad.set(1,{C=1});    	  		   	  		   
	    gens.emulateframeinvisible();
	     gens.emulateframeinvisible();
	      gens.emulateframeinvisible();
	
	while (memory.readbyte(E_Sleep)== 2) do
				gens.emulateframeinvisible();
	end;
	if  memory.readbyte(E_Sleep)== 1 then
	      while (gens.lagged() and memory.readbyte(E_Sleep)== 1) do
				gens.emulateframeinvisible();
		end;		    
	K_Chng = K_Start - memory.readword(K_Hp);	
	if  memory.readbyte(E_Sleep)== 1 and  memory.readword(0xFF3354) == 5 and K_Chng > 200 then
			outs = string.format("%d	%d	%d	%d\n",Cnt, K_Chng,  memory.readbyte(P_Sleep),memory.readword(RNG));
				Combo:write(outs);   
			print(outs);
		end;
	end;
end;

