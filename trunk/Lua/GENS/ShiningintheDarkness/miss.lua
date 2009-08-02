local crits = 3;
local miss_rate = 7;

local count;

local X;
local D7;

miss={};

count = 1;
for D7 = 1,65535,1 do
X = 13 * D7;
X = 7+X;
X = X % 65536;
X = 13 * X;
X = 7+X;
X = X % 65536;
X = 100*X;
X = math.floor(X/65536);

if (X <= miss_rate) then
    miss[count] = D7;
    count = count + 1;
end;
end;

control = {"Up ", "Down ", "Left ", "Right ", "B ", "C ", "A ", "Start"};



gui.register( function ()

curr_rng = memory.readwordunsigned(0xff0ec2);

for i = 2,#miss,1 do
	if ((miss[i-1] < curr_rng) and (miss[i] > curr_rng)) then
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
	outs = "Miss: "
	for x= 1,8,1 do
		if (new_targ%2) == 1 then
		   outs = outs..control[x];
		end;
		new_targ = math.floor(new_targ/2);
	end;
	print(outs);
end)
