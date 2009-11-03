------------------------------------------------------------------------------
-- GLCanvas class 
------------------------------------------------------------------------------
local ctrl = {
  nick = "glcanvas",
  parent = WIDGET,
  creation = "-",
  funcname = "GLCanvas",
  include = "iupgl.h",
  callback = {
    action = "nn",
  },
  extrafuncs = 1,
  extracode = [[ 
int iupgllua_open(lua_State * L)
{
  if (iuplua_opencall_internal(L))
    IupGLCanvasOpen();

  iuplua_changeEnv(L);
  iupglcanvaslua_open(L);
  iuplua_returnEnv(L);
  return 0;
}

/* obligatory to use require"iupluagl" */
int luaopen_iupluagl(lua_State* L)
{
  return iupgllua_open(L);
}

/* obligatory to use require"iupluagl51" */
int luaopen_iupluagl51(lua_State* L)
{
  return iupgllua_open(L);
}

]]
}

function ctrl.createElement(class, arg)
   return GLCanvas()
end

iupRegisterWidget(ctrl)
iupSetClass(ctrl, "iup widget")
