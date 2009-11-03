------------------------------------------------------------------------------
-- FileDlg class 
------------------------------------------------------------------------------
local ctrl = {
  nick = "filedlg",
  parent = WIDGET,
  creation = "",
  callback = {
    file_cb = "ss",
  },
  funcname = "FileDlg"
} 

function ctrl.popup(handle, x, y)
  Popup(handle,x,y)
end

function ctrl.destroy(handle)
  return Destroy(handle)
end

function ctrl.createElement(class, arg)
   return FileDlg()
end
   
iupRegisterWidget(ctrl)
iupSetClass(ctrl, "iup widget")

