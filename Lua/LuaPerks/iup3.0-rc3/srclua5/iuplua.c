/** \file
* \brief IUP binding for Lua 5.
*
* See Copyright Notice in "iup.h"
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "iup.h"
#include "iup_str.h"

#include <lua.h>
#include <lauxlib.h>

#include "iuplua.h"
#include "il.h"


/*****************************************************************************
* Auxiliary functions                                                       *
****************************************************************************/


             /*************************************/
             /* iuplua_dostring and iuplua_dofile */

static void error_message(lua_State *L, const char *msg, const char* traceback)
{
  lua_getglobal(L, "iup");
  lua_pushstring(L, "_ERRORMESSAGE");
  lua_gettable(L, -2);
  lua_remove(L, -2);

  if(lua_isnil(L, -1))
  {
    /* Panic mode */
    if (traceback != NULL)
      fprintf(stderr, "%s\n%s\n", msg, traceback);
    else
      fprintf(stderr, "%s\n", msg);
    fflush(stderr);
    return;
  }
  lua_pushstring(L, msg);
  lua_pushstring(L, traceback);
  lua_call(L, 2, 0);
}

static int report (lua_State *L, int status, int concat_traceback)
{
  if (status && !lua_isnil(L, -1)) 
  {
    const char *msg = lua_tostring(L, -2);



    const char *traceback;
    if (msg == NULL) {
      msg = "(error with no message)";
      traceback = NULL;
    }
    else if (concat_traceback) {
      lua_concat(L, 2);
      msg = lua_tostring(L, -1);
      traceback = NULL;
    }
    else {
      traceback = lua_tostring(L, -1);
    }
    error_message(L, msg, traceback);
    lua_pop(L, 2);
  }
  return status;
}

static int traceback (lua_State *L) {
  lua_getfield(L, LUA_GLOBALSINDEX, "debug");
  if (!lua_istable(L, -1)) {
    lua_pop(L, 1);
    return 1;
  }
  lua_getfield(L, -1, "traceback");
  if (!lua_isfunction(L, -1)) {
    lua_pop(L, 2);
    return 1;
  }

  lua_remove(L, 2);
  lua_pushliteral(L, "");
  lua_pushinteger(L, 2);  /* skip this function */
  lua_call(L, 2, 1);  /* call debug.traceback */
  lua_getglobal(L, "iup"); /* store traceback in iup._LASTTRACEBACK */
  lua_pushstring(L, "_LASTTRACEBACK");
  lua_pushvalue(L, -3);
  lua_settable(L, -3);
  lua_pop(L, 2);
  return 1;
}

static int docall (lua_State *L, int narg, int nret) 
{
  int status;
  int base = lua_gettop(L) - narg;  /* function index */
  lua_pushcfunction(L, traceback);  /* push traceback function */
  lua_insert(L, base);  /* put it under chunk and args */
  status = lua_pcall(L, narg, nret, base);
  lua_remove(L, base);  /* remove traceback function */
  if (status != 0) {
    /* force a complete garbage collection in case of errors */
    lua_gc(L, LUA_GCCOLLECT, 0);
    /* put _LASTTRACEBACK at stack position 2 */
    lua_getglobal(L, "iup");
    lua_pushliteral(L, "_LASTTRACEBACK");
    lua_gettable(L, -2);
    lua_remove(L, -2);
    if (!lua_isstring(L, -1)) {
      lua_pop(L, 1);
      lua_pushliteral(L, "");
      /* set _LASTTRACEBACK as nil */
      lua_getglobal(L, "iup");
      lua_pushliteral(L, "_LASTTRACEBACK");
      lua_pushnil(L);
      lua_settable(L, -3);
      lua_pop(L, 1);
    }
  }
  return status;
}

int iuplua_dofile(lua_State *L, const char *filename)
{
  int status = luaL_loadfile(L, filename);
  if (status == 0)
    status = docall(L, 0, 0);
  return report(L, status, 1);
}

