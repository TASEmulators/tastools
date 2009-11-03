/******************************************************************************
 * Automatically generated file (iuplua5). Please don't change anything.                *
 *****************************************************************************/

#include <stdlib.h>

#include <lua.h>
#include <lauxlib.h>

#include "iup.h"
#include "iuplua.h"
#include "iupgl.h"
#include "il.h"


static int glcanvas_action(Ihandle *self, int p0, int p1)
{
  lua_State *L = iuplua_call_start(self, "action");
  lua_pushnumber(L, p0);
  lua_pushnumber(L, p1);
  return iuplua_call(L, 2);
}

static int GLCanvas(lua_State *L)
{
  Ihandle *ih = IupGLCanvas(NULL);
  iuplua_plugstate(L, ih);
  iuplua_pushihandle_raw(L, ih);
  return 1;
}

void iuplua_glcanvasfuncs_open(lua_State *L);

int iupglcanvaslua_open(lua_State * L)
{
  iuplua_register(L, GLCanvas, "GLCanvas");

  iuplua_register_cb(L, "ACTION", (lua_CFunction)glcanvas_action, "glcanvas");

  iuplua_glcanvasfuncs_open(L);

#ifdef IUPLUA_USELOH
#ifdef TEC_BIGENDIAN
#ifdef TEC_64
#include "loh/glcanvas_be64.loh"
#else
#include "loh/glcanvas_be32.loh"
#endif
#else
#ifdef TEC_64
#ifdef WIN64
#include "loh/glcanvas_le64w.loh"
#else
#include "loh/glcanvas_le64.loh"
#endif
#else
#include "loh/glcanvas.loh"
#endif
#endif
#else
  iuplua_dofile(L, "glcanvas.lua");
#endif

  return 0;
}

 
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

