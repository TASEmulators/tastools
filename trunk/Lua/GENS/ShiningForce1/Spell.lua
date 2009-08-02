minframes = 999999;

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
while (memory.readbytesigned(0XFF00F7) ~= -84) do
  temp = math.random(1,255)
  fc = fc+1;
  framedata[fc] = temp;
  for bit,button in pairs(buttonmap) do
    if AND(temp,bit) ~= 0 then
      joypad.set({[button]=true})
    end    
  end 
  gui.text(10,60,string.format("%d Run: %d frames", i, last_fc));
  gui.text(10,72,string.format("Minimum Run: %d frames", minframes));
  gens.emulateframefast();
end;

last_fc = fc;
if fc < minframes then	
	minframes = fc;
	framedatasave = {};
	for k = 1,fc,1 do
		framedatasave[k] = framedata[k];
	end;
--	outs = string.format("Frames: %d\n", fc); 		
--	Combo = io.open("C:\\Spell.txt", "w");
--    Combo:write(outs);
--    for i = 1,#(framedata),1 do
--    	Combo:write(string.format("%d\n",framedata[i]));
--    end;
--    Combo:close();
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