
CHECK = savestate.create();

RNG = 0xFF2008;
Dmg = 0xFF3A54;
Z_Hp = 	0xFF16A8;
K_Hp = 0xFF34FC;
M_Hp = 0xFF16AA;
P_Hp = 0xFF16AC;
Z_Atk = 0xFF1554;
Z_Atk2 = 0xFF1506;
Sleep = 0xFF3512; -- Equals 4 if asleep.
P_Hp = 0xFF16AC;
Wachunga = false;
local buttonmap = {[1]='up',[2]='down',[4]='left',[8]='right',[16]='A',[32]='B',[64]='C',[128]='start'} 
last = 0;
last2 =0;

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
    N = math.random(1,5)
	for j=1,N,1 do
		temp = 129 + math.random(1,7)*2				
		for bit,button in pairs(buttonmap) do
    		if AND(temp,bit) ~= 0 and bit ~= 16 and  bit ~= 32 and bit ~= 64 then
      			joypad.set({[button]=true})      
	    	end    
  		end 
  		gens.frameadvance()
  	end;
  		gens.frameadvance();
  		gens.frameadvance();
  		joypad.set(1,{C=1});
  		gens.frameadvance();
  		gens.frameadvance();
  	  	while(gens.lagged()) do
  	  			    	  		   	  		   
	      		gens.frameadvance();
	    		gui.text(10,60,string.format("%d %d",last2, last));
	  		end
  	N = math.random(1,5)
  	for j=1,N,1 do
		temp = math.random(1,127)
		temp = temp*2+1	
		for bit,button in pairs(buttonmap) do
    		if AND(temp,bit) ~= 0 and bit ~= 16 and  bit ~= 32 and bit ~= 64 then
      			joypad.set({[button]=true})      
	    	end    
  		end 
  		gens.frameadvance()
  	end;  
  		   gens.frameadvance();
  	  		joypad.set(1,{left=1});    	
  	  		gens.frameadvance();
  	  		gens.frameadvance();
  	  		while(gens.lagged()) do
  	  			joypad.set(1,{C=1});    	  		   	  		   
	      		gens.frameadvance();
	    		gui.text(10,60,string.format("%d %d",last2, last));
	  		end
	  		gens.frameadvance();
  	  		gens.frameadvance();  	  		
  	  		while(gens.lagged()) do
  	  			joypad.set(1,{up=1});    	  		   	  		   
	      		gens.frameadvance();
	    		gui.text(10,60,string.format("%d %d",last2, last));
	  		end
  	  		 gens.frameadvance(); 	
  	     	while(gens.lagged()) do  	
  	     		joypad.set(1,{C=1});       		
	      		gens.frameadvance();
	    		gui.text(10,60,string.format("%d %d",last2, last));
	  		end; 				  			  		
  	  		 gens.frameadvance();    	  		 
  	  		 gens.frameadvance();  
  	  		 gens.frameadvance();   
  	  		 joypad.set(1,{A=1});  
			 gens.frameadvance();     	  		  	  		   	  		 		
			 gens.frameadvance();   
  	     	while(gens.lagged()) do	  	   
  	     		joypad.set(1,{C=1});         		
	      		gens.frameadvance();
	    		gui.text(10,60,string.format("%d %d",last2, last));
	  		end;   				  
  	  		 gens.frameadvance(); 
  	  		 gens.frameadvance(); 
  	  		 gens.frameadvance(); 
  	         while(gens.lagged()) do  	     
	      		gens.frameadvance();
	    		gui.text(10,60,string.format("MAIN LOOP! %d %d",last2, last));
	  		end	   
	   gens.frameadvance();	
	   gui.text(1,90,K_Chng);	  
		Sleeping = SleepState - memory.readbyte(Sleep);
		P_Chng = P_Start - memory.readword(P_Hp);
		    Z_Chng = Z_Start - memory.readword(Z_Hp);
 	         K_Chng_Early = memory.readword(K_Hp); 	         	   	    					
 	 if Sleeping == 1 and P_Chng == 0  then        	
			    
		K_Chng = K_Chng_Early -  memory.readword(K_Hp);
	  	if  K_Chng_Early == 0 then
     		done = true
	    end;
    end;
   end;
