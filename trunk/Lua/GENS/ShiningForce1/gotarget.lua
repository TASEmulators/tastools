 
target = 37184

control = {"Up ", "Down ", "Left ", "Right ", "B ", "C ", "A ", "Start "};



gui.register( function ()

curr_rng = memory.readwordunsigned(0xff0ec2);

print("Target: "..string.format('%d',target));

if (target < curr_rng) then
  target1 = target + 65536; 
else
  target1 = target;  
end;


	cnt_in = target1 - curr_rng;
	frameup = 0;
	while (cnt_in > 255) do
	    cnt_in = cnt_in - 255;
	    frameup = frameup + 1;
	end;
	precont = curr_rng+frameup*255;	
	new_targ = 256-cnt_in;
	outs = "Frame Up: "..string.format('%d',frameup).." ";
	print("Forward-");
	print(outs);
	outs = "";
	for x= 1,8,1 do
		if (new_targ%2) == 1 then
		   outs = outs..control[x];
		end;
		new_targ = math.floor(new_targ/2);
	end;
	print(outs);
	print("");

	if (target > curr_rng) then
	   curr_rng = curr_rng + 65536;
	end;
	cnt_in = curr_rng - target;
	frameup = 0;
	while (cnt_in > 256) do
	    cnt_in = cnt_in - 256;
	    frameup = frameup + 1;
	end;
	precont = curr_rng+frameup*255;	
	new_targ = cnt_in;
	outs = "Frame Release: "..string.format('%d',frameup).." ";
	print("Backward-");
	print(outs);
	outs = "";
	for x= 1,8,1 do
		if (new_targ%2) == 1 then
		   outs = outs..control[x];
		end;
		new_targ = math.floor(new_targ/2);
	end;
	print(outs);


end)