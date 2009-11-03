/******************************************************************************
 * Automatically generated file (iuplua5). Please don't change anything.                *
 *****************************************************************************/

#include <stdlib.h>

#include <lua.h>
#include <lauxlib.h>

#include "iup.h"
#include "iuplua.h"
#include "il.h"


static int filedlg_file_cb(Ihandle *self, char * p0, char * p1)
{
  lua_State *L = iuplua_call_start(self, "file_cb");
  lua_pushstring(L, p0);
  lua_pushstring(L, p1);
  return iuplua_call(L, 2);
}

static int FileDlg(lua_State *L)
{
  Ihandle *ih = IupFileDlg();
  iuplua_plugstate(L, ih);
  iuplua_pushihandle_raw(L, ih);
  return 1;
}

int iupfiledlglua_open(lua_State * L)
{
  iuplua_register(L, FileDlg, "FileDlg");

  iuplua_register_cb(L, "FILE_CB", (lua_CFunction)filedlg_file_cb, NULL);

#ifdef IUPLUA_USELOH
#ifdef TEC_BIGENDIAN
#ifdef TEC_64
#include "loh/filedlg_be64.loh"
#else
#include "loh/filedlg_be32.loh"
#endif
#else
#ifdef TEC_64
#ifdef WIN64
#include "loh/filedlg_le64w.loh"
#else
#include "loh/filedlg_le64.loh"
#endif
#else
#include "loh/filedlg.loh"
#endif
#endif
#else
  iuplua_dofile(L, "filedlg.lua");
#endif

  return 0;
}

