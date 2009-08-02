
CHECK = savestate.create();

RNG = 0xFF2008;
Dmg = 0xFF3A54;
Z_Hp = 	0xFF16A8;
K_Hp = 0xFF34FC;
M_Hp = 0xFF16AA;
P_Hp = 0xFF16AC;
Sleep = 0xFF3512; -- Equals 4 if asleep.
P_Hp = 0xFF16AC;
Wachunga = true;
local buttonmap = {[1]='up',[2]='down',[4]='left',[8]='right',[16]='A',[32]='B',[64]='C',[128]='start'} 

gens.speedmode("nothrottle")
 savestate.save(CHECK);
--SitD Bot
MaxDmg = 0;
last = 0;
Z_Start = memory.readword(Z_Hp);
K_Start = memory.readword(K_Hp);
P_Start = memory.readword(P_Hp);
SleepState = memory.readbyte(Sleep);
if SleepState == 0 then
    SleepState = 5; -- if not currently sleeping at start of battle - obviously putting to sleep.
end;   
done = false
while not done do
	savestate.load(CHECK);
	N = math.random(2,4)
	for j=1,N,1 do
		temp = math.random(1,127)
		temp = temp*2+1	
		for bit,button in pairs(buttonmap) do
    		if AND(temp,bit) ~= 0  and bit ~= 16 then
      			joypad.set({[button]=true})      
	    	end    
  		end   
  		gens.emulateframeinvisible();  		
  	end;  
   joypad.set(1,{left=1});    	
  	gens.emulateframeinvisible();    
    gens.emulateframeinvisible();		  		  	  
  	while(gens.lagged()) do  	    
  	     joypad.set(1,{C=1});    
		gens.emulateframeinvisible();	    		
	end	   	
	 gens.emulateframeinvisible();    	  		 
	 while(gens.lagged()) do  	    
  	     joypad.set(1,{down=1});    
		gens.emulateframeinvisible();	    		
	end	 
	gens.emulateframeinvisible();    	  		 
	while(gens.lagged()) do  	    
  	     joypad.set(1,{C=1});    
		gens.emulateframeinvisible();	    		
	end	 
		gens.emulateframeinvisible();    	  		 
  	  	gens.emulateframeinvisible();  
  	  	gens.emulateframeinvisible();   
  	  	 joypad.set(1,{A=1});  
		 gens.emulateframeinvisible();     	  		  	  		   	  		 		
		 gens.emulateframeinvisible();   
  	     gens.emulateframeinvisible();   	
  	     joypad.set(1,{C=1});         		
	     gens.emulateframeinvisible();
	     gens.emulateframeinvisible(); 	
     while(gens.lagged() and memory.readword(Z_Hp) > 0 and memory.readword(P_Hp) == 999) do  	    
		gens.emulateframeinvisible();	    		
	end	 	
		P_Chng = P_Start - memory.readword(P_Hp);
		    Z_Chng = Z_Start - memory.readword(Z_Hp);
 	         K_Chng = K_Start - memory.readword(K_Hp); 	         
	
	  	if  K_Chng > 64 and P_Chng < 1 and Z_Chng < 1  then
     		done = true
	    end;
    end;
