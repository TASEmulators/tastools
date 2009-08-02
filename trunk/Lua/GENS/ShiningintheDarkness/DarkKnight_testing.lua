	
CHECK = savestate.create();

RNG = 0xFF2008;
Dmg = 0xFF3A54;
Z_Hp = 	0xFF16A8;
K_Hp = 0xFF34FC;
M_Hp = 0xFF16AA;
P_Hp = 0xFF16AC;
Z_Atk = 0xFF1554;
Z_Atk2 = 0xFF1506;
Z_Arm = 0xFF150C;
Sleep = 0xFF3512; -- Equals 4 if asleep.

Wachunga = true;
Combo = io.open("C:\\SitD_DKNew3.txt", "w");
 

local buttonmap = {[1]='up',[2]='down',[4]='left',[8]='right',[16]='A',[32]='B',[64]='C',[128]='start'} 

	

 savestate.save(CHECK);
 startmov = movie.framecount()
--SitD Bot
MaxDmg = -999;
Z_Start = memory.readword(Z_Hp);
K_Start = memory.readword(K_Hp)+100;
M_Start = memory.readword(M_Hp);
P_Start = memory.readword(P_Hp);
for RNGc = 6696,65535,1 do
	savestate.load(CHECK);
	
	gens.emulateframeinvisible();
  	joypad.set(1,{right=1});    	
  	gens.emulateframeinvisible();
    memory.writeword(RNG,RNGc);
    gens.emulateframeinvisible();		  		  	  
  	while(gens.lagged()) do  	    
  	     joypad.set(1,{C=1});    
		gens.emulateframeinvisible();	    		
	end	
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
	while(gens.lagged()) do  	    
  	     joypad.set(1,{A=1});    
		gens.emulateframeinvisible();	    		
	end	 
	gens.emulateframeinvisible();    	  		 
     while(gens.lagged() and memory.readword(Z_Hp) > 0 and memory.readword(P_Hp) == 999 ) do  	    
		gens.emulateframeinvisible();	    		
	end	 	
	Z_Chng = Z_Start - memory.readword(Z_Hp);
	K_Chng = K_Start - memory.readword(K_Hp);
    M_Chng = M_Start - memory.readword(M_Hp);
    P_Chng = P_Start - memory.readword(P_Hp);
     mov =  movie.framecount() - startmov;
    K_ChngL = 0;

    if  Z_Chng == 0 and P_Chng == 0 and K_Chng > 200 then    
	  	outs = string.format('%d	%d	%d	%d	%d\n', RNGc, K_Chng,  memory.readbyte(0xFF1710),  mov, memory.readwordsigned(0xFF3354));
      	Combo:write(outs);   
      	print(outs);       
    end;
end;
