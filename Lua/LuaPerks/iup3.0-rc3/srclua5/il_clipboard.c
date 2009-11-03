/******************************************************************************
 * Automatically generated file (iuplua5). Please don't change anything.                *
 *****************************************************************************/

#include <stdlib.h>

#include <lua.h>
#include <lauxlib.h>

#include "iup.h"
#include "iuplua.h"
#include "il.h"


static int Clipboard(lua_State *L)
{
  Ihandle *ih = IupClipboard();
  iuplua_plugstate(L, ih);
  iuplua_pushihandle_raw(L, ih);
  return 1;
}

int iupclipboardlua_open(lua_State * L)
{
  iuplua_register(L, Clipboard, "Clipboard");


#ifdef IUPLUA_USELOH
#ifdef TEC_BIGENDIAN
#ifdef TEC_64
#include "loh/clipboard_be64.loh"
#else
#include "loh/clipboard_be32.loh"
#endif
#else
#ifdef TEC_64
#ifdef WIN64
#include "loh/clipboard_le64w.loh"
#else
#include "loh/clipboard_le64.loh"
#endif
#else
#include "loh/clipboard.loh"
#endif
#endif
#else
  iuplua_dofile(L, "clipboard.lua");
#endif

  return 0;
}

