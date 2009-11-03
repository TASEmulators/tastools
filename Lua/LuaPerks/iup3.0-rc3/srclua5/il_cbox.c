/******************************************************************************
 * Automatically generated file (iuplua5). Please don't change anything.                *
 *****************************************************************************/

#include <stdlib.h>

#include <lua.h>
#include <lauxlib.h>

#include "iup.h"
#include "iuplua.h"
#include "il.h"


static int Cboxv(lua_State *L)
{
  Ihandle **hlist = iuplua_checkihandle_array(L, 1);
  Ihandle *h = IupCboxv(hlist);
  iuplua_plugstate(L, h);
  iuplua_pushihandle_raw(L, h);
  free(hlist);
  return 1;
}
  int iupcboxlua_open(lua_State * L)
{
  iuplua_register(L, Cboxv, "Cboxv");


#ifdef IUPLUA_USELOH
#ifdef TEC_BIGENDIAN
#ifdef TEC_64
#include "loh/cbox_be64.loh"
#else
#include "loh/cbox_be32.loh"
#endif
#else
#ifdef TEC_64
#ifdef WIN64
#include "loh/cbox_le64w.loh"
#else
#include "loh/cbox_le64.loh"
#endif
#else
#include "loh/cbox.loh"
#endif
#endif
#else
  iuplua_dofile(L, "cbox.lua");
#endif

  return 0;
}

