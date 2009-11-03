/******************************************************************************
 * Automatically generated file (iuplua5). Please don't change anything.                *
 *****************************************************************************/

#include <stdlib.h>

#include <lua.h>
#include <lauxlib.h>

#include "iup.h"
#include "iuplua.h"
#include "il.h"


 
static int Image (lua_State * L)
{
  int w, h, c, num_colors;
  unsigned char *pixels;
  Ihandle* ih;
  char str[20];

  if (lua_istable(L, 1))
  {
    int i, j;

    /* get the number of lines */
    h = luaL_getn(L, 1);  

    /* get the number of columns of the first line */
    lua_pushnumber(L, 1);
    lua_gettable(L, 1);
    w = luaL_getn(L, -1);  
    lua_pop(L, 1);
    
    pixels = (unsigned char *) malloc (h*w);

    for (i=1; i<=h; i++)
    {
      lua_pushnumber(L, i);
      lua_gettable(L, 1);
      for (j=1; j<=w; j++)
      {
        int idx = (i-1)*w+(j-1);
        lua_pushnumber(L, j);
        lua_gettable(L, -2);
        pixels[idx] = (unsigned char)lua_tonumber(L, -1);
        lua_pop(L, 1);
      }
      lua_pop(L, 1);
    }
    
    ih = IupImage(w,h,pixels);  
    free(pixels);

    num_colors = luaL_getn(L, 2);
    num_colors = num_colors>255? 255: num_colors;
    for(c=1; c<=num_colors; c++)
    {
      lua_rawgeti(L, 2, c);
      sprintf(str, "%d", c);
      IupStoreAttribute(ih, str, lua_tostring(L,-1));
      lua_pop(L, 1);
    }
  }
  else
  {
    w = luaL_checkint(L, 1);
    h = luaL_checkint(L, 2);
    pixels = iuplua_checkuchar_array(L, 3, w*h);
    ih = IupImage(w, h, pixels);
    free(pixels);

    num_colors = luaL_getn(L, 4);
    num_colors = num_colors>256? 256: num_colors;
    for(c=1; c<=num_colors; c++)
    {
      lua_rawgeti(L, 4, c);
      sprintf(str, "%d", c-1);
      IupStoreAttribute(ih, str, lua_tostring(L,-1));
      lua_pop(L, 1);
    }
  }

  iuplua_plugstate(L, ih);
  iuplua_pushihandle_raw(L, ih);
  return 1;
} 
 
int iupimagelua_open(lua_State * L)
{
  iuplua_register(L, Image, "Image");


#ifdef IUPLUA_USELOH
#ifdef TEC_BIGENDIAN
#ifdef TEC_64
#include "loh/image_be64.loh"
#else
#include "loh/image_be32.loh"
#endif
#else
#ifdef TEC_64
#ifdef WIN64
#include "loh/image_le64w.loh"
#else
#include "loh/image_le64.loh"
#endif
#else
#include "loh/image.loh"
#endif
#endif
#else
  iuplua_dofile(L, "image.lua");
#endif

  return 0;
}

