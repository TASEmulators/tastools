
target =  58680
F_Offst = 2;


START = savestate.create();
local buttonmap = {[1]='up',[2]='down',[4]='left',[8]='right',[16]='B',[32]='C',[64]='A',[128]='start'} 

TURN_COUNT = 0xFF9C07;
RNG = 0xFF0EC2;

savestate.save(START); 
--Get all RNG values up to location, figure out # of changes.
RNG_Pre = {};
framecnt = 1;
RNG_Pre[framecnt] = memory.readword(RNG);	
gens.emulateframefast();	
WasLag = gens.lagged();
RNG_Move_Type = {};
RNG_EX = 0;

posdist = 0;	
negdist = 0;
print('Finding Battle Transition');
--and  (memory.readbyte(TURN_COUNT) ~= 32) 
 
while (memory.readbyte(TURN_COUNT) ~= 0) and  (memory.readbyte(TURN_COUNT) ~= 32) do
	framecnt = framecnt+1;	
	RNG_Pre[framecnt] = memory.readword(RNG);				
	if (RNG_Pre[framecnt] == RNG_Pre[framecnt-1]) then
		if WasLag then
			RNG_Move_Type[framecnt-1] = -1;
		else
			RNG_Move_Type[framecnt-1] = 0;			
		end;
	elseif (math.fmod(RNG_Pre[framecnt]+256, 65536) == RNG_Pre[framecnt-1]) then	
			RNG_Move_Type[framecnt-1] = 1;
			negdist = negdist + 256;
	elseif (math.fmod(RNG_Pre[framecnt]+512, 65536) == RNG_Pre[framecnt-1]) then	
			RNG_Move_Type[framecnt-1] = 2;
			negdist = negdist + 512;
	elseif (RNG_Pre[framecnt] == math.fmod(RNG_Pre[framecnt-1]*13+7,65536)) then
			RNG_Move_Type[framecnt-1] = 3;
			RNG_EX = RNG_EX + 1;			
	elseif (RNG_Pre[framecnt] == math.fmod((RNG_Pre[framecnt-1]-256)*13+7,65536)) then
			RNG_Move_Type[framecnt-1] = 4;
			RNG_EX = RNG_EX + 1;
	elseif (RNG_Pre[framecnt]+256 == math.fmod((RNG_Pre[framecnt-1])*13+7,65536)) then
			RNG_Move_Type[framecnt-1] = 5;
			RNG_EX = RNG_EX + 1;
	end;		
	gens.emulateframefast();	
	WasLag = gens.lagged();
end
print(string.format('Battle Transition is %d frames away. RNG Executions: %d', framecnt,RNG_EX));

--Get battle info - Needed to test for right order

for z = 1,32,1 do
	
end;

savestate.load(START);


if RNG_EX == 0 then			
	for i = 1,framecnt-F_Offst,1 do
		curr_RNG = memory.readword(RNG);
		targtemp = target;
		if targtemp  < curr_RNG then
			upframes = math.floor((targtemp + 65536 - curr_RNG)/255);		
			downframes = math.floor((curr_RNG-targtemp)/256);		
		else
			upframes = math.floor((targtemp - curr_RNG)/255);				
			downframes = math.floor((curr_RNG - targtemp + 65536)/255);				
		end;		
		if ((RNG_Move_Type[i] == 0) and (upframes < 40)) then
				joypad.set(1,{up=true});
		elseif ((RNG_Move_Type[i] == 1) and ((downframes > 40) or (downframes == 0))) then
				joypad.set(1,{up=true});
		elseif ((RNG_Move_Type[i] == 2) and ((downframes > 40)  or (downframes == 0))) then
				joypad.set(1,{up=true});
		end;
		gens.emulateframefast();	
	end;
	curr_RNG = memory.readword(RNG);
	temp = curr_RNG-target;
	for bit,button in pairs(buttonmap) do
    		if AND(temp,bit) ~= 0 then
      			joypad.set({[button]=true})      
	    	end    
  	end 
  	gens.frameadvance();

elseif RNG_EX == 1 then
	Preframe = 1;	
	curr_RNG = memory.readword(RNG);
	--Find the RNG execution
	while (RNG_Move_Type[Preframe+1] < 3) do 
	 	Preframe = Preframe+1;
	end;
	nxtRNG = math.fmod(curr_RNG*13+7,65536);
	if nxtRNG > target then
		upframes = math.floor((128+target+65536-nxtRNG)/3315);
		newtarg = math.fmod(curr_RNG+upframes*255,65536);
	else
		upframes = math.floor((128+target-nxtRNG)/3315);
		newtarg = math.fmod(curr_RNG+upframes*255,65536);
	end;		
	for i = 1,Preframe,1 do
		curr_RNG = memory.readword(RNG);
		if newtarg  < curr_RNG then
			upframes = math.floor((newtarg + 65536 - curr_RNG)/255);		
			downframes = math.floor((curr_RNG-newtarg)/256);		
		else
			upframes = math.floor((newtarg - curr_RNG)/255);				
			downframes = math.floor((curr_RNG - newtarg + 65536)/255);				
		end;		
		if ((RNG_Move_Type[i] == 0) and (upframes < 40)) then
				joypad.set(1,{up=true});
		elseif ((RNG_Move_Type[i] == 1) and (downframes > 10)) then
				joypad.set(1,{up=true});
		elseif ((RNG_Move_Type[i] == 2) and (downframes > 10)) then
				joypad.set(1,{up=true});
		end;
		gens.emulateframefast();
	end;
	gens.emulateframefast();
	

		
for i = Preframe+2,framecnt-F_Offst,1 do
		curr_RNG = memory.readword(RNG);
		targtemp = target;
		if targtemp  < curr_RNG then
			upframes = math.floor((targtemp + 65536 - curr_RNG)/255);		
			downframes = math.floor((curr_RNG-targtemp)/256);		
		else
			upframes = math.floor((targtemp - curr_RNG)/255);				
			downframes = math.floor((curr_RNG - targtemp + 65536)/255);				
		end;		
		if ((RNG_Move_Type[i] == 0) and (upframes < 40)) then
				joypad.set(1,{up=true});
		elseif ((RNG_Move_Type[i] == 1) and ((downframes > 40) or (downframes == 0))) then
				joypad.set(1,{up=true});
		elseif ((RNG_Move_Type[i] == 2) and ((downframes > 40)  or (downframes == 0))) then
				joypad.set(1,{up=true});
		end;
		gens.emulateframefast();	
	end;	
	curr_RNG = memory.readword(RNG);
	temp = curr_RNG-target;
	if gens.lagged() then
		if temp/2 ~= math.floor(temp/2) then
			print('Script will fail - Wrong parity encountered.')
		else
			print('Lag frame encountered.')
			temp = temp/2;
		end;
	end;	
	print(temp)
	for bit,button in pairs(buttonmap) do
    		if AND(temp,bit) ~= 0 then
      			joypad.set({[button]=true})      
	    	end    
  	end 
  	gens.frameadvance();

end;

