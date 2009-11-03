/******************************************************************************
 * Automatically generated file (iuplua5). Please don't change anything.                *
 *****************************************************************************/

#include <stdlib.h>

#include <lua.h>
#include <lauxlib.h>

#include "iup.h"
#include "iuplua.h"
#include "il.h"


static int text_caret_cb(Ihandle *self, int p0, int p1, int p2)
{
  lua_State *L = iuplua_call_start(self, "caret_cb");
  lua_pushnumber(L, p0);
  lua_pushnumber(L, p1);
  lua_pushnumber(L, p2);
  return iuplua_call(L, 3);
}

static int text_action(Ihandle *self, int p0, char * p1)
{
  lua_State *L = iuplua_call_start(self, "action");
  lua_pushnumber(L, p0);
  lua_pushstring(L, p1);
  return iuplua_call(L, 2);
}

static int text_valuechanged_cb(Ihandle *self)
{
  lua_State *L = iuplua_call_start(self, "valuechanged_cb");
  return iuplua_call(L, 0);
}

static int Text(lua_State *L)
{
  Ihandle *ih = IupText(NULL);
  iuplua_plugstate(L, ih);
  iuplua_pushihandle_raw(L, ih);
  return 1;
}

int iuptextlua_open(lua_State * L)
{
  iuplua_register(L, Text, "Text");

  iuplua_register_cb(L, "CARET_CB", (lua_CFunction)text_caret_cb, NULL);
  iuplua_register_cb(L, "ACTION", (lua_CFunction)text_action, "text");
  iuplua_register_cb(L, "VALUECHANGED_CB", (lua_CFunction)text_valuechanged_cb, NULL);

#ifdef IUPLUA_USELOH
#ifdef TEC_BIGENDIAN
#ifdef TEC_64
#include "loh/text_be64.loh"
#else
#include "loh/text_be32.loh"
#endif
#else
#ifdef TEC_64
#ifdef WIN64
#include "loh/text_le64w.loh"
#else
#include "loh/text_le64.loh"
#endif
#else
#include "loh/text.loh"
#endif
#endif
#else
  iuplua_dofile(L, "text.lua");
#endif

  return 0;
}

