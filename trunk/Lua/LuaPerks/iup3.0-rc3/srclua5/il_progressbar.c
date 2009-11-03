/******************************************************************************
 * Automatically generated file (iuplua5). Please don't change anything.                *
 *****************************************************************************/

#include <stdlib.h>

#include <lua.h>
#include <lauxlib.h>

#include "iup.h"
#include "iuplua.h"
#include "il.h"


static int ProgressBar(lua_State *L)
{
  Ihandle *ih = IupProgressBar();
  iuplua_plugstate(L, ih);
  iuplua_pushihandle_raw(L, ih);
  return 1;
}

int iupprogressbarlua_open(lua_State * L)
{
  iuplua_register(L, ProgressBar, "ProgressBar");


#ifdef IUPLUA_USELOH
#ifdef TEC_BIGENDIAN
#ifdef TEC_64
#include "loh/progressbar_be64.loh"
#else
#include "loh/progressbar_be32.loh"
#endif
#else
#ifdef TEC_64
#ifdef WIN64
#include "loh/progressbar_le64w.loh"
#else
#include "loh/progressbar_le64.loh"
#endif
#else
#include "loh/progressbar.loh"
#endif
#endif
#else
  iuplua_dofile(L, "progressbar.lua");
#endif

  return 0;
}

