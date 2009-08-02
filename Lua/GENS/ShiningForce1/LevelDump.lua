
--LEVEL WATCH

x = io.open("C:\\LevelDump.txt", "w");

Lvl = 0xFF9C9D
At = 0xFF9C9E
Df = 0xff9c9F
Ag = 0xFF9CA0
Hp = 0xFF9CA5

To_Watch = {0,1,7,2,21,22};
Levels = {};
Names = {'Max','Mae','Gort','Pelle','Amon','Balbaroy'}

for i = 1,#To_Watch,1 do
   Levels[i] = memory.readbyte(Lvl+To_Watch[i]*40);
end;

while movie.active() do
   gens.emulateframefast()
   gens.emulateframefast()
   gens.emulateframefast()
   gens.emulateframefast()
   gens.emulateframefast()
for i = 1,#To_Watch,1 do
	if Levels[i] ~= memory.readbyte(Lvl+To_Watch[i]*40) then
	Levels[i] = memory.readbyte(Lvl+To_Watch[i]*40);
	outs = Names[i]..string.format(" - Level: %d  At: %d  Df %d  Ag: %d  Hp: %d\n\n",	 memory.readbyte(Lvl+To_Watch[i]*40),  memory.readbyte(At+To_Watch[i]*40),memory.readbyte(Df+To_Watch[i]*40),memory.readbyte(Ag+To_Watch[i]*40),memory.readbyte(Hp+To_Watch[i]*40));
	x:write(outs);
	end;
end
end
x:close();