int iuplua_dostring(lua_State *L, const char *s, const char *name)
{
  int status = luaL_loadbuffer(L, s, strlen(s), name);
  if (status == 0)
    status = docall(L, 0, 0);
  return report(L, status, 1);
}

             /*************************************/
             /*              Utilities        */

Ihandle *iuplua_checkihandleornil(lua_State *L, int pos)
{
  if (lua_isnil(L, pos))
    return NULL;
  else
    return iuplua_checkihandle(L, pos);
}

Ihandle *iuplua_checkihandle(lua_State *L, int pos)
{
  lua_getmetatable(L, pos);   /* t2 = metatable(stack(pos)) */
  lua_pushstring(L, "iup handle");
  lua_gettable(L, LUA_REGISTRYINDEX);  /* t = registry["iup handle"] */
  if (lua_equal(L, -2, -1))   /* check (t2==t)? */
  {
    lua_pop (L, 2);
    return *(Ihandle**)lua_touserdata(L, pos);
  } 
  else 
  {
    luaL_argerror(L, pos, "iup handle expected");
    return NULL;
  }
}

void iuplua_pushihandle_raw(lua_State *L, Ihandle *ih)
{
  if (ih) 
  {
    Ihandle** new_pointer = (Ihandle**)lua_newuserdata(L, sizeof(Ihandle*));
    *new_pointer = ih;
  } 
  else 
    lua_pushnil(L);
}

void iuplua_pushihandle(lua_State *L, Ihandle *ih)
{
  if (ih) 
  {
    /* not created in Lua? */
    char* sref = IupGetAttribute(ih, "_IUPLUA_WIDGET_TABLE_REF");
    if (!sref)
    {
      lua_getglobal(L,"iup");
      lua_pushstring(L,"RegisterHandle");
      lua_gettable(L, -2);  /* f = iup.RegisterHandle */
      lua_remove(L, -2);
      iuplua_pushihandle_raw(L, ih);  /* push ih */
      lua_pushstring(L, IupGetClassName(ih));  /* push type */
      lua_call(L, 2, 1);          /* call f(ih, type) */
    }
    else
    {
      iuplua_pushihandle_raw(L, ih);  /* push ih */
      lua_pushstring(L, "iup handle");
      lua_gettable(L, LUA_REGISTRYINDEX);  /* t = registry["iup handle"] */
      lua_setmetatable(L, -2);    /* metatable(ih) = t */
    }
  } 
  else 
    lua_pushnil(L);
}

void iuplua_removeihandle(lua_State *L, Ihandle *ih)
{
  char* sref;

  /* called from Destroy.
     must also remove references from the children. */
  Ihandle* child = IupGetNextChild(ih, NULL);
  while(child)
  {
    iuplua_removeihandle(L, child);
    child = IupGetNextChild(ih, child);
  }

  sref = IupGetAttribute(ih, "_IUPLUA_WIDGET_TABLE_REF");
  if (sref)
  {
    int ref = atoi(sref);

    /* removes the ihandle reference in the lua table */
    /* object.handle = nil */
    lua_getref(L, ref);  /* push object */
    lua_pushstring(L, "handle");
    lua_pushnil(L);
    lua_settable(L, -3);
    lua_pop(L,1);

    /* removes the association of the ihandle with the lua table */
    lua_unref(L, ref);  /* this is the complement of SetWidget */
    IupSetAttribute(ih, "_IUPLUA_WIDGET_TABLE_REF", NULL);
  }
}

char ** iuplua_checkstring_array(lua_State *L, int pos)
{
  int i,n;
  char **v;
  n = luaL_getn(L,pos);
  v = (char **) malloc (n*sizeof(char *));
  for(i=1; i<=n; i++)
  {
    lua_pushnumber(L,i);
    lua_gettable(L,pos);
    v[i-1] = (char*)lua_tostring(L, -1);
    lua_pop(L,1);
  }
  return v;
}

