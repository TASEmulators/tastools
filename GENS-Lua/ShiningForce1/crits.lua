local crit_rate = 3;
local level =5;
local miss_rate = 8;
local count;

local X;
local D7;

miss={};
crits={};
dhsc = {};
crit_dmg = {};
dhdc = {};
dhdc_dmg ={};

doublehit = math.floor((level*2)/3)+7;

count = 2;
count2 = 2;
count3 = 2;
count4 = 2;
miss[1] = 0;
crits[1] = 0;
dhsc[1] = 0;
dhdc[1] = 0;


for D7 = 1,65535,1 do
X = 13 * D7;
X = 7+X;
X = X % 65536;
X = 13 * X;
X = 7+X;
X = X % 65536;
y = 100*X;
y = math.floor(y/65536);

if (y <= 7) then
    miss[count] = D7;
    count = count + 1;
elseif (y > miss_rate) then
   X = 13 * X;
   X = 7+X;
   X = X % 65536;	
   X = 13 * X;
   X = 7+X;
   X = X % 65536;	

   Y = 100*X;   
   Y = math.floor(Y/65536);

   X = 13 * X;
   X = 7+X;
   X = X % 65536;	
   A = 21*X;
   A = math.floor(A/65536);
   
   X = 13 * X;
   X = 7+X;
   X = X % 65536;	
   B = 21*X;
   B = math.floor(B/65536);

   
   if (Y<crit_rate) then              
              crits[count2] = D7-224;
	      crit_dmg[count2] = 80+math.floor((A+B)/2);
		count2 = count2 + 1;
       end;
end;
end;

for D7 = 1,65535,1 do
X = 13 * D7;
X = 7+X;
X = X % 65536;
y = 100*X;
y = math.floor(y/65536);
if (y < doublehit) then


	X = 13 * X;
	X = 7+X;
	X = X % 65536;
	y = 100*X;
	y = math.floor(y/65536);
	if (y > miss_rate) then
	   X = 13 * X;
	   X = 7+X;
	   X = X % 65536;	
	   X = 13 * X;
	   X = 7+X;
	   X = X % 65536;	
	   C1 = 100*X;   
	   C1 = math.floor(C1/65536);
	   X = 13 * X;
	   X = 7+X;
	   X = X % 65536;	
	   A = 21*X;
	   A = math.floor(A/65536);   
	   X = 13 * X;
	   X = 7+X;
	   X = X % 65536;	
	   B = 21*X;
	   B = math.floor(B/65536);
	   X = 13 * X;
	   X = 7+X;
	   X = X % 65536;	
	   Y = 100*X;   
	   Y = math.floor(Y/65536);
	   X = 13 * X;
	   X = 7+X;
	   X = X % 65536;	
	   X = 13 * X;
	   X = 7+X;
	   X = X % 65536;	
	   C2 = 100*X;   
	   C2 = math.floor(C2/65536);
	   X = 13 * X;
	   X = 7+X;
	   X = X % 65536;	
	   C = 21*X;
	   C = math.floor(C/65536);   
	   X = 13 * X;
	   X = 7+X;
	   X = X % 65536;	
	   D = 21*X;
	   D = math.floor(D/65536);
  	  if ((y > miss_rate) and (Y > miss_rate) and ((C1 < crit_rate) or (C2 < crit_rate)))  then              
              dhsc[count3] = D7-224;	      
	      count3 = count3 + 1;
          end;
         if ((y > miss_rate) and (Y > miss_rate) and (C1 < crit_rate) and (C2 < crit_rate))  then
              dhdc[count4] = D7-224;
	      dhdc_dmg[count4] = (math.floor(80+(A+B)/2)+math.floor(80+(C+D)/2))/2;	      
	      count4 = count4 + 1;              
          end;
end;

end;
end;




 
control = {"Up ", "Down ", "Left ", "Right ", "B ", "C ", "A ", "Start "};



gui.register( function ()

curr_rng = memory.readwordunsigned(0xff0ec2);

for i = 2,#miss,1 do
	if ((miss[i-1] < curr_rng) and (miss[i] >= curr_rng)) then
	  target = miss[i];
	end;
end;

	cnt_in = target - curr_rng;
	frameup = 0;
	while (cnt_in > 255) do
	    cnt_in = cnt_in - 255;
	    frameup = frameup + 1;
	end;
	precont = curr_rng+frameup*255;	
	new_targ = 256-cnt_in;
	outs = "Miss: ";
	if (frameup>0) then
		outs = outs.."^"..string.format('%d',frameup).." ";
	end;
	for x= 1,8,1 do
		if (new_targ%2) == 1 then
		   outs = outs..control[x];
		end;
		new_targ = math.floor(new_targ/2);
	end;
	print(outs);
for i = 2,#crits,1 do
	if ((crits[i-1] < curr_rng) and (crits[i] >= curr_rng)) then
	  target = crits[i];
	  temp = crit_dmg[i];
	end;
end;

	cnt_in = target - curr_rng;
	frameup = 0;
	while (cnt_in > 255) do
	    cnt_in = cnt_in - 255;
	    frameup = frameup + 1;
	end;
	precont = curr_rng+frameup*255;	
	new_targ = 256-cnt_in;
	outs = "Crit: ";
	if (frameup>0) then
		outs = outs.."^"..string.format('%d',frameup).." ";
	end;
	for x= 1,8,1 do
		if (new_targ%2) == 1 then
		   outs = outs..control[x];
		end;
		new_targ = math.floor(new_targ/2);
	end;
       
	outs = outs.." Dmg:"..string.format('%d',temp);
	print(outs);
target = curr_rng;
    for i = 2,#dhsc,1 do
	if ((dhsc[i-1] < curr_rng) and (dhsc[i] > curr_rng)) then
	  target = dhsc[i];

	end;
     end;

	cnt_in = target - curr_rng;
	frameup = 0;
	while (cnt_in > 255) do
	    cnt_in = cnt_in - 255;
	    frameup = frameup + 1;
	end;
	precont = curr_rng+frameup*255;	
	new_targ = 256-cnt_in;
	outs = "DHSC: ";
	if (frameup>0) then
		outs = outs.."^"..string.format('%d',frameup).." ";
	end;
	for x= 1,8,1 do
		if (new_targ%2) == 1 then
		   outs = outs..control[x];
		end;
		new_targ = math.floor(new_targ/2);
	end;
       	
	print(outs);
target = curr_rng;
for i = 2,#dhdc,1 do
	if ((dhdc[i-1] < curr_rng) and (dhdc[i] > curr_rng)) then
	  target = dhdc[i];
	  temp = dhdc_dmg[i];
	end;
end;
	targetsv = target+224;
	cnt_in = target - curr_rng;
	frameup = 0;
	while (cnt_in > 255) do
	    cnt_in = cnt_in - 255;
	    frameup = frameup + 1;
	end;
	precont = curr_rng+frameup*255;	
	new_targ = 256-cnt_in;
	outs = "DHDC: ";
	if (frameup>0) then
		outs = outs..string.format('%d',targetsv).."  ^"..string.format('%d',frameup).." ";
	end;
	for x= 1,8,1 do
		if (new_targ%2) == 1 then
		   outs = outs..control[x];
		end;
		new_targ = math.floor(new_targ/2);
	end;
       
	outs = outs.." Dmg:"..string.format('%d',temp);
	print(outs);
	print("");
 



end)
