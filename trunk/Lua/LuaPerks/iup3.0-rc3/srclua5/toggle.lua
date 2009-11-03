------------------------------------------------------------------------------
-- Toggle class 
------------------------------------------------------------------------------
local ctrl = {
  nick = "toggle",
  parent = WIDGET,
  creation = "S-",
  callback = {
    action = "n",
  }
} 

function ctrl.createElement(class, arg)
  return Toggle(arg.title)
end
   
iupRegisterWidget(ctrl)
iupSetClass(ctrl, "iup widget")
