/******************************************************************************
 * Automatically generated file (iuplua5). Please don't change anything.                *
 *****************************************************************************/

#include <stdlib.h>

#include <lua.h>
#include <lauxlib.h>

#include "iup.h"
#include "iuplua.h"
#include "iupcontrols.h"
#include "il.h"


static int dial_mousemove_cb(Ihandle *self, double p0)
{
  lua_State *L = iuplua_call_start(self, "mousemove_cb");
  lua_pushnumber(L, p0);
  return iuplua_call(L, 1);
}

static int Dial(lua_State *L)
{
  Ihandle *ih = IupDial((char *) luaL_checkstring(L, 1));
  iuplua_plugstate(L, ih);
  iuplua_pushihandle_raw(L, ih);
  return 1;
}

int iupdiallua_open(lua_State * L)
{
  iuplua_register(L, Dial, "Dial");

  iuplua_register_cb(L, "MOUSEMOVE_CB", (lua_CFunction)dial_mousemove_cb, "dial");

#ifdef IUPLUA_USELOH
#ifdef TEC_BIGENDIAN
#ifdef TEC_64
#include "loh/dial_be64.loh"
#else
#include "loh/dial_be32.loh"
#endif
#else
#ifdef TEC_64
#ifdef WIN64
#include "loh/dial_le64w.loh"
#else
#include "loh/dial_le64.loh"
#endif
#else
#include "loh/dial.loh"
#endif
#endif
#else
  iuplua_dofile(L, "dial.lua");
#endif

  return 0;
}

