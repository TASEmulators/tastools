------------------------------------------------------------------------------
-- Canvas class 
------------------------------------------------------------------------------
local ctrl = {
  nick = "canvas",
  parent = WIDGET,
  creation = "-",
  callback = {
    action = "ff",
    button_cb = "nnnns",
    enterwindow_cb = "",
    leavewindow_cb = "",
    motion_cb = "nns",
    resize_cb = "nn",
    scroll_cb = "nff",
    keypress_cb = "nn",
    wom_cb = "n",
    wheel_cb = "fnns",
    mdiactivate_cb = "",
    focus_cb = "n",
  }
}

function ctrl.createElement(class, arg)
   return Canvas()
end

iupRegisterWidget(ctrl)
iupSetClass(ctrl, "iup widget")
