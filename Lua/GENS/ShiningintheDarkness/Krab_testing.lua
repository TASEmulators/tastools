	
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

Wachunga = true;
Combo = io.open("C:\\SitD_Krab.txt", "w");
 

local buttonmap = {[1]='up',[2]='down',[4]='left',[8]='right',[16]='A',[32]='B',[64]='C',[128]='start'} 


	--memory.writeword(Z_Atk+6,21);


 savestate.save(CHECK);

--SitD Bot
MaxDmg = -999;
last = 0;
last2 = 0;
Z_Start = memory.readword(Z_Hp);
K_Start = memory.readword(K_Hp);
M_Start = memory.readword(M_Hp);
P_Start = memory.readword(P_Hp);
	LiveChars = 0;
	if Z_Start > 0 then LiveChars = LiveChars + 1; end;
	if M_Start > 0 then LiveChars = LiveChars + 1; end;
	if P_Start > 0 then LiveChars = LiveChars + 1; end;
for RNGc = 7000,65535,1 do
	savestate.load(CHECK);
	    memory.writeword(RNG,RNGc);	
  	  		joypad.set(1,{C=1});    	
  	  		gens.emulateframeinvisible();    
	  		gens.emulateframeinvisible();  	  		
  	  			joypad.set(1,{C=1});    	  		   	  		   
	      		gens.emulateframeinvisible();
	    		gens.emulateframeinvisible();		  		  	  
  	         while(gens.lagged()) do  	     
	      		gens.emulateframeinvisible();	    		
	  		end	   
		Z_Chng = Z_Start - memory.readword(Z_Hp);
	K_Chng = K_Start - memory.readword(K_Hp);
    M_Chng = M_Start - memory.readword(M_Hp);
    P_Chng = P_Start - memory.readword(P_Hp);
    
    K_ChngL = 0;
    if Z_Chng > 1 and M_Chng == 0 and P_Chng == 0 and K_Chng > 8  then    
         outs = string.format('%d	%d	%d	%d\n', RNGc, K_Chng, Z_Chng, movie.framecount());
    	 Combo:write(outs);   
    	 print(outs);   
    end;
end;
