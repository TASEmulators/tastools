------------------------------------------------------------------------------
-- Dial class 
------------------------------------------------------------------------------
local ctrl = {
  nick = "dial",
  parent = WIDGET,
  creation = "s",
  callback = {
     mousemove_cb = "d",       -- already registered by the val, but has a name conflict
--     button_press_cb = "d",    -- already registered by the val
--     button_release_cb = "d",  -- already registered by the val
  },
  include = "iupcontrols.h",
}

function ctrl.createElement(class, arg)
   return Dial(arg[1])
end

iupRegisterWidget(ctrl)
iupSetClass(ctrl, "iup widget")
