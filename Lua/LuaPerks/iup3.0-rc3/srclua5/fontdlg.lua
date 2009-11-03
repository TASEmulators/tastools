------------------------------------------------------------------------------
-- FontDlg class 
------------------------------------------------------------------------------
local ctrl = {
  nick = "fontdlg",
  parent = WIDGET,
  creation = "",
  funcname = "FontDlg",
  callback = {}
} 

function ctrl.popup(handle, x, y)
  Popup(handle,x,y)
end

function ctrl.destroy(handle)
  return Destroy(handle)
end

function ctrl.createElement(class, arg)
   return FontDlg()
end
   
iupRegisterWidget(ctrl)
iupSetClass(ctrl, "iup widget")

