/******************************************************************************
 * Automatically generated file (iuplua5). Please don't change anything.                *
 *****************************************************************************/

#include <stdlib.h>

#include <lua.h>
#include <lauxlib.h>

#include "iup.h"
#include "iuplua.h"
#include "il.h"


static int MessageDlg(lua_State *L)
{
  Ihandle *ih = IupMessageDlg();
  iuplua_plugstate(L, ih);
  iuplua_pushihandle_raw(L, ih);
  return 1;
}

int iupmessagedlglua_open(lua_State * L)
{
  iuplua_register(L, MessageDlg, "MessageDlg");


#ifdef IUPLUA_USELOH
#ifdef TEC_BIGENDIAN
#ifdef TEC_64
#include "loh/messagedlg_be64.loh"
#else
#include "loh/messagedlg_be32.loh"
#endif
#else
#ifdef TEC_64
#ifdef WIN64
#include "loh/messagedlg_le64w.loh"
#else
#include "loh/messagedlg_le64.loh"
#endif
#else
#include "loh/messagedlg.loh"
#endif
#endif
#else
  iuplua_dofile(L, "messagedlg.lua");
#endif

  return 0;
}

