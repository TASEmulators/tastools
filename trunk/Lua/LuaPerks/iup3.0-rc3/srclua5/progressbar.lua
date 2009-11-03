------------------------------------------------------------------------------
-- ProgressBar class 
------------------------------------------------------------------------------
local ctrl = {
  nick = "progressbar",
  parent = WIDGET,
  creation = "",
  funcname = "ProgressBar",
  callback = {}
} 

function ctrl.createElement(class, arg)
   return ProgressBar()
end
   
iupRegisterWidget(ctrl)
iupSetClass(ctrl, "iup widget")
