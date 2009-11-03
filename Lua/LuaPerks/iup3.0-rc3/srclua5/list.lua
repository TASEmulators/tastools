------------------------------------------------------------------------------
-- List class 
------------------------------------------------------------------------------
local ctrl = {
  nick = "list",
  parent = WIDGET,
  creation = "-",
  callback = {
     action = "snn", 
     multiselect_cb = "s",
     edit_cb = "ns",
     dropdown_cb = "n",
     dblclick_cb = "ns",
   }
} 

function ctrl.createElement(class, arg)
   return List()
end
   
iupRegisterWidget(ctrl)
iupSetClass(ctrl, "iup widget")
