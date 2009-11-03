------------------------------------------------------------------------------
-- MessageDlg class 
------------------------------------------------------------------------------
local ctrl = {
  nick = "messagedlg",
  parent = WIDGET,
  creation = "",
  funcname = "MessageDlg",
  callback = {}
} 

function ctrl.popup(handle, x, y)
  Popup(handle,x,y)
end

function ctrl.destroy(handle)
  return Destroy(handle)
end

function ctrl.createElement(class, arg)
   return MessageDlg()
end
   
iupRegisterWidget(ctrl)
iupSetClass(ctrl, "iup widget")

