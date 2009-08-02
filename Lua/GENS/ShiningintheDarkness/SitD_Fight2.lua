	
CHECK = savestate.create();

RNG = 0xFF2008;
Dmg = 0xFF3A54;
Z_Hp = 	0xFF16A8;
K_Hp = 0xFF34FC;
M_Hp = 0xFF16AA;
P_Hp = 0xFF16AC;
Z_Atk = 0xFF1554;
Z_Atk2 = 0xFF1506;
Wachunga =false;

Sleep = 0xFF3512; -- Equals 4 if asleep.


local buttonmap = {[1]='up',[2]='down',[4]='left',[8]='right',[16]='A',[32]='B',[64]='C',[128]='start'} 

gens.speedmode("nothrottle")
 savestate.save(CHECK);
--SitD Bot
MaxDmg = 999;

last = 0;
Z_Start = memory.readword(Z_Hp);
K_Start = memory.readword(K_Hp);
M_Start = memory.readword(M_Hp);
P_Start = memory.readword(P_Hp);
done = false
while not done do

	savestate.load(CHECK);
	N = math.random(1,2)
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
  		gens.emulateframeinvisible(); 	      	  		  	
  		N = math.random(1,2);
  		if N == 1 then
  	  		joypad.set(1,{C=1});  
  	    else
  	    	joypad.set(1,{A=1});  
  	    end;
  	  	N = math.random(1,2);
  		if N == 1 then
  	  		joypad.set(1,{start=1});  
  	    end;
  			gens.emulateframeinvisible();
  			gens.emulateframeinvisible();
  			
  			Dmg = 999;
  	     	while(gens.lagged()) do
  	     	
	  		gens.emulateframeinvisible();
	    
	  		end;   				  
   	  	 			   
	Z_Chng = Z_Start - memory.readword(Z_Hp);
	K_Chng = K_Start - memory.readword(K_Hp);
    M_Chng = M_Start - memory.readword(M_Hp);
    P_Chng = P_Start - memory.readword(P_Hp);
        
 
  if K_Chng > 64  and P_Chng == 0  then
     done = true
    end;
    
end;