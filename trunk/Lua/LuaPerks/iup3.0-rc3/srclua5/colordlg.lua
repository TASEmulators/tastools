------------------------------------------------------------------------------
-- ColorDlg class 
------------------------------------------------------------------------------
local ctrl = {
  nick = "colordlg",
  parent = WIDGET,
  creation = "",
  funcname = "ColorDlg",
  callback = {}
} 

function ctrl.popup(handle, x, y)
  Popup(handle,x,y)
end

function ctrl.destroy(handle)
  return Destroy(handle)
end

function ctrl.createElement(class, arg)
   return ColorDlg()
end
   
iupRegisterWidget(ctrl)
iupSetClass(ctrl, "iup widget")

