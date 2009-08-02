
Frame_start = {9363,  13127, 25428, 31558, 35080, 39916, 43679, 49070, 51627, 56360, 66819, 69706, 73402, 84485,  96644}
Frame_stop =  {12250, 13600, 30697, 34391, 37170, 41567, 47360, 49510, 53863, 64084, 68029, 70171, 83876, 93648, 100540} 
curr = 1;
while 1 do
	mov = movie.framecount();
	if mov > Frame_start[curr] and mov < Frame_stop[curr] then
		gens.emulateframeinvisible()
		gens.emulateframeinvisible()
		gens.emulateframeinvisible()
		gens.frameadvance()
	else
		gens.frameadvance()
	end;
	if mov > Frame_stop[curr] and curr ~= 15 then
		curr = curr+1;
	end;	
end;
		