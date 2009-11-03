/******************************************************************************
 * Automatically generated file (iuplua5). Please don't change anything.                *
 *****************************************************************************/

#include <stdlib.h>

#include <lua.h>
#include <lauxlib.h>

#include "iup.h"
#include "iuplua.h"
#include "il.h"


static int multiline_action(Ihandle *self, int p0, char * p1)
{
  lua_State *L = iuplua_call_start(self, "action");
  lua_pushnumber(L, p0);
  lua_pushstring(L, p1);
  return iuplua_call(L, 2);
}

static int MultiLine(lua_State *L)
{
  Ihandle *ih = IupMultiLine(NULL);
  iuplua_plugstate(L, ih);
  iuplua_pushihandle_raw(L, ih);
  return 1;
}

int iupmultilinelua_open(lua_State * L)
{
  iuplua_register(L, MultiLine, "MultiLine");

  iuplua_register_cb(L, "ACTION", (lua_CFunction)multiline_action, "multiline");

#ifdef IUPLUA_USELOH
#ifdef TEC_BIGENDIAN
#ifdef TEC_64
#include "loh/multiline_be64.loh"
#else
#include "loh/multiline_be32.loh"
#endif
#else
#ifdef TEC_64
#ifdef WIN64
#include "loh/multiline_le64w.loh"
#else
#include "loh/multiline_le64.loh"
#endif
#else
#include "loh/multiline.loh"
#endif
#endif
#else
  iuplua_dofile(L, "multiline.lua");
#endif

  return 0;
}

