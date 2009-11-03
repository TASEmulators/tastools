------------------------------------------------------------------------------
-- Label class 
------------------------------------------------------------------------------
local ctrl = {
  nick = "label",
  parent = WIDGET,
  creation = "S",
  callback = {}
}

function ctrl.createElement(class, arg)
   return Label(arg.title)
end

iupRegisterWidget(ctrl)
iupSetClass(ctrl, "iup widget")
