------------------------------------------------------------------------------
-- Menu class 
------------------------------------------------------------------------------
local ctrl = {
  nick = "menu",
  parent = BOX,
  creation = "-",
  callback = {
    open_cb = "",
    menuclose_cb = "",
  }
}

function ctrl.popup(handle, x, y)
  Popup(handle, x, y)
end

function ctrl.append(handle, elem)
  Append(handle, elem)
end

function ctrl.createElement(class, arg)
  local n = #arg
  for i=1,n do
    if type(arg[i]) == "table" then 
      itemarg = {}
      for u,v in pairs(arg[i]) do
        if type(u) ~= "number" then
          itemarg[u] = v
        end
      end
      if type(arg[i][1]) == "string" and (type(arg[i][2]) == "function" or type(arg[i][2]) == "string") then
        itemarg.title = arg[i][1]
        itemarg.action = arg[i][2]
        arg[i] = item(itemarg)
      elseif type(arg[i][1]) == "string" and type(arg[i][2]) == "userdata" then
        itemarg[1] = arg[i][2]
        itemarg.title = arg[i][1]
        arg[i] = submenu(itemarg)
      end
    end
  end
   return Menu()
end

function ctrl.showxy(handle, x, y)
  return ShowXY(handle, x, y)
end

function ctrl.destroy(handle)
  return Destroy(handle)
end

iupRegisterWidget(ctrl)
iupSetClass(ctrl, "iup widget")
