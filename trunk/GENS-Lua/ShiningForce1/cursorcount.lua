--CursorCount


cursor_live = true;
start = true;
total_move = 0;
prevx = 0;
prevy = 0;
gui.register( function ()

gui.text(10,30, string.format('Cursor: %d',total_move))

if cursor_live and (memory.readbyte(0xFF0FFD) ~=  16) then
	cursor_live = false;
	if start then
		start = false;
		prevx = memory.readbyte(0xFF9C4A);
		prevy = memory.readbyte(0xFF9C4B);		
	else 
		total_move = total_move + math.max(math.abs(prevx-memory.readbyte(0xFF9C4A)),math.abs(prevy-memory.readbyte(0xFF9C4B)));
		
	end;
elseif not cursor_live and (memory.readbyte(0xFF0FFD) ==  16) then
	    prevx = memory.readbyte(0xFF9C4A);
		prevy = memory.readbyte(0xFF9C4B);	
		cursor_live = true;
end;
end);