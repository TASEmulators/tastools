------------------------------------------------------------------------------
-- Button class 
------------------------------------------------------------------------------
local ctrl = {
  nick = "button",
  parent = WIDGET,
  creation = "S-",
  callback = {
    action = "", 
  }
} 

function ctrl.createElement(class, arg)
  return Button(arg.title)
end

iupRegisterWidget(ctrl)
iupSetClass(ctrl, "iup widget")
