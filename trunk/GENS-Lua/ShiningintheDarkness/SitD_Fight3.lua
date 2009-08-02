	
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
  	    gens.frameadvance();
		temp = math.random(1,15)	
		for bit,button in pairs(buttonmap) do
    		if AND(temp,bit) ~= 0  then
      			joypad.set({[button]=true})      
	    	end    
  		end   
  		gens.frameadvance();  		
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
  			gens.frameadvance();
  			gens.frameadvance();
  			
  			for i = 1,5,1 do 
	  		gens.frameadvance();
	    
	  		end;   				  
   	  	if memory.readword(RNG) <  9222  and memory.readword(RNG) > 9000 then
   	  		done = true;
   	  	end;
   	  		       
    
end;