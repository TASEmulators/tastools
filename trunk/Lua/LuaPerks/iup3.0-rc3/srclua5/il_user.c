/******************************************************************************
 * Automatically generated file (iuplua5). Please don't change anything.                *
 *****************************************************************************/

#include <stdlib.h>

#include <lua.h>
#include <lauxlib.h>

#include "iup.h"
#include "iuplua.h"
#include "il.h"


static int User(lua_State *L)
{
  Ihandle *ih = IupUser();
  iuplua_plugstate(L, ih);
  iuplua_pushihandle_raw(L, ih);
  return 1;
}

int iupuserlua_open(lua_State * L)
{
  iuplua_register(L, User, "User");


#ifdef IUPLUA_USELOH
#ifdef TEC_BIGENDIAN
#ifdef TEC_64
#include "loh/user_be64.loh"
#else
#include "loh/user_be32.loh"
#endif
#else
#ifdef TEC_64
#ifdef WIN64
#include "loh/user_le64w.loh"
#else
#include "loh/user_le64.loh"
#endif
#else
#include "loh/user.loh"
#endif
#endif
#else
  iuplua_dofile(L, "user.lua");
#endif

  return 0;
}

