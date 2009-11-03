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


static int Gauge(lua_State *L)
{
  Ihandle *ih = IupGauge();
  iuplua_plugstate(L, ih);
  iuplua_pushihandle_raw(L, ih);
  return 1;
}

int iupgaugelua_open(lua_State * L)
{
  iuplua_register(L, Gauge, "Gauge");


#ifdef IUPLUA_USELOH
#ifdef TEC_BIGENDIAN
#ifdef TEC_64
#include "loh/gauge_be64.loh"
#else
#include "loh/gauge_be32.loh"
#endif
#else
#ifdef TEC_64
#ifdef WIN64
#include "loh/gauge_le64w.loh"
#else
#include "loh/gauge_le64.loh"
#endif
#else
#include "loh/gauge.loh"
#endif
#endif
#else
  iuplua_dofile(L, "gauge.lua");
#endif

  return 0;
}

