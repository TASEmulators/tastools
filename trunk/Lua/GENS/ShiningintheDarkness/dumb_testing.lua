	
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
Combo = io.open("C:\\SitD_Shell2.txt", "w");
 

local buttonmap = {[1]='up',[2]='down',[4]='left',[8]='right',[16]='A',[32]='B',[64]='C',[128]='start'} 

	
SleepState = memory.readbyte(Sleep);
if SleepState == 0 then
    SleepState = 5; -- if not currently sleeping at start of battle - obviously putting to sleep.
end;   
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
for RNGc = 7019,65535,1 do
	savestate.load(CHECK);
	gens.frameadvance();
  	joypad.set(1,{left=1});    	
  	gens.frameadvance();
    memory.writeword(RNG,RNGc);
    gens.frameadvance();		  		  	  
  	while(gens.lagged()) do  	    
  	     joypad.set(1,{C=1});    
		gens.frameadvance();	    		
	end	   	
	 gens.frameadvance();    	  		 
	 while(gens.lagged()) do  	    
  	     joypad.set(1,{down=1});    
		gens.frameadvance();	    		
	end	 
	gens.frameadvance();    	  		 
	while(gens.lagged()) do  	    
  	     joypad.set(1,{C=1});    
		gens.frameadvance();	    		
	end	 
		gens.frameadvance();    	  		 
  	  	gens.frameadvance();  
  	  	gens.frameadvance();   
  	  	 joypad.set(1,{A=1});  
		 gens.frameadvance();     	  		  	  		   	  		 		
		 gens.frameadvance();   
  	     gens.frameadvance();   	
  	     joypad.set(1,{C=1});         		
	     gens.frameadvance();
	     gens.frameadvance(); 	
     while(gens.lagged()) do  	    
		gens.frameadvance();	    		
	end	 
	Sleeping = SleepState - memory.readbyte(Sleep);
	Z_Chng = Z_Start - memory.readword(Z_Hp);
	K_Chng = K_Start - memory.readword(K_Hp);
    M_Chng = M_Start - memory.readword(M_Hp);
    P_Chng = P_Start - memory.readword(P_Hp);
    K_ChngL = 0;
    
	  	outs = string.format('%d	%d	%d	%d	%d	%d\n', RNGc, K_Chng, K_ChngL, P_Chng, Z_Chng,  memory.readbyte(0xFF16F4), movie.framecount());
      	Combo:write(outs);   
      	print(outs);       
    end;

