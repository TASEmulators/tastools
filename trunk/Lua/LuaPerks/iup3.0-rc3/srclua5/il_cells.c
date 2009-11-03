/******************************************************************************
 * Automatically generated file (iuplua5). Please don't change anything.                *
 *****************************************************************************/

#include <stdlib.h>

#include <lua.h>
#include <lauxlib.h>

#include "iup.h"
#include "iuplua.h"
#include "iupcontrols.h"
#include "il.h"


static int cells_width_cb(Ihandle *self, int p0)
{
  lua_State *L = iuplua_call_start(self, "width_cb");
  lua_pushnumber(L, p0);
  return iuplua_call(L, 1);
}

static int cells_mousemotion_cb(Ihandle *self, int p0, int p1, int p2, int p3, char * p4)
{
  lua_State *L = iuplua_call_start(self, "mousemotion_cb");
  lua_pushnumber(L, p0);
  lua_pushnumber(L, p1);
  lua_pushnumber(L, p2);
  lua_pushnumber(L, p3);
  lua_pushstring(L, p4);
  return iuplua_call(L, 5);
}

static int cells_mouseclick_cb(Ihandle *self, int p0, int p1, int p2, int p3, int p4, int p5, char * p6)
{
  lua_State *L = iuplua_call_start(self, "mouseclick_cb");
  lua_pushnumber(L, p0);
  lua_pushnumber(L, p1);
  lua_pushnumber(L, p2);
  lua_pushnumber(L, p3);
  lua_pushnumber(L, p4);
  lua_pushnumber(L, p5);
  lua_pushstring(L, p6);
  return iuplua_call(L, 7);
}

static int cells_ncols_cb(Ihandle *self)
{
  lua_State *L = iuplua_call_start(self, "ncols_cb");
  return iuplua_call(L, 0);
}

static int cells_nlines_cb(Ihandle *self)
{
  lua_State *L = iuplua_call_start(self, "nlines_cb");
  return iuplua_call(L, 0);
}

static int cells_scrolling_cb(Ihandle *self, int p0, int p1)
{
  lua_State *L = iuplua_call_start(self, "scrolling_cb");
  lua_pushnumber(L, p0);
  lua_pushnumber(L, p1);
  return iuplua_call(L, 2);
}

static int cells_height_cb(Ihandle *self, int p0)
{
  lua_State *L = iuplua_call_start(self, "height_cb");
  lua_pushnumber(L, p0);
  return iuplua_call(L, 1);
}

static int cells_hspan_cb(Ihandle *self, int p0, int p1)
{
  lua_State *L = iuplua_call_start(self, "hspan_cb");
  lua_pushnumber(L, p0);
  lua_pushnumber(L, p1);
  return iuplua_call(L, 2);
}

static int cells_vspan_cb(Ihandle *self, int p0, int p1)
{
  lua_State *L = iuplua_call_start(self, "vspan_cb");
  lua_pushnumber(L, p0);
  lua_pushnumber(L, p1);
  return iuplua_call(L, 2);
}

static int Cells(lua_State *L)
{
  Ihandle *ih = IupCells();
  iuplua_plugstate(L, ih);
  iuplua_pushihandle_raw(L, ih);
  return 1;
}

int iupcellslua_open(lua_State * L)
{
  iuplua_register(L, Cells, "Cells");

  iuplua_register_cb(L, "WIDTH_CB", (lua_CFunction)cells_width_cb, NULL);
  iuplua_register_cb(L, "MOUSEMOTION_CB", (lua_CFunction)cells_mousemotion_cb, NULL);
  iuplua_register_cb(L, "MOUSECLICK_CB", (lua_CFunction)cells_mouseclick_cb, NULL);
  iuplua_register_cb(L, "NCOLS_CB", (lua_CFunction)cells_ncols_cb, NULL);
  iuplua_register_cb(L, "NLINES_CB", (lua_CFunction)cells_nlines_cb, NULL);
  iuplua_register_cb(L, "SCROLLING_CB", (lua_CFunction)cells_scrolling_cb, NULL);
  iuplua_register_cb(L, "HEIGHT_CB", (lua_CFunction)cells_height_cb, NULL);
  iuplua_register_cb(L, "HSPAN_CB", (lua_CFunction)cells_hspan_cb, NULL);
  iuplua_register_cb(L, "VSPAN_CB", (lua_CFunction)cells_vspan_cb, NULL);

#ifdef IUPLUA_USELOH
#ifdef TEC_BIGENDIAN
#ifdef TEC_64
#include "loh/cells_be64.loh"
#else
#include "loh/cells_be32.loh"
#endif
#else
#ifdef TEC_64
#ifdef WIN64
#include "loh/cells_le64w.loh"
#else
#include "loh/cells_le64.loh"
#endif
#else
#include "loh/cells.loh"
#endif
#endif
#else
  iuplua_dofile(L, "cells.lua");
#endif

  return 0;
}

