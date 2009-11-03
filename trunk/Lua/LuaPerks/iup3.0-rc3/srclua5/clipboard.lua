------------------------------------------------------------------------------
-- Clipboard class 
------------------------------------------------------------------------------
local ctrl = {
  nick     = "clipboard",
  parent   = WIDGET,
  creation = "",
  callback = {
  },
} 

function ctrl.createElement(class, arg)
  return Clipboard()
end
   
iupRegisterWidget(ctrl)
iupSetClass(ctrl, "iup widget")