int * iuplua_checkint_array(lua_State *L, int pos)
{
  int i,n;
  int *v;
  n = luaL_getn(L,pos);
  v = (int *) malloc (n*sizeof(int));
  for(i=1; i<=n; i++)
  {
    lua_pushnumber(L,i);
    lua_gettable(L,pos);
    v[i-1] = (int)lua_tonumber(L, -1);
    lua_pop(L,1);
  }
  return v;
}

unsigned char* iuplua_checkuchar_array(lua_State *L, int pos, int count)
{
  int i,n;
  unsigned char *v;
  n = luaL_getn(L,pos);
  if (n != count)
  {
    lua_pushstring(L, "invalid number of elements in array");
    lua_error(L);
  }
  v = (unsigned char *) malloc (n*sizeof(unsigned char));
  for(i=1; i<=n; i++)
  {
    lua_pushnumber(L,i);
    lua_gettable(L,pos);
    v[i-1] = (unsigned char)lua_tonumber(L, -1);
    lua_pop(L,1);
  }
  return v;
}

Ihandle ** iuplua_checkihandle_array(lua_State *L, int pos)
{
  Ihandle **v;
  int i, n = luaL_getn(L, pos);
  v = (Ihandle **) malloc ((n+1)*sizeof(Ihandle *));
  for (i=1; i<=n; i++)
  {
    lua_pushnumber(L,i);
    lua_gettable(L,pos);
    v[i-1] = iuplua_checkihandle(L, -1);
    lua_pop(L,1);
  }
  v[i-1] = NULL;
  return v;
}

             /*************************************/
             /*         used by callbacks         */

void iuplua_plugstate(lua_State *L, Ihandle *ih)
{
  IupSetAttribute(ih, "_IUPLUA_STATE_CONTEXT",(char *) L);
}

lua_State* iuplua_getstate(Ihandle *ih)
{
  return (lua_State *) IupGetAttribute(ih, "_IUPLUA_STATE_CONTEXT");
}

lua_State* iuplua_call_start(Ihandle *ih, const char* name)
{
  lua_State *L = iuplua_getstate(ih);

  /* prepare to call iupCallMethod(name, handle, ...) */
  lua_getglobal(L,"iup");
  lua_pushstring(L,"iupCallMethod");
  lua_gettable(L, -2);
  lua_remove(L, -2);

  lua_pushstring(L, name);
  iuplua_pushihandle(L, ih);
  return L;
}

int iuplua_call(lua_State* L, int nargs)
{
  int status = docall(L, nargs+2, 1);
  report(L, status, 0);

  if (status)
    return IUP_DEFAULT;
  else
  {
    int tmp = (int) lua_isnil(L, -1) ? IUP_DEFAULT : (int)lua_tonumber(L,-1);
    lua_pop(L, 1);
    return tmp;    
  }
}

char* iuplua_call_rs(lua_State *L, int nargs)
{
  int status = docall(L, nargs+2, 1);
  report(L, status, 0);

  if (status)
    return NULL;
  else
  {
    char* tmp = lua_isnil(L, -1) ? NULL: (char*)lua_tostring(L,-1);
    lua_pop(L, 1);
    return tmp;    
  }
}

int iuplua_call_raw(lua_State* L, int nargs, int nresults)
{
  int status = docall(L, nargs, nresults);
  report(L, status, 0);
  return status;
}

void iuplua_register_cb(lua_State *L, const char* name, lua_CFunction func, const char* type)
{
  lua_getglobal(L, "RegisterCallback");
  lua_pushstring(L, name);
  lua_pushcfunction(L, func);
  lua_pushstring(L, type);
  lua_call(L, 3, 0); 
}

/* iupSetCallback(handle, name, func, value) */
static int SetCallback(lua_State *L)
{
  Icallback func;

  Ihandle* ih = iuplua_checkihandle(L, 1);
  const char* name = luaL_checkstring(L, 2);

  if (!lua_iscfunction(L, 3))
  {
    lua_pushstring(L, "invalid function when set callback");
    lua_error(L);
  }
  func = (Icallback)lua_tocfunction(L, 3);

  if (lua_isnil(L, 4))  /* value is only used here to remove the callback */
    IupSetCallback(ih, name, (Icallback)NULL);
  else
    IupSetCallback(ih, name, func);

  /* value, when not nil, is always the same name of a C callback in lowercase */

  return 0;
}

             /*************************************/
             /*        metatable                  */

