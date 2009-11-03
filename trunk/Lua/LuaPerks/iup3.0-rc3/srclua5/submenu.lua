------------------------------------------------------------------------------
-- Submenu class 
------------------------------------------------------------------------------
local ctrl = {
  nick = "submenu",
  parent = WIDGET,
  creation = "Si",
  callback = {
--    open_cb = "",       -- already registered by the menu
--    menuclose_cb = "",  -- already registered by the menu
  }
} 

function ctrl.createElement(class, arg)
  return Submenu(arg.title, arg[1])
end
   
iupRegisterWidget(ctrl)
iupSetClass(ctrl, "iup widget")
