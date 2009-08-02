 target =   7176

--8186 Dance
--17919 Blaze + Hammer - 394
-- 18470 Box of Doom
--18482 Heal
--8732 










RNG = 0xFF2008;

testframe = savestate.create(); 
 
local buttonmap = {[1]='up',[2]='down',[4]='left',[8]='right',[16]='B',[32]='C',[64]='A',[128]='start'} 


c = {};
--reverse the target

cur = memory.readword(RNG);
x = target;
for k = 0,12,1 do
        y = k*65536+x;
        y = y - 7;
        if math.fmod(y,13) == 0 then 
			target = y/13;
		end;
end;
target = target - 8;

if target < 6000 then
	print('Target < 6000, algo will fail');
end;

if math.fmod(target,2) == 0 then
		even = true;
	else
		even = false;
	end;

savestate.save(testframe);
gens.frameadvance();
cur = memory.readword(RNG);
if math.fmod(cur,2) == 0 then
	if not even then
		savestate.load(testframe);
		joypad.set(1,{up=1});
		gens.frameadvance();
		cur = memory.readword(RNG);
	end;
else
	if even then
		savestate.load(testframe);
		joypad.set(1,{up=1});
		gens.frameadvance();
		cur = memory.readword(RNG);
	end;
end;


con_target = 65536 + math.fmod(20165*target, 65536);
miny = math.fmod(2197*cur+13744, 65536);
diff = con_target - miny;
c[1] = 1 + 2*math.floor(diff/4394);
diff = diff - (c[1]-1)*2197;
c[2] = 1 + 2*math.floor(diff/338);
diff = diff - (c[2]-1)*169;
c[3] = 1 + 2*math.floor(diff/26);
c[4] = 1 + diff - (c[3]-1)*13;

cur = memory.readword(RNG);
for i = 1,4,1 do
    cur = math.fmod((cur+c[i])*13+7,65536);
    if cur < 6000 then
    	print(string.format('Algo will fail on C%d',i));
    end;
end;

for inpt = 1,4,1 do
	cont = c[inpt];
	for bit,button in pairs(buttonmap) do
    		if AND(cont,bit) ~= 0  then
      			joypad.set({[button]=true})      
	    	end    
  		end   
  		gens.frameadvance();
  	end;

  	joypad.set(1,{right=1});    	
  	gens.frameadvance();
  	gens.frameadvance();
  		while(gens.lagged()) do
  	  			joypad.set(1,{C=1});    	  		   	  		   
	      		gens.emulateframeinvisible();
	    	
	  		end
	  		gens.emulateframeinvisible();
  	  		gens.emulateframeinvisible();  	  		
  	  		while(gens.lagged()) do
  	  			joypad.set(1,{up=1});    	  		   	  		   
	      		gens.emulateframeinvisible();

	  		end	  		
  	  		 gens.emulateframeinvisible(); 
  	  		 while(gens.lagged()) do
  	  			joypad.set(1,{C=1});    	  		   	  		   
	      		gens.emulateframeinvisible();	    		
	  		end
  	  		 gens.emulateframeinvisible(); 
  	  	