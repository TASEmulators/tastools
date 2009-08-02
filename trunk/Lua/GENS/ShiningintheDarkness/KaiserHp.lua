
x = io.open("C:\\Kaiser_Target.txt", "w");
KHP = 60;
K_Hp = 0xFF34FC;
RNG = 0xFF2008;
gens.speedmode("turbo")
CHECK = savestate.create();
 savestate.save(CHECK);
 for i = 14395,65536,1 do
 
	lastread =  memory.readword(K_Hp);
	savestate.load(CHECK);
	memory.writeword(RNG,i);
	gui.text(50,50,string.format("%d   %d     %d\n",i,KHP,lastread));
	gens.frameadvance();
	gui.text(50,50,string.format("%d   %d     %d\n",i,KHP,lastread));
	gens.frameadvance();
    gui.text(50,50,string.format("%d   %d     %d\n",i,KHP,lastread));
	if memory.readword(K_Hp) < KHP then 
	     KHP = memory.readword(K_Hp);
          outs = string.format("%d   %d\n",i,KHP);
          print(outs)        
   	end;
end;





x:close();