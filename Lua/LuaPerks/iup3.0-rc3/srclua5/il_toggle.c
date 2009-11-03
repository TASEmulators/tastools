/******************************************************************************
 * Automatically generated file (iuplua5). Please don't change anything.                *
 *****************************************************************************/

#include <stdlib.h>

#include <lua.h>
#include <lauxlib.h>

#include "iup.h"
#include "iuplua.h"
#include "il.h"


static int toggle_action(Ihandle *self, int p0)
{
  lua_State *L = iuplua_call_start(self, "action");
  lua_pushnumber(L, p0);
  return iuplua_call(L, 1);
}

static int Toggle(lua_State *L)
{
  Ihandle *ih = IupToggle((char *) luaL_optstring(L, 1, NULL), NULL);
  iuplua_plugstate(L, ih);
  iuplua_pushihandle_raw(L, ih);
  return 1;
}

int iuptogglelua_open(lua_State * L)
{
  iuplua_register(L, Toggle, "Toggle");

  iuplua_register_cb(L, "ACTION", (lua_CFunction)toggle_action, "toggle");

#ifdef IUPLUA_USELOH
#ifdef TEC_BIGENDIAN
#ifdef TEC_64
#include "loh/toggle_be64.loh"
#else
#include "loh/toggle_be32.loh"
#endif
#else
#ifdef TEC_64
#ifdef WIN64
#include "loh/toggle_le64w.loh"
#else
#include "loh/toggle_le64.loh"
#endif
#else
#include "loh/toggle.loh"
#endif
#endif
#else
  iuplua_dofile(L, "toggle.lua");
#endif

  return 0;
}

