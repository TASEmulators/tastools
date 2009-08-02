
CHECK = savestate.create();

RNG = 0xFF2008;
Dmg = 0xFF3A54;
Z_Hp = 	0xFF16A9;
K_Hp = 0xFF34FC;
M_Hp = 0x16AA;
P_Hp = 0x16AC;

local buttonmap = {[1]='up',[2]='down',[4]='left',[8]='right',[16]='A',[32]='B',[64]='C',[128]='start'} 

gens.speedmode("nothrottle")
 savestate.save(CHECK);
--SitD Bot
MaxDmg = 0;
last = 0;
Z_Start = memory.readbyte(Z_Hp);
K_Start = memory.readword(K_Hp);
P_Start = memory.readword(P_Hp);
done = false
for i = 1,127,1 do
	savestate.load(CHECK);
	--N = math.random(1,10)
			
		temp = i*2+1
		for bit,button in pairs(buttonmap) do
    		if AND(temp,bit) ~= 0  then
      			joypad.set({[button]=true})      
	    	end    
  		end   	
  		gens.frameadvance();  	 	
  		gens.frameadvance();  		
  		gens.frameadvance();
  		gens.frameadvance();  		  	
	while(gens.lagged()) do
	   gens.frameadvance();
	   gui.text(10,50,string.format('Max-%d    Last-%d:',MaxDmg,last));
	end;   	
	
	Z_Chng = Z_Start - memory.readbyte(Z_Hp);
	K_Chng = K_Start - memory.readword(K_Hp);
	P_Chng = P_Start - memory.readword(P_Hp);
	if K_Chng > MaxDmg then
	     MaxDmg = K_Chng
	end;
	last =K_Chng
 
  if  K_Chng == 60 and Z_Chng == 0 and P_Chng == 0 then
     done = true
    end;
end;