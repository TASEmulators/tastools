------------------------------------------------------------------------------
-- ColorBrowser class 
------------------------------------------------------------------------------
local ctrl = {
  nick = "colorbrowser",
  parent = WIDGET,
  creation = "",
  callback = {
    drag_cb = "ccc",
    change_cb = "ccc",
  },
  funcname = "ColorBrowser",
  include = "iupcontrols.h",
}

function ctrl.createElement(class, arg)
   return ColorBrowser(arg.action)
end

iupRegisterWidget(ctrl)
iupSetClass(ctrl, "iup widget")
