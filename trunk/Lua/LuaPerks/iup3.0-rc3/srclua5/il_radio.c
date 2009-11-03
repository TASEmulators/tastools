/******************************************************************************
 * Automatically generated file (iuplua5). Please don't change anything.                *
 *****************************************************************************/

#include <stdlib.h>

#include <lua.h>
#include <lauxlib.h>

#include "iup.h"
#include "iuplua.h"
#include "il.h"


static int Radio(lua_State *L)
{
  Ihandle *ih = IupRadio(iuplua_checkihandle(L, 1));
  iuplua_plugstate(L, ih);
  iuplua_pushihandle_raw(L, ih);
  return 1;
}

int iupradiolua_open(lua_State * L)
{
  iuplua_register(L, Radio, "Radio");


#ifdef IUPLUA_USELOH
#ifdef TEC_BIGENDIAN
#ifdef TEC_64
#include "loh/radio_be64.loh"
#else
#include "loh/radio_be32.loh"
#endif
#else
#ifdef TEC_64
#ifdef WIN64
#include "loh/radio_le64w.loh"
#else
#include "loh/radio_le64.loh"
#endif
#else
#include "loh/radio.loh"
#endif
#endif
#else
  iuplua_dofile(L, "radio.lua");
#endif

  return 0;
}

