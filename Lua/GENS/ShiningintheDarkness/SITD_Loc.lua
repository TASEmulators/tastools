Direc = 0xFF000F; -- current direction Facing
-- 0 North
-- 1 East
-- 2 South
-- 3 West

Horiz  = 0xFF0010; -- Current Horizontal (East/West) location 
Vert = 0xFF0011; -- Current Vertical (North/South) location
gui.register( function ()
    cx = memory.readbyte(Horiz)
    cy = memory.readbyte(Vert);
    cd = memory.readbyte(Direc);
	if cd == 0 then
		d = 'North';
	elseif cd == 1 then
		d = 'East';
	elseif cd ==2 then
		d = 'South';
	else
		d = 'West';
	end;	
	
outs = 'Facing ' .. d;
outs2 = string.format('X: %d  Y: %d',cx,cy);
gui.text(1,100,outs);
gui.text(1,110,outs2);
end)