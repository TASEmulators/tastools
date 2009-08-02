minframes = 999999;

LEVEL = 0xFF9C9D;
STAT_HEX = {0xFF9C9E, 0xFF9C9F, 0xFF9CA0, 0xFF9CA5, 0xFF9CA8,0xFF9CA2};
CHECK = savestate.create();
last_fc = 0;
local buttonmap = {[1]='up',[2]='down',[4]='left',[8]='right',[16]='A',[32]='B',[64]='C',[128]='start'} 
savestate.save(CHECK);
i = 0;
done = false;
while not done do
   i = i + 1;
	savestate.load(CHECK);
    fc = 100;
    framedata = {};
	for j=1,100,1 do
		temp = math.random(1,255)
		framedata[j] = temp;		
		for bit,button in pairs(buttonmap) do
    		if AND(temp,bit) ~= 0 then
      			joypad.set({[button]=true})      
	    	end    
  		end 
  		gui.text(10,60,string.format("%d Run: %d frames", i, last_fc));
	    gens.emulateframefast();
    end;    
while (memory.readbyte(LEVEL + 40*0) ~= 5) do
  temp = math.random(1,255)
  fc = fc+1;
  framedata[fc] = temp;
  for bit,button in pairs(buttonmap) do
    if AND(temp,bit) ~= 0 then
      joypad.set({[button]=true})
    end    
  end 
  if last_fc == 0 then
	 gui.text(10,60,string.format("%d Run: FAIL", i));   
  else
	 gui.text(10,60,string.format("%d Run: %d frames", i, last_fc));
  end;
  gui.text(10,72,string.format("Minimum Run: %d frames", minframes));
  gens.emulateframefast();
end;


if (memory.readbyte(STAT_HEX[1]) == 15) and (memory.readbyte(STAT_HEX[2]) > 22) and (memory.readbyte(STAT_HEX[3]) == 30) and (memory.readbyte(STAT_HEX[4]) == 28) then
   last_fc = fc;
	if fc < minframes then	
		minframes = fc;
		framedatasave = {};
		for k = 1,fc,1 do
			framedatasave[k] = framedata[k];
		end;
	end;
else
last_fc = 0;
end;

input_state = input.get(); 
gens.frameadvance();
	if input_state.enter then
		done = true;
	end;
end;

savestate.load(CHECK);
for j = 1,minframes,1 do		
		temp = framedatasave[j];
		for bit,button in pairs(buttonmap) do
    		if AND(temp,bit) ~= 0 then
      			joypad.set({[button]=true})      
	    	end    
  		end 
  	 	gens.emulateframefast();
  end;
  gui.text(10,72,string.format("Minimum Run: %d frames", minframes));
  gens.emulateframefast();
  gens.pause();