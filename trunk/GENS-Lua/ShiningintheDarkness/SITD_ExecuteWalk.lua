--Get Walk
local buttonmap = {[1]='up',[2]='down',[4]='left',[8]='right',[16]='A',[32]='B',[64]='C',[128]='start'} 

Walk = io.open("C:\\SitD_Walk.txt", "r");
Direc = 0xFF000F; -- current direction Facing
-- 0 North
-- 1 East
-- 2 South
-- 3 West

Horiz  = 0xFF0010; -- Current Horizontal (East/West) location 
Vert = 0xFF0011; -- Current Vertical (North/South) location
P_Hp = 0xFF16AC;
Battle = 0xFF3519;
RNG = 0xFF2008; 
LastPos = savestate.create();
memory.writeword(P_Hp,999);
WalkBuff = {};
cnt = 0;
NextStep = Walk:read('*l');
while NextStep ~= nil do
    cnt = cnt + 1;
	z = string.sub(NextStep,1,1);
	if z == 'U' then
		WalkBuff[cnt] = 1;
	elseif z == 'D' then
		WalkBuff[cnt] = 2;
	elseif z == 'L' then
		WalkBuff[cnt] = 4;
	elseif z == 'R' then
		WalkBuff[cnt] = 8;
	end;
	NextStep = Walk:read('*l');
end;

	
totalsteps = cnt;		
memory.writebyte(0xFF3519,1);

	
AlarmState = savestate.create();	
HoldState = savestate.create();
TempState = savestate.create();
done = false;
currstep = 1;
while not done do 	
	while gens.lagged() do
		savestate.save(LastPos);
		gens.frameadvance();
		gui.text(10,120,string.format('Next Step %d', WalkBuff[currstep]));
		gui.text(10,130,string.format('Step %d of %d',currstep,totalsteps-1));
	end;
	currRNG = memory.readword(RNG);
	savestate.load(LastPos);
	if memory.readbyte(Battle)== 0 then
	   if currstep < 8 then
	   	  print('FAILED, ENEMY ENCOUNTERED TOO CLOSE');
	   	  done = true;
	   else
	     print('Loaded Alarm State');	   	 	
	      savestate.load(AlarmState)
	      currstep = AlarmStep;
	      WalkBuff[AlarmStep+6] = WalkBuff[AlarmStep+6] + 16;
	   end;
	else
	if math.fmod(currstep,6) == 0 then
		if currstep > 6 then
		    savestate.save(TempState);
		    savestate.load(HoldState);
			savestate.save(AlarmState);
			savestate.load(TempState);
		end
		AlarmStep = currstep-6;
		savestate.save(HoldState);	
	end;		
	step = WalkBuff[currstep];	 		
	Encounter1 = math.fmod(math.fmod(step+currRNG,65536)*13+7,65536);
	res = math.floor(Encounter1*254/65536);
	if res < 22 then
		step = step+188;
		Encounter1 = math.fmod(math.fmod(step+currRNG,65536)*13+7,65536);
		res = math.floor(Encounter1*254/65536);
 	else
 		Encounter2 = math.fmod(math.fmod(WalkBuff[currstep+1]+Encounter1,65536)*13+7,65536);
 		res = math.floor(Encounter2*254/65536); 		
 		if res < 22 then
 		   step = step + 128;
 		end;
 	end;	
 		for bit,button in pairs(buttonmap) do
    		if AND(step,bit) ~= 0  then
      			joypad.set({[button]=true})      
	    	end    
  		end   	 
    gens.frameadvance();
    gens.frameadvance();
    if currstep == totalsteps-1 then
    	done = true
    end;
    currstep = currstep+1;
   	end;
end;    	    

Walk:close();    	
gens.pause();	