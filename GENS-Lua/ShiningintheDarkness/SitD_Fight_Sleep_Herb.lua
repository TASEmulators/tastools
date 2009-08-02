
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
Wachunga = true;
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
		temp = math.random(1,127)
		temp = temp*2+1	
		for bit,button in pairs(buttonmap) do
    		if AND(temp,bit) ~= 0 and bit ~= 16 and  bit ~= 32 and bit ~= 64 then
      			joypad.set({[button]=true})      
	    	end    
  		end 
  		gens.emulateframeinvisible()
  	end;  
  		   gens.emulateframeinvisible();
  	  		joypad.set(1,{right=1});    	
  	  		gens.emulateframeinvisible();
  	  		gens.emulateframeinvisible();
  	  		while(gens.lagged()) do
  	  			joypad.set(1,{C=1});    	  		   	  		   
	      		gens.emulateframeinvisible();
	    		gui.text(10,60,string.format("%d %d",last2, last));
	  		end
	  		gens.emulateframeinvisible();
  	  		gens.emulateframeinvisible();  	  		
  	  		while(gens.lagged()) do
  	  			joypad.set(1,{up=1});    	  		   	  		   
	      		gens.emulateframeinvisible();
	    		gui.text(10,60,string.format("%d %d",last2, last));
	  		end
  	  		 gens.emulateframeinvisible(); 	
  	     	while(gens.lagged()) do  	
  	     		joypad.set(1,{C=1});       		
	      		gens.emulateframeinvisible();
	    		gui.text(10,60,string.format("%d %d",last2, last));
	  		end; 				  	
	  			gens.emulateframeinvisible();		  		
  	  			while(gens.lagged()) do  	
  	     		joypad.set(1,{A=1});       		
	      		gens.emulateframeinvisible();
	    		gui.text(10,60,string.format("%d %d",last2, last));
	  		end; 	
	  			gens.emulateframeinvisible();
	  				gens.emulateframeinvisible();
  	         while(gens.lagged()) do  	     
	      		gens.emulateframeinvisible();
	    		gui.text(10,60,string.format("MAIN LOOP! %d %d",last2, last));
	  		end	   
	  	Sleeping = SleepState - memory.readbyte(Sleep);
	Z_Chng = Z_Start - memory.readword(Z_Hp);
	K_Chng = K_Start - memory.readword(K_Hp);
    
    P_Chng = P_Start - memory.readword(P_Hp);
    K_ChngL = 0;
	  if Wachunga and P_Chng == 0 and Z_Chng < -29 and Sleeping == 1 then    
    	memory.writeword(Z_Atk,0);
		memory.writeword(Z_Atk2,0);
	    memory.writeword(K_Hp,100);
		memory.writeword(P_Hp,999);
    	LiveChars2 = 0;    	
		if memory.readword(Z_Hp) > 0 then LiveChars2 = LiveChars2 + 1; end;
		if memory.readword(M_Hp) > 0 then LiveChars2 = LiveChars2 + 1; end;
		if memory.readword(P_Hp) > 0 then LiveChars2 = LiveChars2 + 1; end;    	
    	for i = 1,LiveChars2,1 do
    		gens.emulateframeinvisible();
  	  		joypad.set(1,{C=1});  
  	  		gens.emulateframeinvisible();
  	  		gens.emulateframeinvisible();
  	  		joypad.set(1,{C=1});  
  			gens.emulateframeinvisible();
  			gens.emulateframeinvisible();  	
  	     	while(gens.lagged()) do
	      		gens.emulateframeinvisible();
	    		gui.text(10,50,'Wachunga!');
	  		end;   		
	  	end;
	  	K_ChngL = 100 - memory.readword(K_Hp);	  
	  	end;
	  	
	    if K_ChngL >  40 and memory.readbyte(0xFF16F4)==0  then        				   			  	
     		done = true
	    end;
    end;
