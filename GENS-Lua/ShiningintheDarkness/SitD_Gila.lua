




RNG = 0xFF2008;
current = memory.readword(RNG);
CHECK = savestate.create();


Dmg = 0xFF3A54;
Z_Hp = 	0xFF16A8;
K_Hp = 0xFF34FC;
M_Hp = 0xFF16AA;
P_Hp = 0xFF16AC;
Z_Atk = 0xFF1554;

LiveChars = 0;
local buttonmap = {[1]='up',[2]='down',[4]='left',[8]='right',[16]='A',[32]='B',[64]='C',[128]='start'} 

gens.speedmode("nothrottle")

--SitD Bot!
MaxHP = 9999;
Gila = false;
Z_Start = memory.readword(Z_Hp);
M_Start = memory.readword(M_Hp);
P_Start = memory.readword(P_Hp);
savestate.save(CHECK);
 for i = current+512,current+1024,1 do
	savestate.load(CHECK);	
	
	memory.writeword(RNG,i);
	while(gens.lagged()) do
	   gens.emulateframefast();
	   gui.text(10,50,i);
	end;   		
	K_Start =  memory.readword(K_Hp);
	
    if memory.readword(0xFF3354) == 5 and K_Start < MaxHP then
      		print(string.format('RNG: %d HP: %d',i, K_Start));      
      		MaxHP = K_Start;	
    	end;
     end;
