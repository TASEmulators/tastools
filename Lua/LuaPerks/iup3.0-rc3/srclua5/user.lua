
------------------------------------------------------------------------------
-- User class 
------------------------------------------------------------------------------
local ctrl = {
  nick = "user",
  parent = WIDGET,
  creation = "",
  callback = {}
}

function ctrl.createElement(class, arg)
   return User()
end

iupRegisterWidget(ctrl)
iupSetClass(ctrl, "iup widget")
