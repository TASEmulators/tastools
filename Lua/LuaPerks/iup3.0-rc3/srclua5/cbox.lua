------------------------------------------------------------------------------
-- Cbox class 
------------------------------------------------------------------------------
local ctrl = {
  nick = "cbox",
  parent = WIDGET,
  creation = "v",
  callback = {},
  funcname = "Cboxv",
  createfunc = [[
static int Cboxv(lua_State *L)
{
  Ihandle **hlist = iuplua_checkihandle_array(L, 1);
  Ihandle *h = IupCboxv(hlist);
  iuplua_plugstate(L, h);
  iuplua_pushihandle_raw(L, h);
  free(hlist);
  return 1;
}
  ]],
}

function ctrl.createElement(class, arg)
  return Cboxv(arg)
end

iupRegisterWidget(ctrl)
iupSetClass(ctrl, "iup widget")