/* iupNewClass(class_name) 
  Calls:
    iupNewClass("iup handle")
    iupNewClass("iup widget")
*/
static int NewClass(lua_State *L)
{
  lua_newtable(L);     /* push t */
  lua_pushstring(L, "class");  /* push "class" */
  lua_pushvalue(L, 1); /* push again the class_name to the stack, because settable will remove it */
  lua_settable(L, -3); /* t.class = class_name */
  lua_settable(L, LUA_REGISTRYINDEX);  /* registry[class_name] = t */
  return 0;
}

/* iupSetClass(t, class_name) 
  Calls:
    iupSetClass(handle, "iup handle")  --Used only in RegisterHandle and WIDGET.constructor
    iupSetClass(object, "iup widget")  --Used whenever a new control class is created.
*/
static int SetClass(lua_State *L)
{
  lua_gettable(L, LUA_REGISTRYINDEX);  /* t2 = registry[class_name] */
  if (lua_isnil(L, -1))
  {
    lua_pushstring(L, "invalid class name");
    lua_error(L);
  }
  lua_setmetatable(L, -2);  /* metatable(t) = t2 */
  return 0;
}

/* class_name = iupGetClass(t) */
static int GetClass(lua_State *L)
{
  if (lua_istable(L, 1) || lua_isuserdata(L, 1)) 
  {
    lua_getmetatable(L, 1); /* t2 = metatable(t) */
    if (lua_istable(L, -1)) 
    {
      lua_pushstring(L, "class");
      lua_gettable(L, -2);    /* class_name = t2.class */ 
      return 1;
    }
  }
  lua_pushnil(L);
  return 1;
}

/* iupSetMethod(class_name, method, function)
   For ex:
     iupSetMethod("iup handle", "__index", ihandle_gettable)
*/
static int SetMethod(lua_State *L)
{
  lua_pushvalue(L, 1); /* push class_name */
  lua_gettable(L, LUA_REGISTRYINDEX);  /* t = registry[class_name] */
  if (lua_isnil(L, -1)) 
  {
    lua_pushstring(L, "invalid class name");
    lua_error(L);
  }
  lua_pushvalue(L, -3); /* push method */
  lua_pushvalue(L, -3); /* push function */
  lua_settable(L, -3);  /* t.method = function */
  return 0;
}

static int ihandle_tostring (lua_State *L) 
{
  Ihandle *ih = iuplua_checkihandle(L, 1);
  if (!ih)
    lua_pushstring (L, "(not an IUP handle)");
  else
    lua_pushfstring (L, "IUP(%s): %p", IupGetClassName(ih), ih);
  return 1;
}

static int ihandle_compare(lua_State *L)
{
  Ihandle *a = iuplua_checkihandle(L, 1);
  Ihandle *b = iuplua_checkihandle(L, 2);
  if(a == b)
    lua_pushboolean(L, 1);
  else
    lua_pushboolean(L, 0);
  return 1;
}

             /*************************************/
             /*       table <-> ihandle           */

/* local object = iupGetWidget(handle) */
static int GetWidget(lua_State *L)
{
  /* Pushes a table that is associanted with an ihandle */
  /* Used by the "__index" metamethod of the "iup handle" */
  Ihandle * ih = iuplua_checkihandle(L,1);
  char* sref = IupGetAttribute(ih, "_IUPLUA_WIDGET_TABLE_REF");
  if (!sref)
    lua_pushnil(L);
  else
    lua_getref(L, atoi(sref));
  return 1;
}

