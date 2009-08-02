
LEVEL = 0xFF9C9D;


START = savestate.create();

local buttonmap = {[1]='up',[2]='down',[4]='left',[8]='right',[16]='B',[32]='C',[64]='A',[128]='start'} 

TURN_COUNT = 0xFF9C07;
TURN_PLACE = 0xFF9C08;
RNG = 0xFF0EC2;
STAT_HEX = {0xFF9C9E, 0xFF9C9F, 0xFF9CA0, 0xFF9CA5, 0xFF9CA8,0xFF9CA2};
MAPPLAYER = 0xFFA1B6;
savestate.save(START); 
--Get all RNG values up to location, figure out # of changes.
framecnt = 0;

RNG_Move_Type = {};
RNG_EX = 0;

currchar = memory.readbyte(MAPPLAYER + 16*memory.readbyte(TURN_PLACE + 2*memory.readbyte(TURN_COUNT)));
currlev = memory.readbyte(40*currchar + LEVEL);
ZeroCheck = 27751;
ZeroResult = 12975;
last_RNG = memory.readword(RNG);
start_RNG = memory.readword(RNG);
currstats = {};
for i = 1,6,1 do
	currstats[i] = memory.readbyte(STAT_HEX[i] + currchar*40);
	memory.writebyte(STAT_HEX[i] + currchar*40,0);
end;
joypad.set(1,{up=true});
gens.emulateframefast();
 
done = false;
print('Finding Level Up');
while not done do	
	framecnt = framecnt+1;	
	memory.writeword(RNG,ZeroCheck); 
	joypad.set(1,{up=true});
	gens.emulateframefast();
	if memory.readbyte(40*currchar + LEVEL) == currlev+1 then
	 done = true;
	end;	
		
end
stats = {};
max_stats = {};
min_stats = {};
for i = 1,6,1 do
	stats[i] = memory.readbyte(STAT_HEX[i] + currchar*40);
	
	max_stats[i] = math.max(math.min(math.floor(stats[i]/4),5)-1,0) + stats[i];
	min_stats[i] = math.max(-1*math.min(math.floor(stats[i]/4),4) + stats[i], currstats[i]);
end;

done = false
currcolor = 1;
ccount = 0;
currloc = 1;
plusr = true;
minusr = true;
leftr = true;
rightr = true;
savestate.load(START);
colors = {'white','gray','black'};
while not done do
	
    input_state = input.get(); 
	gens.frameadvance();
gui.text(10,50,              '     ATT   DEF   AGI   HP    MP   CRIT');
	gui.text(10,60,string.format('MAX: %02d    %02d    %02d    %02d    %02d    %02d', max_stats[1], max_stats[2], max_stats[3], max_stats[4], max_stats[5], max_stats[6]));
	gui.text(6,70,string.format('CURR: %02d    %02d    %02d    %02d    %02d    %02d', currstats[1], currstats[2], currstats[3], currstats[4], currstats[5], currstats[6]));
	gui.text(10,80,string.format('MIN: %02d    %02d    %02d    %02d    %02d    %02d', min_stats[1], min_stats[2], min_stats[3], min_stats[4], min_stats[5], min_stats[6]));
	gui.text(currloc*24+18,70,'<-');
	if input_state.enter then
		done = true;
	end;
	if input_state.numpad8 and plusr then
		plusr = false;
		currstats[currloc] = math.min((currstats[currloc] + 1), max_stats[currloc]);				
	end;
	if input_state.numpad2 and minusr then
		minusr = false;
		currstats[currloc] = math.max((currstats[currloc] - 1), min_stats[currloc]);			
	end;
	if input_state.numpad4 and leftr then
		leftr = false;
		currloc = math.max(currloc-1,1);	
	end;
	if input_state.numpad6 and rightr then
		rightr = false;
		currloc = math.min(currloc+1,6);	
	end;
	
	 if (not input_state.numpad8) then
       plusr = true;
     end;
     if (not input_state.numpad2) then
       minusr = true;
     end;
     if (not input_state.numpad6) then
       rightr = true;
     end;
     if (not input_state.numpad4) then
       leftr = true;
     end;
end;
start_RNG = start_RNG - framecnt;
destat = {};
offst = {};
for i = 1,6,1 do
 destat[i] = currstats[i] - stats[i];
 offst[i] = max_stats[i] - stats[i];
end;

i = start_RNG-255; 
done = false;
while not done  do
   x = i;
   fail = false
   for z=1,6,1 do		
		x = math.mod((x*13 + 7),65536);
		Y1 = math.floor(x*(offst[z]+1)/65536);
		x = math.mod((x*13 + 7),65536);
		Y2 = math.floor(x*(1+offst[z])/65536);
		Y = Y1 - Y2;
		if (Y + stats[z]) < math.min(min_stats[z]) then
			Y = 0;
		end;
		if Y ~= destat[z] then
		  fail = true;
		end;
	end;
	if not fail then
	 target = i;
	 done = true;
	else	 
	 i = i - 1;
	 if i < 0 then
	   i = 65536;
	 end;
	end;
	
end;

print(string.format("Target: %d  Distance: %d",target,math.floor((start_RNG-target)/255)));
savestate.load(START);

for i = 1,framecnt-1,1 do	
	if memory.readword(RNG)-target < (framecnt-i)+255 then
	 		joypad.set(1,{up=true});
	end;
	gens.emulateframefast();
end;
temp = memory.readword(RNG)-target;

for bit,button in pairs(buttonmap) do
    		if AND(temp,bit) ~= 0 then
      			joypad.set({[button]=true})      
	    	end    
  	end 
  	gens.emulateframefast();
  	gens.emulateframefast();
  	gens.emulateframefast();
    test = 1;
  	for i = 1,6,1 do
		if (currstats[i] ~= memory.readbyte(STAT_HEX[i] + currchar*40)) then	
	  	test = 0;
		end;	  
	end;
	if (test == 0)then
	    print("Normal method failed - Trying secondary");
		savestate.load(START);

		for i = 1,framecnt-1,1 do	
			if memory.readword(RNG)-(target-255) < (framecnt-i)+255 then
	 			joypad.set(1,{up=true});
			end;
			gens.emulateframefast();
		end;
		temp = memory.readword(RNG)-target;
		for bit,button in pairs(buttonmap) do
    		if AND(temp,bit) ~= 0 then
      			joypad.set({[button]=true})      
	    	end    
	    end;
	    test = 1;
		gens.emulateframefast();
  		gens.emulateframefast();
  		gens.emulateframefast();
    	test = 1;
  		for i = 1,6,1 do
		if (currstats[i] ~= memory.readbyte(STAT_HEX[i] + currchar*40)) then	
	 		 test = 0;
		end;
		end;	  	    	
  	end    	
  	if (test==0)  then
  		print('Level up failed');
  	end;
  	gens.pause();