
------------------------------------------------------------------------------
-- Normalizer class 
------------------------------------------------------------------------------
local ctrl = {
  nick = "normalizer",
  parent = WIDGET,
  creation = "-",
  callback = {}
}

function ctrl.setAttributes(object, arg)
  local handle = rawget(object, "handle")
  local n = #arg
  for i = 1, n do
    if iupGetClass(arg[i]) == "iup handle" then 
      object.addcontrol = arg[i]
    end
  end
  WIDGET.setAttributes(object, arg)
end

function ctrl.createElement(class, arg)
   return Normalizer()
end

iupRegisterWidget(ctrl)
iupSetClass(ctrl, "iup widget")
