/** \file
 * \brief IM Lua 5 Binding
 *
 * See Copyright Notice in im_lib.h
 * $Id: imlua_convert.c,v 1.3 2009/08/18 02:23:33 scuri Exp $
 */

#include "im.h"
#include "im_image.h"
#include "im_convert.h"

#include <lua.h>
#include <lauxlib.h>

#include "imlua.h"
#include "imlua_image.h"
#include "imlua_aux.h"

/*****************************************************************************\
 im.ConvertDataType(src_image, dst_image, cpx2real, gamma, abssolute, cast_mode)
\*****************************************************************************/
static int imluaConvertDataType (lua_State *L)
{
  imImage* src_image = imlua_checkimage(L, 1);
  imImage* dst_image = imlua_checkimage(L, 2);
  int cpx2real = luaL_checkint(L, 3);
  float gamma = (float) luaL_checknumber(L, 4);
  int abssolute = lua_toboolean(L, 5);
  int cast_mode = luaL_checkint(L, 6);

  imlua_matchcolorspace(L, src_image, dst_image);
  imlua_pusherror(L, imConvertDataType(src_image, dst_image, cpx2real, gamma, abssolute, cast_mode));
  return 1;
}

/*****************************************************************************\
 im.ConvertColorSpace(src_image, dst_image)
\*****************************************************************************/
static int imluaConvertColorSpace (lua_State *L)
{
  imImage* src_image = imlua_checkimage(L, 1);
  imImage* dst_image = imlua_checkimage(L, 2);

  imlua_matchdatatype(L, src_image, dst_image);
  imlua_pusherror(L, imConvertColorSpace(src_image, dst_image));
  return 1;
}

/*****************************************************************************\
 im.ConvertToBitmap(src_image, dst_image, cpx2real, gamma, abssolute, cast_mode)
\*****************************************************************************/
static int imluaConvertToBitmap (lua_State *L)
{
  imImage* src_image = imlua_checkimage(L, 1);
  imImage* dst_image = imlua_checkimage(L, 2);
  int cpx2real = luaL_checkint(L, 3);
  float gamma = (float) luaL_checknumber(L, 4);
  int abssolute = lua_toboolean(L, 5);
  int cast_mode = luaL_checkint(L, 6);

  imlua_matchsize(L, src_image, dst_image);
  imlua_matchcheck(L, imImageIsBitmap(dst_image), "image must be a bitmap");

  imlua_pusherror(L, imConvertToBitmap(src_image, dst_image, cpx2real, gamma, abssolute, cast_mode));
  return 1;
}

static const luaL_reg imconvert_lib[] = {
  {"ConvertDataType", imluaConvertDataType},
  {"ConvertColorSpace", imluaConvertColorSpace},
  {"ConvertToBitmap", imluaConvertToBitmap},
  {NULL, NULL}
};

void imlua_open_convert (lua_State *L)
{
  /* "im" table is at the top of the stack */
  luaL_register(L, NULL, imconvert_lib);
#ifdef TEC_BIGENDIAN
#ifdef TEC_64
#include "loh/im_convert_be64.loh"
#else
#include "loh/im_convert_be32.loh"
#endif
#else
#ifdef TEC_64
#ifdef WIN64
#include "loh/im_convert_le64w.loh"
#else
#include "loh/im_convert_le64.loh"
#endif
#else
#include "loh/im_convert.loh"
#endif
#endif
}
