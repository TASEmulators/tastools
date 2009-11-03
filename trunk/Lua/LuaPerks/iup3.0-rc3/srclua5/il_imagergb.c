/******************************************************************************
 * Automatically generated file (iuplua5). Please don't change anything.                *
 *****************************************************************************/

#include <stdlib.h>

#include <lua.h>
#include <lauxlib.h>

#include "iup.h"
#include "iuplua.h"
#include "il.h"


 
static int ImageRGB(lua_State *L)
{
  int w = luaL_checkint(L, 1);
  int h = luaL_checkint(L, 2);
  unsigned char *pixels = iuplua_checkuchar_array(L, 3, w*h*3);
  Ihandle *ih = IupImageRGB(w, h, pixels);
  iuplua_plugstate(L, ih);
  iuplua_pushihandle_raw(L, ih);
  free(pixels);
  return 1;
}
 
int iupimagergblua_open(lua_State * L)
{
  iuplua_register(L, ImageRGB, "ImageRGB");


#ifdef IUPLUA_USELOH
#ifdef TEC_BIGENDIAN
#ifdef TEC_64
#include "loh/imagergb_be64.loh"
#else
#include "loh/imagergb_be32.loh"
#endif
#else
#ifdef TEC_64
#ifdef WIN64
#include "loh/imagergb_le64w.loh"
#else
#include "loh/imagergb_le64.loh"
#endif
#else
#include "loh/imagergb.loh"
#endif
#endif
#else
  iuplua_dofile(L, "imagergb.lua");
#endif

  return 0;
}

