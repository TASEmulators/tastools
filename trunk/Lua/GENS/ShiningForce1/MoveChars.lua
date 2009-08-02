CamPosX =0xFF9C48;
CamPosY =0xFF9C49;
Alive = 0xFFA1B6;
LocX = 0xFFA1B7;
LocY = 0xFFA1B8;
OL = 0xFF9C08;
SizeX = 24;
SizeY = 24;
Holding = false;
leftclickrelease = true;
lbrelease = true;
lbrelease = true;
gens.registerbefore(function()
	  CamX = memory.readbyte(CamPosX);
	  CamY = memory.readbyte(CamPosY);	  
	  OnScreen = {};
	  PosX = {};
	  PosY = {};
	  Ord = {};
	  for n = 0,31,1 do
	    Ord[memory.readbyte(OL+n*2)] = n;
	  end;
	  i=0;
	  for c = 0,31,1 do
	      if memory.readbyte(Alive+c*16) ~= 255 then
	      	  X = memory.readbyte(LocX+c*16);
	      	  Y = memory.readbyte(LocY+c*16);
	      	  if ((X > CamX-1) and (X < CamX+11) and (Y > CamY-1) and (Y < CamY+10)) then
	      	    i = i+1;
	      	    OnScreen[i] = c;
	      	  	PosX[i] = (X-CamX)*SizeX-4;
	      	  	PosY[i] = (Y-CamY)*SizeY+7;
	      	    gui.drawbox(PosX[i],PosY[i],PosX[i]+SizeX,PosY[i]-SizeY,"clear","white")
	      	    gui.text(PosX[i]+3,PosY[i]-8,string.format("%d %d",c,Ord[c]));
	      	    gui.text(PosX[i]+3,PosY[i]-21,string.format("%d %d",X,Y));
	      	  end;
	     end;
	 end;     
	 input_state = input.get();
     xmouse = input_state.xmouse+1;
     ymouse = input_state.ymouse+1;
     Mx = math.floor((xmouse+4)/SizeX)+CamX;
     My = math.floor((ymouse-7)/SizeY)+CamY+1;     
     MDx = (Mx-CamX)*SizeX-4;
 	 MDy = (My-CamY)*SizeY+7;
     if ((xmouse > 0) and (xmouse < 320) and (ymouse > 0) and (ymouse < 224)) then     	     	     	
         if (not Holding) then
     	  gui.drawbox(MDx,MDy,MDx+SizeX,MDy-SizeY,"opaque","black")
     	  if (input_state.leftclick) and leftclickrelease then
     	   leftclickrelease = false;
       		for m = 1,i,1 do       			         		
        		  if ((xmouse>=PosX[m]) and (xmouse<=PosX[m]+SizeX) and (ymouse<=PosY[m]) and (ymouse>=PosY[m]-SizeY)) then
            		 Holding=true;            		 
            	 	CharHold = OnScreen[m];            	
           	 	   CharOrd = Ord[CharHold];
              	end;	 		            
     		end;        
     	  end;
     	elseif (Holding) then     	     	  
     	  gui.drawbox(MDx,MDy,MDx+SizeX,MDy-SizeY,"opaque","red")     	
     	   memory.writebyte(LocX+CharHold*16,Mx);
     	   memory.writebyte(LocY+CharHold*16,My);
     	   if (input_state.leftbracket) and (CharOrd > 0) and lbrelease then
     	   	 lbrelease = false;
     	     temp = memory.readbyte(OL+CharOrd*2-2);
     	     memory.writebyte(OL+CharOrd*2-2,CharHold);
     	     memory.writebyte(OL+CharOrd*2,temp);
     	     CharOrd = CharOrd-1;
     	   end;   	   
     	   if (input_state.rightbracket) and (CharOrd < 31) and rbrelease then
     	     rbrelease = false;
     	     temp = memory.readbyte(OL+CharOrd*2+2);
     	     memory.writebyte(OL+CharOrd*2+2,CharHold);
     	     memory.writebyte(OL+CharOrd*2,temp);
     	     CharOrd = CharOrd+1;
     	   end;   	   
     	   if (input_state.leftclick) and leftclickrelease then
     	      leftclickrelease = false;
     	      Holding = false;
     	   end;     	   
     	end;
     end;     	   
     if (not input_state.leftclick) then
       leftclickrelease = true;
       end;
     if (not input_state.leftbracket) then
       lbrelease = true;
       end;
       if (not input_state.rightbracket) then
       rbrelease = true;
       end;
end);	      
