-- SNES9x-style input display
-- Drag and drop with the mouse

x = 193
y = 216
pad1 = 1
pad2 = 1

function drawpad(padnum,padx,pady)

   color = "#4cdc48ff"
   controller = joypad.read(padnum)
   padx = x
   
   if     (padnum == 1) then pady = y
   elseif (padnum == 2) then pady = y+9
   end   

   if (controller.right) then
      gui.line (padx+1, pady+1, padx+2, pady+1, color)   -- ##
      gui.line (padx+2, pady+2, padx+3, pady+2, color)   --  ###
      gui.pixel(padx+5, pady+3, color)                   --    ##
      gui.line (padx+2, pady+4, padx+3, pady+4, color)   --  ###
      gui.line (padx+1, pady+5, padx+2, pady+5, color)   -- ##
      gui.line (padx+4, pady+2, padx+4, pady+4, color)
   end
   
      if (controller.left) then
      gui.line (padx+12, pady+1, padx+13, pady+1, color) --    ##
      gui.line (padx+11, pady+2, padx+12, pady+2, color) --  ###
      gui.pixel(padx+ 9, pady+3, color)                  -- ##
      gui.line (padx+11, pady+4, padx+12, pady+4, color) --  ###
      gui.line (padx+12, pady+5, padx+13, pady+5, color) --    ##
      gui.line (padx+10, pady+2, padx+10, pady+4, color)
   end
   
   if (controller.down) then
      gui.line (padx+17, pady+1, padx+17, pady+2, color) -- #   #
      gui.line (padx+18, pady+2, padx+18, pady+3, color) -- ## ##
      gui.pixel(padx+19, pady+5, color)                  --  # #
      gui.line (padx+20, pady+2, padx+20, pady+3, color) --  ###
      gui.line (padx+21, pady+1, padx+21, pady+2, color) --   #
      gui.line (padx+18, pady+4, padx+20, pady+4, color)
   end
   
   if (controller.up) then
      gui.line (padx+25, pady+4, padx+25, pady+5, color) --   #
      gui.line (padx+26, pady+3, padx+26, pady+4, color) --  ###
      gui.pixel(padx+27, pady+1, color)                  --  # #
      gui.line (padx+28, pady+3, padx+28, pady+4, color) -- ## ##
      gui.line (padx+29, pady+4, padx+29, pady+5, color) -- #   #
      gui.line (padx+26, pady+2, padx+28, pady+2, color)
   end   
   
   if (controller.start) then                            -- ######
      gui.line (padx+33, pady+1, padx+38, pady+1, color) --   ##
      gui.line (padx+35, pady+2, padx+35, pady+5, color) --   ##
      gui.line (padx+36, pady+2, padx+36, pady+5, color) --   ##
   end
   
   if (controller.select) then
      gui.line (padx+42, pady+1, padx+44, pady+1, color) --  ###
      gui.line (padx+41, pady+2, padx+42, pady+2, color) -- ##
      gui.line (padx+42, pady+3, padx+44, pady+3, color) --  ###
      gui.line (padx+44, pady+4, padx+45, pady+4, color) --    ##
      gui.line (padx+42, pady+5, padx+44, pady+5, color) --  ###
   end  

   if (controller.B) then
      gui.line (padx+50, pady+1, padx+52, pady+1, color) -- ####
      gui.pixel(padx+53, pady+2, color)                  -- ##  #
      gui.line (padx+50, pady+3, padx+52, pady+3, color) -- ####
      gui.pixel(padx+53, pady+4, color)                  -- ##  #
      gui.line (padx+50, pady+5, padx+52, pady+5, color) -- ####
      gui.line (padx+49, pady+1, padx+49, pady+5, color)
      gui.line (padx+50, pady+2, padx+50, pady+4, color)
   end   
   
   if (controller.A) then
      gui.line (padx+58, pady+1, padx+60, pady+1, color) --  ###
      gui.line (padx+58, pady+3, padx+60, pady+3, color) -- ##  #
      gui.line (padx+57, pady+2, padx+57, pady+5, color) -- #####
      gui.line (padx+61, pady+2, padx+61, pady+5, color) -- ##  #
      gui.line (padx+58, pady+2, padx+58, pady+5, color) -- ##  #
   end 
 
end

function movable()

   keys = input.get()
   
   gui.box(x, y, x+62, y+15, "#00000080")
   
   if keys.xmouse >= ( x ) and keys.xmouse <= ( x + 62 ) then
      if keys.ymouse >= ( y-10 ) and keys.ymouse <= ( y + 25 ) then 
         if keys["leftclick"] then 
            x = keys.xmouse - 31
            y = keys.ymouse - 7
         end 
      end
   end 
   
   if x < 0 then x = 0 elseif x > 193 then x = 193 end
   if y < 0 then y = 0 elseif y > 216 then y = 216 end
   
   if (pad1 == 1) then drawpad(1, padx, pady) end 
   if (pad2 == 1) then drawpad(2, padx, pady) end 
    
end
 
gui.register(movable)
FCEU.frameadvance()