/* iupSetWidget(handle, object) */
static int SetWidget(lua_State *L)
{
  /* Saves the object table reference as an attribute, 
     i.e. associates a table with the ihandle.
     Used by the "iup widget" constructor and by RegisterHandle */
  Ihandle * ih = iuplua_checkihandle(L,1);
  char* sref = IupGetAttribute(ih, "_IUPLUA_WIDGET_TABLE_REF");
  if (!sref)
  {
    int ref = lua_ref(L, 1);
    IupSetfAttribute(ih, "_IUPLUA_WIDGET_TABLE_REF", "%d", ref);  /* this must be a non-inheritable attribute */
  }
  return 0;
}

             /*************************************/
             /*          registration             */

void iuplua_changeEnv(lua_State *L)
{
  /* Replaces global environment */
  lua_pushvalue(L, LUA_GLOBALSINDEX);
  lua_getglobal(L, "iup");
  lua_replace(L, LUA_GLOBALSINDEX); 
}

void iuplua_returnEnv(lua_State *L)
{
  /* reestablishes global environment */
  lua_replace(L, LUA_GLOBALSINDEX);
}

int iuplua_opencall_internal(lua_State * L)
{
  int ret = 0;
  const char* s;
  lua_getglobal(L, "iup");
  lua_pushliteral(L,"_IUPOPEN_CALL");
  lua_gettable(L, -2);
  s = lua_tostring(L, -1);
  if (s && strcmp(s, "INTERNAL")==0)
    ret = 1;
  lua_pop(L,2);
  return ret;
}

void iuplua_register(lua_State *L, lua_CFunction func, const char* name)
{
  lua_pushcfunction(L, func);
  lua_setglobal(L, name);
}

void iuplua_regstring(lua_State *L, const char* s, const char* name)
{
  lua_pushstring(L, s); 
  lua_setglobal(L, name);
}

/*****************************************************************************
* Common Callbacks                                                     *
****************************************************************************/

static int help_cb(Ihandle *self)
{
  lua_State *L = iuplua_call_start(self, "help_cb");
  return iuplua_call(L, 0);
}

static int getfocus_cb(Ihandle *self)
{
  lua_State *L = iuplua_call_start(self, "getfocus_cb");
  return iuplua_call(L, 0);
}

static int k_any(Ihandle *self, int c)
{
  lua_State *L = iuplua_call_start(self, "k_any");
  lua_pushnumber(L, c);
  return iuplua_call(L, 1);
}

static int killfocus_cb(Ihandle *self)
{
  lua_State *L = iuplua_call_start(self, "killfocus_cb");
  return iuplua_call(L, 0);
}

static int Idlecall(void)
{
  int ret = 0;
  lua_State *L = (lua_State *) IupGetGlobal("_IUP_LUA_DEFAULT_STATE");
  lua_getglobal(L, "_IUP_LUA_IDLE_FUNC_"); 
  lua_call(L, 0, 1);
  ret = (int) lua_tonumber(L, -1);
  lua_pop(L, 1);
  return ret;
}

static int SetIdle(lua_State *L)
{
  if lua_isnoneornil(L,1)
    IupSetFunction("IDLE_ACTION", NULL);
  else
  {
    luaL_checktype(L, 1, LUA_TFUNCTION);
    lua_pushvalue(L,1);
    lua_setglobal(L, "_IUP_LUA_IDLE_FUNC_");
    IupSetFunction("IDLE_ACTION", (Icallback) Idlecall);
  }
  return 0;
}

/*****************************************************************************
* Iuplua bind functions                                                     *
****************************************************************************/


static int GetFromC(lua_State *L)
{
  Ihandle *n;
  const char *a;
  if (!lua_istable(L, -1)) 
  {
    lua_pushstring(L, "IupGetFromC: wrong arguments to function"); 
    lua_error(L);
    return 0;
  }
  lua_pushnumber(L, 1);
  lua_gettable(L, -2);
  if (!lua_isstring(L, -1)) 
  {
    lua_pushstring(L, "IupGetFromC: wrong arguments to function"); 
    lua_error(L);
    return 0;
  }
  a = lua_tostring(L, -1);
  n = IupGetHandle((char*)a);
  if(n)
    iuplua_pushihandle(L, n);
  else
    lua_pushnil(L);
  return 1;
}


