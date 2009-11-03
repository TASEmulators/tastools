------------------------------------------------------------------------------
-- Frame class 
------------------------------------------------------------------------------
local ctrl = {
  nick = "frame",
  parent = WIDGET,
  creation = "i",
  callback = {}
}

function ctrl.createElement(class, arg)
   return Frame(arg[1])
end

iupRegisterWidget(ctrl)
iupSetClass(ctrl, "iup widget")
