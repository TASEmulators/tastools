




RNG = 0xFF2008;
current = memory.readword(RNG);
CHECK = savestate.create();


Dmg = 0xFF3A54;
Z_Hp = 	0xFF16A8;
K_Hp = 0xFF34FC;
M_Hp = 0xFF16AA;
P_Hp = 0xFF16AC;
Z_Atk = 0xFF1554;
current = memory.readword(RNG);

LiveChars = 0;
local buttonmap = {[1]='up',[2]='down',[4]='left',[8]='right',[16]='A',[32]='B',[64]='C',[128]='start'} 

gens.speedmode("nothrottle")

--SitD Bot
MaxHP = 9999;
Gila = false;
Z_Start = memory.readword(Z_Hp);
M_Start = memory.readword(M_Hp);
P_Start = memory.readword(P_Hp);
if Z_Start > 0 then LiveChars = LiveChars + 1; end;
if M_Start > 0 then LiveChars = LiveChars + 1; end;
if P_Start > 0 then LiveChars = LiveChars + 1; end;
savestate.save(CHECK);
 for i = current,current+1024,1 do
	savestate.load(CHECK);	
	memory.writeword(P_Hp,999);
	memory.writeword(Z_Atk,0);
	memory.writeword(0xFF1506,0)
	memory.writeword(RNG,i);
	while(gens.lagged()) do
	   gens.emulateframefast();
	   gui.text(10,50,i);
	end;   		      
	  K_Start = memory.readword(K_Hp);
	  if  memory.readword(0xFF3354) == 5  then
      		print(string.format('RNG: %d HP: %d',i, K_Start));
      		MaxHP = K_Start;
      		Gila = true;
    	end;
     end;