/*****************************************************************************
 *   iupkey                                                                  *
 ****************************************************************************/


static void register_key(char *name, int code, void* user_data)
{
  lua_State *L = (lua_State*)user_data;
  lua_pushnumber(L, code); lua_setglobal(L, name);
}

/* from iupkey.c */
void iupKeyForEach(void (*func)(char *name, int code, void* user_data), void* user_data);

int iupkey_open(lua_State *L)
{
  (void)L;
  /* does nothing, kept for backward compatibility */
  return 0;
}


/*****************************************************************************
* iuplua_open                                                               *
****************************************************************************/


static int il_open(lua_State * L)
{
  int ret, argc = 0; 
  char **argv = NULL;

  lua_getglobal(L, "arg");
  if (lua_istable(L, -1))
  {
    int i;
    argc = luaL_getn(L, -1);
    argv = malloc(sizeof(char*)*argc);
    for(i=1; i<=argc; i++)
    {
      lua_pushnumber(L,i);
      lua_gettable(L,-2);
      argv[i-1] = (char*)lua_tostring(L, -1);
      lua_pop(L,1);
    }
  }
  lua_pop(L, 1);

  ret = IupOpen(&argc, &argv);
  if (ret == IUP_ERROR)
  {
    if (argv) free(argv);
    lua_pushstring(L, "IupOpen: could not initialize"); 
    lua_error(L);
    return 0;
  }

  if (argv) free(argv);
  lua_pushinteger(L, ret);
  return 1;
}

int iuplua_close(lua_State * L)
{
  if (iuplua_opencall_internal(L))
    IupClose();
  return 0; /* nothing in stack */
}

static void setinfo (lua_State *L) 
{
  /* table "iup" is at the stack */

  lua_pushliteral (L, "_COPYRIGHT");
  lua_pushliteral (L, IUP_COPYRIGHT);
  lua_settable (L, -3);

  lua_pushliteral (L, "_DESCRIPTION");
  lua_pushliteral (L, IUP_DESCRIPTION);
  lua_settable (L, -3);

  lua_pushliteral (L, "_NAME");
  lua_pushliteral (L, IUP_NAME);
  lua_settable (L, -3);

  lua_pushliteral (L, "_VERSION");
  lua_pushstring (L, IupVersion());
  lua_settable (L, -3);

  lua_pushliteral (L, "_VERSION_DATE");
  lua_pushliteral (L, IUP_VERSION_DATE);
  lua_settable (L, -3);

  lua_pushliteral (L, "_VERSION_NUMBER");
  lua_pushinteger (L, IupVersionNumber());
  lua_settable (L, -3);
}

