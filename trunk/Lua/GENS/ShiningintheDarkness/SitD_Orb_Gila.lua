

current =58050;




CHECK = savestate.create();
Combo = io.open("C:\\SitD_Gila.txt", "w");
RNG = 0xFF2008;
Dmg = 0xFF3A54;
Z_Hp = 	0xFF16A9;
K_Hp = 0xFF34FC;
M_Hp = 0x16AA;
P_Hp = 0xFF16AC;


local buttonmap = {[1]='up',[2]='down',[4]='left',[8]='right',[16]='A',[32]='B',[64]='C',[128]='start'} 

gens.speedmode("nothrottle")

--SitD Bot

Temps = {16, 64, 192, 144, 208}
MaxDmg = 0;
last = 0;
Z_Start = memory.readbyte(Z_Hp);

P_Start = memory.readword(P_Hp);
savestate.save(CHECK);
 for i = 0,10,1 do		
    for j = 1,5,1 do
    savestate.load(CHECK);		
    if i > 0 then
	for q= 1,i,1 do
	    joypad.set(1,{start=1});  
  		gens.emulateframefast();
  		gens.emulateframefast();   
    end;
    end;
    temp = Temps[j];
	for bit,button in pairs(buttonmap) do
    		if AND(temp,bit) ~= 0 then
      			joypad.set({[button]=true})      
	    	end    
    end   
    gens.emulateframefast();
    gens.emulateframefast();
	while(gens.lagged()) do
	   gens.emulateframefast();
	   gui.text(10,50,K_Start);
	end;   		
	 K_Start = memory.readword(K_Hp);
  	K_Chng = K_Start - memory.readword(K_Hp);
     if  memory.readword(0xFF3354) == 5 then
      gens.frameadvance();
    end;
end;
end;