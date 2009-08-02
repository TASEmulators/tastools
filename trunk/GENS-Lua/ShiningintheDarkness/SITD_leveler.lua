

Char =2;
--           HP,  MP, Att, Def, Ag, Luck,  IQ 
Z_Weight = {.8,    0,  2,  0,    0,  0, 0};
P_Weight = { .5,    0,  3,  0,   0,   0,  0};




Address = {0xFF16B4, 0xFF16BA, 0xFF16C0, 0xFF1500, 0xFF1512, 0xFF1518, 0xFF16D2};

Hp = 0xFF16B4

local buttonmap = {[1]='up',[2]='down',[4]='left',[8]='right',[16]='B',[32]='A',[64]='C',[128]='start'} 
Score = 0;
maxvec = {0,0,0,0,0, 0, 0}
Check = savestate.create();
currscore = 0;
current = {}; 
if Char == 1 then offset = 0; Weights = Z_Weight; else offset = 4; Weights = P_Weight; end;

for i = 1,7,1 do
	current[i] = memory.readword(Address[i]+offset);
end;
savestate.save(Check);
	
for adv = 0,2,1 do	
	for i = 0,31,1 do
		for j = 1,7,1 do
	   		savestate.load(Check);
	   		if adv > 0 then
				for t = 1,adv,1 do
					joypad.set(1,{right=1});
					joypad.set(1,{left=1});
					joypad.set(1,{down=1});
					joypad.set(1,{up=1});
					joypad.set(1,{start=1});
					gens.emulateframefast();	
				end;
			end;
			if i > 15 then
				control = (i-16)+128;
			else
				control = i;
			end;
			control = control + j*16;
			for bit,button in pairs(buttonmap) do
    			if AND(control,bit) ~= 0  then
      				joypad.set({[button]=true})      
	    		end    
  			end
  			while (memory.readword(Hp+offset) == current[1]) do 
  				gui.text(1,80,string.format('Last Score: %g',currscore));
  				gui.text(1,90,string.format('Max Score: %g',Score));
  				gui.text(1,100,string.format('Hp %d   Attk %d   Def %d   Luck %d  IQ %d', maxvec[1], maxvec[3],maxvec[4], maxvec[6], maxvec[7]));
				gens.emulateframefast();	
			end;
				gens.emulateframefast();	
			    gens.emulateframefast();	
			    gens.emulateframefast();	
			    gens.emulateframefast();
			currscore = 0;  		  
			for k = 1,7,1 do			
				currscore = currscore+(memory.readword(Address[k]+offset)-current[k])*Weights[k];
				if memory.readword(Address[k]+offset)-current[k] > maxvec[k] then
					maxvec[k] = memory.readword(Address[k]+offset)-current[k];
				end;
			end;		
			if currscore > Score then
				Score = currscore;
				endresult = {adv, control};
				print(string.format('Advance %d   Control %d',adv, control));
			end;
		end;
	end;
end;

savestate.load(Check);
adv = endresult[1];
control = endresult[2];
if adv > 0 then
	for t = 1,adv,1 do
	joypad.set(1,{right=1});
					joypad.set(1,{left=1});
					joypad.set(1,{down=1});
					joypad.set(1,{up=1});					
			joypad.set(1,{start=1});
			gui.text(1,80,string.format('Last Score: %g',currscore));
  			gui.text(1,90,string.format('Max Score: %g',Score));		
			gens.frameadvance();
	end;
end;
			for bit,button in pairs(buttonmap) do
    			if AND(control,bit) ~= 0  then
      				joypad.set({[button]=true})      
	    		end    
  			end
  			for i=1,5,1 do
				gui.text(1,100,string.format('Score: %g',Score));			
				gens.frameadvance();
			end;
				