int iuplua_open(lua_State * L)
{
  int ret;

  struct luaL_reg funcs[] = {
    {"key_open", iupkey_open},
    {"Open", il_open},
    {"Close", iuplua_close},
    {"SetIdle", SetIdle},
    {"GetFromC", GetFromC},
    {"iupGetWidget", GetWidget},
    {"iupSetWidget", SetWidget},
    {"iupNewClass", NewClass},
    {"iupSetClass", SetClass},
    {"iupGetClass", GetClass},
    {"iupSetMethod", SetMethod},
    {"iupSetCallback", SetCallback},
    {"ihandle_compare", ihandle_compare},
    {"ihandle_tostring", ihandle_tostring},
    {NULL, NULL},
  };

  if (!il_open(L))
    return 0;

  ret = lua_tointeger(L, -1); /* retrieve IupOpen return value */
  lua_pop(L, -1);

  /* Creating global namespace iup */
  lua_newtable(L);
  lua_setglobal(L, "iup");

  /* Registers functions in iup namespace */
  luaL_openlib(L, "iup", funcs, 0);
  iupluaapi_open(L);

  /* set version info */
  setinfo(L);

  /* register if IupOpen was called here or from outside IupLua */
  lua_pushliteral (L, "_IUPOPEN_CALL");
  if (ret == IUP_OPENED) lua_pushliteral (L, "EXTERNAL");
  else                   lua_pushliteral (L, "INTERNAL");
  lua_settable (L, -3);

  /* used by Idle */
  IupSetGlobal("_IUP_LUA_DEFAULT_STATE", (char *) L);  

  /* Creating new environment with metamethod __index 
   * (so that normal Lua functions will be found while GLOBALSINDEX is the "iup" environment) */
  iuplua_dostring(L, "iup._G = _G\
                     setmetatable(iup, {__index = iup._G})", "iuplua_setmetatable_global");

  /* Changing environment to iup 
   * (all created variables and functions from now on will be put in "iup" environment) */
  iuplua_changeEnv(L);

#ifdef IUPLUA_USELOH
#ifdef TEC_BIGENDIAN
#ifdef TEC_64
#include "loh/iuplua_be64.loh"
#include "loh/constants_be64.loh"
#else
#include "loh/iuplua_be32.loh"
#include "loh/constants_be32.loh"
#endif  
#else
#ifdef TEC_64
#ifdef WIN64
#include "loh/iuplua_le64w.loh"
#include "loh/constants_le64w.loh"
#else
#include "loh/iuplua_le64.loh"
#include "loh/constants_le64.loh"
#endif  
#else
#include "loh/iuplua.loh"
#include "loh/constants.loh"
#endif  
#endif  
#else
  iuplua_dofile(L, "iuplua.lua");
  iuplua_dofile(L, "constants.lua");
#endif

  /* Register the common callbacks */
  iuplua_register_cb(L, "HELP_CB", (lua_CFunction)help_cb, NULL);
  iuplua_register_cb(L, "GETFOCUS_CB", (lua_CFunction)getfocus_cb, NULL);
  iuplua_register_cb(L, "K_ANY", (lua_CFunction)k_any, NULL);
  iuplua_register_cb(L, "KILLFOCUS_CB", (lua_CFunction)killfocus_cb, NULL);

  /* Register Keys */
  iupKeyForEach(register_key, (void*)L);

  /* Iup Modules initialization */
  iupbuttonlua_open(L);
  iupcanvaslua_open(L);
  iupdialoglua_open(L);
  iupfilllua_open(L);
  iupframelua_open(L);
  iupfiledlglua_open(L);
  iuphboxlua_open(L);
  iupitemlua_open(L);
  iupimagelua_open(L);
  iuplabellua_open(L);
  iuplistlua_open(L);
  iupmenulua_open(L);
  iupmultilinelua_open(L);
  iupradiolua_open(L);
  iupseparatorlua_open(L);
  iupsubmenulua_open(L);
  iuptextlua_open(L);
  iuptogglelua_open(L);
  iupvboxlua_open(L);
  iupzboxlua_open(L);
  iuptimerlua_open(L);
  iupsboxlua_open(L);
  iupspinlua_open(L);
  iupspinboxlua_open(L);
  iupcboxlua_open(L);
  iupgclua_open(L);
  iupgetparamlua_open(L);
  iupvallua_open(L);
  iuptabslua_open(L);
  iupfontdlglua_open(L);
  iupmessagedlglua_open(L);
  iupcolordlglua_open(L);
  iupimagergbalua_open(L);
  iupimagergblua_open(L);
  iupprogressbarlua_open(L);
  iupnormalizerlua_open(L);
  iupuserlua_open(L);
  iuptreelua_open(L);
  iupclipboardlua_open(L);

  iuplua_returnEnv(L);

  return 0; /* nothing in stack */
}

/* obligatory to use require"iuplua" */
int luaopen_iuplua(lua_State* L)
{
  return iuplua_open(L);
}

/* obligatory to use require"iuplua51" */
int luaopen_iuplua51(lua_State* L)
{
  return iuplua_open(L);
}
