/******************************************************************************
 * Automatically generated file (iuplua5). Please don't change anything.                *
 *****************************************************************************/

#include <stdlib.h>

#include <lua.h>
#include <lauxlib.h>

#include "iup.h"
#include "iuplua.h"
#include "iupole.h"
#include "il.h"


static int OleControl(lua_State *L)
{
  Ihandle *ih = IupOleControl((char *) luaL_checkstring(L, 1));
  iuplua_plugstate(L, ih);
  iuplua_pushihandle_raw(L, ih);
  return 1;
}

int iupolecontrollua_open(lua_State * L)
{
  iuplua_register(L, OleControl, "OleControl");


#ifdef IUPLUA_USELOH
#ifdef TEC_BIGENDIAN
#ifdef TEC_64
#include "loh/olecontrol_be64.loh"
#else
#include "loh/olecontrol_be32.loh"
#endif
#else
#ifdef TEC_64
#ifdef WIN64
#include "loh/olecontrol_le64w.loh"
#else
#include "loh/olecontrol_le64.loh"
#endif
#else
#include "loh/olecontrol.loh"
#endif
#endif
#else
  iuplua_dofile(L, "olecontrol.lua");
#endif

  return 0;
}

 
int iupolelua_open(lua_State* L)
{
  if (iuplua_opencall_internal(L))
    IupOleControlOpen();
    
  iuplua_changeEnv(L);
  iupolecontrollua_open(L);
  iuplua_returnEnv(L);
  return 0;
}

/* obligatory to use require"iupluaole" */
int luaopen_iupluaole(lua_State* L)
{
  return iupolelua_open(L);
}

/* obligatory to use require"iupluaole51" */
int luaopen_iupluaole51(lua_State* L)
{
  return iupolelua_open(L);
}

