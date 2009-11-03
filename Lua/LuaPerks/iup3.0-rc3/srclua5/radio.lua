------------------------------------------------------------------------------
-- Radio class 
------------------------------------------------------------------------------
local ctrl = {
  nick = "radio",
  parent = WIDGET,
  creation = "i",
  callback = {}
} 

function ctrl.CreateChildrenNames(obj)
  if obj then
    if obj.parent.parent == BOX then
      local i = 1
      while obj[i] do
        ctrl.CreateChildrenNames (obj[i])
        i = i+1
      end
    elseif obj.parent == IUPFRAME then
      ctrl.CreateChildrenNames (obj[1])
    else
      ihandle_setname(obj)
    end
  end
end

function ctrl.createElement(class, arg)
   ctrl.CreateChildrenNames(arg[1])
   return Radio(arg[1])
end
   
iupRegisterWidget(ctrl)
iupSetClass(ctrl, "iup widget")
