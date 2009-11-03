/******************************************************************************
 * Automatically generated file (iuplua5). Please don't change anything.                *
 *****************************************************************************/

#include <stdlib.h>

#include <lua.h>
#include <lauxlib.h>

#include "iup.h"
#include "iuplua.h"
#include "il.h"


static int FontDlg(lua_State *L)
{
  Ihandle *ih = IupFontDlg();
  iuplua_plugstate(L, ih);
  iuplua_pushihandle_raw(L, ih);
  return 1;
}

int iupfontdlglua_open(lua_State * L)
{
  iuplua_register(L, FontDlg, "FontDlg");


#ifdef IUPLUA_USELOH
#ifdef TEC_BIGENDIAN
#ifdef TEC_64
#include "loh/fontdlg_be64.loh"
#else
#include "loh/fontdlg_be32.loh"
#endif
#else
#ifdef TEC_64
#ifdef WIN64
#include "loh/fontdlg_le64w.loh"
#else
#include "loh/fontdlg_le64.loh"
#endif
#else
#include "loh/fontdlg.loh"
#endif
#endif
#else
  iuplua_dofile(L, "fontdlg.lua");
#endif

  return 0;
}

