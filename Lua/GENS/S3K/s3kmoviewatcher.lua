-- a script that attempts to make S3&K movies more interesting to watch
-- by fast-forwarding non-gameplay or offscreen or special stage sections
-- and disabling supersonic music.

-- TODO: instead of skipping offscreen sections, display them via camhack

-- KNOWN ISSUES: causes huge obvious desyncs if used at the same time as
--               certain other scripts such as the ghost script.

local lastmode = -1
local boringmodes = {0x34,0,4,0x4c,0x8c,0x28,0x48}
local function isboring()
  local mode = memory.readbyte(0xFFF600)
  if mode ~= lastmode then
    lastmode = mode
  else
    for _,m in ipairs(boringmodes) do
      if mode == m then return true end
    end
    if mode == 0x0c then
      if memory.readword(0xFFF7D2) ~= 0 or memory.readword(0xFFF7D4) ~= 0 then
        return true
      end
      if AND(memory.readbyte(0xFFB004), 0x80) == 0 then
        return true
      end
      if memory.readword(0xFFEE7c) == 346 and memory.readword(0xffee4e) == 1 then
        local xcam = memory.readword(0xFFEE78)
        return xcam >= 16960 and xcam <= 17468
      end
    end
  end
end

local x,y = 100,100
local dx,dy = 1,1

gui.register( function()

  if movie.playing() then
    
    if isboring() then

      sound.clear()
      local iter = 0
      local emeraldMessage
      if memory.readbyte(0xFFF600) == 0x34 then emeraldMessage = string.format("Collecting Emerald %d...", 1 + memory.readbyte(0xFFFFB0) + memory.readbyte(0xFFFFB1)) end

      while isboring() do
      
        gens.emulateframeinvisible()
        gens.emulateframeinvisible()
        iter = iter + 1
        
        if iter == 30 then
          iter = 0
          if emeraldMessage then gui.drawtext(x,y, emeraldMessage) end
          gui.drawtext(320-y*3/2,x*2/3, "Please Wait...")
          x = x + dx
          y = y + dy
          if x < 0 or x >= 320 then
            dx = -dx
            x = x + (2*dx)
          end
          if y < 0 or y >= 240 then
            dy = -dy
            y = y + (2*dy)
          end
          return
        end
        if iter == 15 then gens.wait() end

      end
    end
  end
end)

memory.register(0xa01c0a, 2, function()
  if memory.readbyte(0xa01c0b) == 0x2C then
    if memory.readbyte(0xa01c0a) ~= 0x9F then
      memory.writebyte(0xa01c0b, 1+memory.readbyte(0xFFEE4F)+2*memory.readbyte(0xFFEE4E))
    else
      memory.writebyte(0xa01c0b, 0)
    end
    memory.writelong(0xa01c22, 0x0)
  end
end)
