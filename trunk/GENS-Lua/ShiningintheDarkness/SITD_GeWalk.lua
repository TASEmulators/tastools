--Get Walk

Walk = io.open("C:\\SitD_Walk.txt", "w");
Direc = 0xFF000F; -- current direction Facing
-- 0 North
-- 1 East
-- 2 South
-- 3 West

Horiz  = 0xFF0010; -- Current Horizontal (East/West) location 
Vert = 0xFF0011; -- Current Vertical (North/South) location

done = false;
lastx = memory.readbyte(Horiz);
	lasty = memory.readbyte(Vert);	
	lastdir = memory.readbyte(Direc);			
while not done do 	
	gens.emulateframefast();
	while (gens.lagged()) do 	
       gens.emulateframefast();
       gui.text(10,50,outs);
    end;
    cx = memory.readbyte(Horiz)
    cy = memory.readbyte(Vert);
    cd = memory.readbyte(Direc);
    if lastx == cx and lasty == cy and lastdir == cd then
        done = true;
    else    	
    	
    	if lastdir ~= cd then
    		change = math.fmod(lastdir-cd+4,4)  
    		if  change == 3 then
    		  outs = string.format("R %d %d\n",cx,cy);
    		elseif change == 1 then
    		  outs = string.format("L %d %d\n",cx,cy);	
    		end;    	
    	elseif lastx ~= cx then
    	   if cd == 3 then
    	    	if lastx > cx then
    	    		outs = string.format("U %d %d\n",cx,cy);    	    	
    	    	else
    	    		outs = string.format("D %d %d\n",cx,cy);
    	    	end;
    	    else
    	    	if lastx < cx then
    	    		outs = string.format("U %d %d\n",cx,cy);    	    	
    	    	else
    	    		outs = string.format("D %d %d\n",cx,cy);
    	    	end;
    	    end;
    	 else
    	     if cd == 2 then
    	    	if lasty  < cy then
    	    		outs = string.format("U %d %d\n",cx,cy);    	    	
    	    	else
    	    		outs = string.format("D %d %d\n",cx,cy);
    	    	end;
    	    else
    	    	if lasty > cy then
    	    		outs = string.format("U %d %d\n",cx,cy);    	    	
    	    	else
    	    		outs = string.format("D %d %d\n",cx,cy);
    	    	end;
    	    end;
    	 end;
    	 Walk:write(outs);
    	lastx = memory.readbyte(Horiz);
		lasty = memory.readbyte(Vert);	
		lastdir = memory.readbyte(Direc);	
	end;   	 
end;    	    

Walk:close();    	
	