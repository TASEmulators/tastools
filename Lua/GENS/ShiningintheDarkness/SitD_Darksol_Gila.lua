

current =58050;




CHECK = savestate.create();
Combo = io.open("C:\\SitD_Gila.txt", "w");
RNG = 0xFF2008;
Dmg = 0xFF3A54;
Z_Hp = 	0xFF16A9;
K_Hp = 0xFF34FC;
M_Hp = 0x16AA;
P_Hp = 0xFF16AC;

Z_Atk = 0xFF1554;
Z_Atk2 = 0xFF1506;
local buttonmap = {[1]='up',[2]='down',[4]='left',[8]='right',[16]='A',[32]='B',[64]='C',[128]='start'} 

gens.speedmode("nothrottle")

--SitD Bot
MinKStart = 9999;
MaxDmg = 0;
last = 0;

Z_Start = memory.readbyte(Z_Hp);

P_Start = memory.readword(P_Hp);
savestate.save(CHECK);
for i = 1,5000,1 do
    savestate.load(CHECK);		
	N = math.random(0,3)
	for j=1,N,1 do
	    q = math.random(0,31)		
		if q > 15 then
			control = (q-16)+128;
		else
			control = q;
		end;
		for bit,button in pairs(buttonmap) do
    		if AND(control,bit) ~= 0  then
      			joypad.set({[button]=true})      
	    	end    
  		end   
  		gens.emulateframefast();  		
  	end;
  	control = math.random(0,7) + math.random(1,7)*16;
  	for bit,button in pairs(buttonmap) do
    	if AND(control,bit) ~= 0  then
      		joypad.set({[button]=true})      
	    end    
  	end   
  	gens.emulateframefast();  
  	
    gens.emulateframefast();
    gens.emulateframefast();
	while(gens.lagged()) do
	   gens.emulateframefast();
	   gui.text(10,50,K_Start);
	end;   		
	K_Start   = memory.readword(K_Hp);	
     if  memory.readword(0xFF3354) == 5  and K_Start < 1300 then
     gens.frameadvance();
      print(K_Start);
      MinKStart = K_Start;
    end;
end;
