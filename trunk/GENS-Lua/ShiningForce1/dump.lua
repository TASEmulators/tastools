

start_ag_add = 0xFFA1BB;

start_x_add = 0xFFA1B7;
start_y_add = 0xFFA1B8;
outs = "End_Loc = [" 


count = 0;
done = 0;
while (done == 0) do
	z = memory.readbytesigned(start_ag_add+count*16);
	if (z == -1) then
	   done = 1;
	else
	   count = count + 1;
        end;	
end;

for i = 0,count-1,1 do

   outs = outs..string.format("%d %d;",memory.readbyte(start_x_add+16*i), memory.readbyte(start_y_add+16*i));
end;
 outs = outs.."];";


x = io.open("C:\\end_loc.txt", "w");




x:write(outs);

x:close();