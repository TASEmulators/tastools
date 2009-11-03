------------------------------------------------------------------------------
-- Dialog class 
------------------------------------------------------------------------------
local ctrl = {
  nick = "dialog",
  parent = WIDGET,
  creation = "I",
  callback = {
    map_cb = "",
    unmap_cb = "",
    close_cb = "",
    show_cb = "n",
    move_cb = "nn",
    trayclick_cb = "nnn",
    dropfiles_cb = "snnn",
  }
}

function ctrl.createElement(class, arg)
   return Dialog(arg[1])
end

function ctrl.popup(handle, x, y)
  Popup(handle,x,y)
end

function ctrl.showxy(handle, x, y)
  return ShowXY(handle, x, y)
end

function ctrl.destroy(handle)
  return Destroy(handle)
end

iupRegisterWidget(ctrl)
iupSetClass(ctrl, "iup widget")
