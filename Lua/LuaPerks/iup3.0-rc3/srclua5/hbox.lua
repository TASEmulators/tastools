------------------------------------------------------------------------------
-- HBox class 
------------------------------------------------------------------------------
local ctrl = {
  nick = "hbox",
  parent = BOX,
  creation = "-",
  callback = {}
}

function ctrl.append(handle, elem)
  Append(handle, elem)
end

function ctrl.createElement(class, arg)
   return Hbox()
end

iupRegisterWidget(ctrl)
iupSetClass(ctrl, "iup widget")
