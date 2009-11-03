/** \file
 * \brief IupGetColor bindig to Lua 5.
 *
 * See Copyright Notice in "iup.h"
 */
 
#include <stdlib.h>

#include <lua.h>
#include <lauxlib.h>

#include "iup.h"

#include "iuplua.h"
#include "il.h"
#include "il_controls.h"

 
static int GetColor(lua_State *L)
{
  int x = (int)luaL_checknumber(L,1);
  int y = (int)luaL_checknumber(L,2);
  unsigned char r = (unsigned char) luaL_optnumber(L,3,0);
  unsigned char g = (unsigned char) luaL_optnumber(L,4,0);
  unsigned char b = (unsigned char) luaL_optnumber(L,5,0);
  int ret = IupGetColor(x,y,&r,&g,&b);
  if (ret) 
  {
    lua_pushnumber(L, (int) r);
    lua_pushnumber(L, (int) g);
    lua_pushnumber(L, (int) b);
    return 3;
  } 
  else 
  {
    lua_pushnil(L);
    return 1;
  }
}
int iupgclua_open(lua_State * L)
{
  lua_pushcfunction(L, GetColor);
  lua_setglobal(L, "GetColor");
  return 0;
}

