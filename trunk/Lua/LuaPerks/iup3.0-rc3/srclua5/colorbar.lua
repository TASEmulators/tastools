------------------------------------------------------------------------------
-- Colorbar class 
------------------------------------------------------------------------------
local ctrl = {
  nick = "colorbar",
  parent = WIDGET,
  creation = "",
  callback = {
    select_cb = "nn",
    cell_cb = {"n", ret = "s"},
    switch_cb = "nn",
    extended_cb = "n",
   },
  funcname = "Colorbar",
  include = "iupcontrols.h",
}

PRIMARY = -1
SECONDARY = -2

function ctrl.createElement(class, arg)
   return Colorbar(arg.action)
end

iupRegisterWidget(ctrl)
iupSetClass(ctrl, "iup widget")
