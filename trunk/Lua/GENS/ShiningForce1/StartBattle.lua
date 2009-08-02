
target =  49258 






START = savestate.create();
local buttonmap = {[1]='up',[2]='down',[4]='left',[8]='right',[16]='B',[32]='C',[64]='A',[128]='start'} 


RNG = 0xFF0EC2;

savestate.save(START); 
--Get all RNG values up to location, figure out # of changes.
gens.emulateframefast();	
last_RNG = memory.readword(RNG);	
RNG_Count = 0;
gens.emulateframefast();	
while (last_RNG ~= memory.readword(RNG)) do
	last_RNG = memory.readword(RNG);
	gens.emulateframefast();	
	RNG_Count = RNG_Count + 1;
end;
print(RNG_Count);
--Calculate the backwards RNG. 
savestate.load(START);
curr_RNG = memory.readword(RNG);
multiplier = 1;
for t  = 1,RNG_Count,1 do
   multiplier = math.mod((13*multiplier),65536);
end;

 
print(multiplier)
if last_RNG > target then
		dist = target + 65536 - last_RNG;
	else
		dist = target - last_RNG;
	end;		

done = FALSE;
offst = 0;
print(dist)
while not done do 	
	if math.mod(dist+offst*65536,multiplier) < (765) then
		done = true;
	else	
		offst = offst + 1;
	end;
end	

temp = 256-math.floor((dist+offst*65536)/multiplier);

for bit,button in pairs(buttonmap) do
    		if AND(temp,bit) ~= 0 then
      			joypad.set({[button]=true})      
	    	end    
end
for i=1,RNG_Count+1,1 do
	gens.emulateframefast();	
end;

while (target ~= memory.readword(RNG)) do
	if memory.readword(RNG) < (target  - 255) then
	  	joypad.set(1,{up=true});
	else
		temp = 256  - target + memory.readword(RNG);	
		for bit,button in pairs(buttonmap) do
    		if AND(temp,bit) ~= 0 then
      			joypad.set({[button]=true})      
	    	end    
		end
	end
	gens.emulateframefast();
end;


