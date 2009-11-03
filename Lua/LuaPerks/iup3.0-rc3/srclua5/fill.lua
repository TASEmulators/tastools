------------------------------------------------------------------------------
-- Fill class 
------------------------------------------------------------------------------
local ctrl = {
  nick = "fill",
  parent = WIDGET,
  creation = "",
  callback = {}
}

function ctrl.createElement(class, arg)
   return Fill()
end

iupRegisterWidget(ctrl)
iupSetClass(ctrl, "iup widget")
