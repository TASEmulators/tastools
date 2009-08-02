	
CHECK = savestate.create();

RNG = 0xFF2008;
Dmg = 0xFF3A54;
Z_Hp = 	0xFF16A8;
K_Hp = 0xFF34FC;
M_Hp = 0xFF16AA;
P_Hp = 0xFF16AC;
Z_Atk = 0xFF1554;
Z_Atk2 = 0xFF1506;
Wachunga = true;

Sleep = 0xFF3512; -- Equals 4 if asleep.
temp = savestate.create();

local buttonmap = {[1]='up',[2]='down',[4]='left',[8]='right',[16]='A',[32]='B',[64]='C',[128]='start'} 

--gens.speedmode("nothrottle")
 savestate.save(CHECK);
--SitD Bot
MaxDmg = -999;
last = 0;
last2 = 0;
Z_Start = memory.readword(Z_Hp);
K_Start = memory.readword(K_Hp)+100;
M_Start = memory.readword(M_Hp);
P_Start = memory.readword(P_Hp);
	LiveChars = 0;
	if Z_Start > 0 then LiveChars = LiveChars + 1; end;
	if M_Start > 0 then LiveChars = LiveChars + 1; end;
	if P_Start > 0 then LiveChars = LiveChars + 1; end;
done = false
while not done do

	savestate.load(CHECK);
	N = math.random(0,1);
	if N == 1 then
		gens.emulateframeinvisible();
	end;
	N = math.random(1,9)
	for j=1,N,1 do
		temp = math.random(1,127)
		temp = temp*2+1	
		for bit,button in pairs(buttonmap) do
    		if AND(temp,bit) ~= 0  then
      			joypad.set({[button]=true})      
	    	end    
  		end   
  		gens.emulateframeinvisible();  		
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
	  		end
  	  		 gens.emulateframeinvisible(); 
  	         while(gens.lagged()) do  	     
	      		gens.emulateframeinvisible();
	    		gui.text(10,60,string.format("%d %d",last2, last));
	  		end	   
	Z_Chng = Z_Start - memory.readword(Z_Hp);
	K_Chng = K_Start - memory.readword(K_Hp);
    M_Chng = M_Start - memory.readword(M_Hp);
    P_Chng = P_Start - memory.readword(P_Hp);
    mov = movie.framecount();
    if (memory.readbyte(0xFF1716)==0) and Z_Chng == 0 and P_Chng == 0 and   K_Chng == 213 and  mov > 163900  then        
     		done = true    	
    	end;
 
end;
