/******************************************************************************
 * Automatically generated file (iuplua5). Please don't change anything.                *
 *****************************************************************************/

#include <stdlib.h>

#include <lua.h>
#include <lauxlib.h>

#include "iup.h"
#include "iuplua.h"
#include "il.h"


static int Label(lua_State *L)
{
  Ihandle *ih = IupLabel((char *) luaL_optstring(L, 1, NULL));
  iuplua_plugstate(L, ih);
  iuplua_pushihandle_raw(L, ih);
  return 1;
}

int iuplabellua_open(lua_State * L)
{
  iuplua_register(L, Label, "Label");


#ifdef IUPLUA_USELOH
#ifdef TEC_BIGENDIAN
#ifdef TEC_64
#include "loh/label_be64.loh"
#else
#include "loh/label_be32.loh"
#endif
#else
#ifdef TEC_64
#ifdef WIN64
#include "loh/label_le64w.loh"
#else
#include "loh/label_le64.loh"
#endif
#else
#include "loh/label.loh"
#endif
#endif
#else
  iuplua_dofile(L, "label.lua");
#endif

  return 0;
}

