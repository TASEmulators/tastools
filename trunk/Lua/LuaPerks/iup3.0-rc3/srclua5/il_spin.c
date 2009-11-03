/******************************************************************************
 * Automatically generated file (iuplua5). Please don't change anything.                *
 *****************************************************************************/

#include <stdlib.h>

#include <lua.h>
#include <lauxlib.h>

#include "iup.h"
#include "iuplua.h"
#include "il.h"


static int spin_spin_cb(Ihandle *self, int p0)
{
  lua_State *L = iuplua_call_start(self, "spin_cb");
  lua_pushnumber(L, p0);
  return iuplua_call(L, 1);
}

static int Spin(lua_State *L)
{
  Ihandle *ih = IupSpin();
  iuplua_plugstate(L, ih);
  iuplua_pushihandle_raw(L, ih);
  return 1;
}

int iupspinlua_open(lua_State * L)
{
  iuplua_register(L, Spin, "Spin");

  iuplua_register_cb(L, "SPIN_CB", (lua_CFunction)spin_spin_cb, NULL);

#ifdef IUPLUA_USELOH
#ifdef TEC_BIGENDIAN
#ifdef TEC_64
#include "loh/spin_be64.loh"
#else
#include "loh/spin_be32.loh"
#endif
#else
#ifdef TEC_64
#ifdef WIN64
#include "loh/spin_le64w.loh"
#else
#include "loh/spin_le64.loh"
#endif
#else
#include "loh/spin.loh"
#endif
#endif
#else
  iuplua_dofile(L, "spin.lua");
#endif

  return 0;
}

