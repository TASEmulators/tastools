/** \file
 * \brief Ihandle Class Attribute Management
 *
 * See Copyright Notice in "iup.h"
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <memory.h>

#include "iup.h"

#include "iup_object.h"
#include "iup_drv.h"
#include "iup_drvfont.h"
#include "iup_str.h"
#include "iup_attrib.h"
#include "iup_assert.h"
#include "iup_register.h"
#include "iup_globalattrib.h"


typedef struct _IattribFunc
{
  IattribGetFunc get;
  IattribSetFunc set;
  const char* default_value;
  const char* system_default;
  int call_global_default;
  int flags;
} IattribFunc;


static int iClassIsGlobalDefault(const char* name)
{
  if (iupStrEqual(name, "DEFAULTFONT"))
    return 1;
  if (iupStrEqual(name, "DLGBGCOLOR"))
    return 1;
  if (iupStrEqual(name, "DLGFGCOLOR"))
    return 1;
  if (iupStrEqual(name, "TXTBGCOLOR"))
    return 1;
  if (iupStrEqual(name, "TXTFGCOLOR"))
    return 1;
  if (iupStrEqual(name, "MENUBGCOLOR"))
    return 1;
  return 0;
}

static const char* iClassFindId(const char* name)
{
  while(*name)
  {
    if (*name >= '0' && *name <= '9')
      return name;
    if (*name == '*' || *name == ':')
      return name;

    name++;
  }
  return NULL;
}

static const char* iClassCutNameId(const char* name, const char* name_id)
{
  char* str;
  int len = name_id - name;
  if (len == 0)
    return NULL;

  str = iupStrGetMemory(len+1);
  memcpy(str, name, len);
  str[len] = 0;
  return str;
}


static char* iClassGetDefaultValue(IattribFunc* afunc)
{
  if (afunc->call_global_default)
    return IupGetGlobal(afunc->default_value);
  else
    return (char*)afunc->default_value;
}

int iupClassObjectSetAttribute(Ihandle* ih, const char* name, const char * value, int *inherit)
{
  IattribFunc* afunc;

  if (ih->iclass->has_attrib_id)
  {
    const char* name_id = iClassFindId(name);
    if (name_id)
    {
      IattribFunc* afunc;
      const char* partial_name = iClassCutNameId(name, name_id);
      if (!partial_name)
        partial_name = "IDVALUE";  /* pure numbers are used as attributes in IupList and IupMatrix, 
                                      translate them into IDVALUE. */
      afunc = (IattribFunc*)iupTableGet(ih->iclass->attrib_func, partial_name);
      if (afunc)
      {         
        *inherit = 0;       /* id numbered attributes are NON inheritable always */

        if (afunc->flags & IUPAF_READONLY)
        {
          if (afunc->flags & IUPAF_NO_STRING)
            return -1;  /* value is NOT a string, can NOT call iupAttribStoreStr */
          return 0;
        }

        if (afunc->set && (ih->handle || afunc->flags & IUPAF_NOT_MAPPED))
        {
          /* id numbered attributes have default value NULL always */
          IattribSetIdFunc id_set = (IattribSetIdFunc)afunc->set;
          return id_set(ih, name_id, value);
        }

        if (afunc->flags & IUPAF_NO_STRING)
          return -1; /* value is NOT a string, can NOT call iupAttribStoreStr */

        return 1; /* if the function exists, then must return here */
      }
    }
  }

  /* if not has_attrib_id, or not found an ID, or not found the partial name, check using the full name */

  afunc = (IattribFunc*)iupTableGet(ih->iclass->attrib_func, name);
  *inherit = 1; /* default is inheritable */
  if (afunc)
  {
    *inherit = !(afunc->flags & IUPAF_NO_INHERIT) && !(afunc->flags & IUPAF_NO_STRING);

    if (afunc->flags & IUPAF_READONLY)
    {
      if (afunc->flags & IUPAF_NO_STRING)
        return -1;  /* value is NOT a string, can NOT call iupAttribStoreStr */
      return 0;
    }

    if (afunc->set && (ih->handle || afunc->flags & IUPAF_NOT_MAPPED))
    {
      int ret;
      if (!value)
      {
        /* inheritable attributes when reset must check the parent value */
        if (*inherit && ih->parent)   
          value = iupAttribGetInherit(ih->parent, name); 

        if (!value)
          value = iClassGetDefaultValue(afunc);
      }

      if (afunc->flags & IUPAF_HAS_ID)
      {
        IattribSetIdFunc id_set = (IattribSetIdFunc)afunc->set;
        return id_set(ih, "", value);  /* empty Id */
      }
      else
        ret = afunc->set(ih, value);

      if (ret == 1 && afunc->flags & IUPAF_NO_STRING)
        return -1;  /* value is NOT a string, can NOT call iupAttribStoreStr */

      if (*inherit)
        return 1;   /* inheritable attributes are always stored in the hash table, */
      else          /* to indicate that they are set at the control.               */
        return ret;
    }
  }

  return 1;
}

char* iupClassObjectGetAttribute(Ihandle* ih, const char* name, char* *def_value, int *inherit)
{
  IattribFunc* afunc;

  if (ih->iclass->has_attrib_id)
  {
    const char* name_id = iClassFindId(name);
    if (name_id)
    {
      IattribFunc* afunc;
      const char* partial_name = iClassCutNameId(name, name_id);
      if (!partial_name)
        partial_name = "IDVALUE";  /* pure numbers are used as attributes in IupList and IupMatrix, 
                                      translate them into IDVALUE. */
      afunc = (IattribFunc*)iupTableGet(ih->iclass->attrib_func, partial_name);
      if (afunc)
      {
        *def_value = NULL;  /* id numbered attributes have default value NULL always */
        *inherit = 0;       /* id numbered attributes are NON inheritable always */

        if (afunc->flags & IUPAF_WRITEONLY)
          return NULL;

        if (afunc->get && (ih->handle || afunc->flags & IUPAF_NOT_MAPPED))
        {
          IattribGetIdFunc id_get = (IattribGetIdFunc)afunc->get;
          return id_get(ih, name_id);
        }
        else
          return NULL;      /* if the function exists, then must return here */
      }
    }
  }

  /* if not has_attrib_id, or not found an ID, or not found the partial name, check using the full name */

  afunc = (IattribFunc*)iupTableGet(ih->iclass->attrib_func, name);
  *def_value = NULL;
  *inherit = 1; /* default is inheritable */
  if (afunc)
  {
    *def_value = iClassGetDefaultValue(afunc);
    *inherit = !(afunc->flags & IUPAF_NO_INHERIT) && !(afunc->flags & IUPAF_NO_STRING);

    if (afunc->flags & IUPAF_WRITEONLY)
      return NULL;

    if (afunc->get && (ih->handle || afunc->flags & IUPAF_NOT_MAPPED))
    {
      if (afunc->flags & IUPAF_HAS_ID)
      {
        IattribGetIdFunc id_get = (IattribGetIdFunc)afunc->get;
        return id_get(ih, "");  /* empty Id */
      }
      else
        return afunc->get(ih);
    }
  }
  return NULL;
}

void iupClassObjectGetAttributeInfo(Ihandle* ih, const char* name, char* *def_value, int *inherit)
{
  IattribFunc* afunc;

  if (ih->iclass->has_attrib_id)
  {
    const char* name_id = iClassFindId(name);
    if (name_id)
    {
      IattribFunc* afunc;
      const char* partial_name = iClassCutNameId(name, name_id);
      if (!partial_name)
        partial_name = "IDVALUE";  /* pure numbers are used as attributes in IupList and IupMatrix, 
                                      translate them into IDVALUE. */
      afunc = (IattribFunc*)iupTableGet(ih->iclass->attrib_func, partial_name);
      if (afunc)
      {
        *def_value = NULL;  /* id numbered attributes have default value NULL always */
        *inherit = 0;       /* id numbered attributes are NON inheritable always */
         return;      /* if the function exists, then must return here */
      }
    }
  }

  /* if not has_attrib_id, or not found an ID, or not found the partial name, check using the full name */

  afunc = (IattribFunc*)iupTableGet(ih->iclass->attrib_func, name);
  *def_value = NULL;
  *inherit = 1; /* default is inheritable */
  if (afunc)
  {
    *def_value = iClassGetDefaultValue(afunc);
    *inherit = !(afunc->flags & IUPAF_NO_INHERIT) && !(afunc->flags & IUPAF_NO_STRING);
  }
}

int iupClassObjectCurAttribIsInherit(Iclass* ic)
{
  IattribFunc* afunc = (IattribFunc*)iupTableGetCurr(ic->attrib_func);
  if (afunc && !(afunc->flags & IUPAF_NO_INHERIT))
    return 1;
  return 0;
}

int iupClassObjectAttribIsNotString(Ihandle* ih, const char* name)
{
  IattribFunc* afunc = (IattribFunc*)iupTableGet(ih->iclass->attrib_func, name);
  if (afunc && afunc->flags & IUPAF_NO_STRING)
    return 1;
  return 0;
}

void iupClassRegisterAttribute(Iclass* ic, const char* name, 
                               IattribGetFunc _get, IattribSetFunc _set, 
                               const char* _default_value, const char* _system_default, int _flags)
{
  IattribFunc* afunc = (IattribFunc*)iupTableGet(ic->attrib_func, name);
  if (afunc)
    free(afunc);  /* overwrite a previous registration */

  afunc = (IattribFunc*)malloc(sizeof(IattribFunc));
  afunc->get = _get;
  afunc->set = _set;
  if (_default_value == IUPAF_SAMEASSYSTEM)
    afunc->default_value = _system_default;
  else
    afunc->default_value = _default_value;
  afunc->system_default = _system_default;
  afunc->flags = _flags;

  if (iClassIsGlobalDefault(afunc->default_value))
    afunc->call_global_default = 1;
  else
    afunc->call_global_default = 0;

  iupTableSet(ic->attrib_func, name, (void*)afunc, IUPTABLE_POINTER);
}

void iupClassRegisterAttributeId(Iclass* ic, const char* name, 
                               IattribGetIdFunc _get, IattribSetIdFunc _set, 
                               int _flags)
{
  IattribFunc* afunc = (IattribFunc*)iupTableGet(ic->attrib_func, name);
  if (afunc)
    free(afunc);  /* overwrite a previous registration */

  afunc = (IattribFunc*)malloc(sizeof(IattribFunc));
  afunc->get = (IattribGetFunc)_get;
  afunc->set = (IattribSetFunc)_set;
  afunc->default_value = NULL;
  afunc->system_default = NULL;
  afunc->flags = _flags|IUPAF_HAS_ID|IUPAF_NO_INHERIT|IUPAF_NO_DEFAULTVALUE;
  afunc->call_global_default = 0;

  iupTableSet(ic->attrib_func, name, (void*)afunc, IUPTABLE_POINTER);
}

void iupClassRegisterGetAttribute(Iclass* ic, const char* name, 
                                  IattribGetFunc *_get, IattribSetFunc *_set, 
                                  const char* *_default_value, const char* *_system_default, int *_flags)
{
  IattribFunc* afunc = (IattribFunc*)iupTableGet(ic->attrib_func, name);
  if (afunc)
  {
    if (_get) *_get = afunc->get;
    if (_set) *_set = afunc->set;
    if (_default_value) *_default_value = afunc->default_value;
    if (_system_default) *_system_default = afunc->system_default;
    if (_flags) *_flags = afunc->flags;
  }
}

void iupClassRegisterCallback(Iclass* ic, const char* name, const char* format)
{
  /* Since attributes and callbacks do not conflict 
     we can use the same structure to store the callback format using the default_value. */
  iupClassRegisterAttribute(ic, name, NULL, NULL, format, NULL, IUPAF_NO_INHERIT);
}

char* iupClassCallbackGetFormat(Iclass* ic, const char* name)
{
  IattribFunc* afunc = (IattribFunc*)iupTableGet(ic->attrib_func, name);
  if (afunc)
    return (char*)afunc->default_value;
  return NULL;
}

int IupGetClassAttributes(const char* classname, char** names, int n)
{
  Iclass* ic;
  int i = 0;
  char* name;

  iupASSERT(classname!=NULL);
  if (!classname)
    return 0;

  ic = iupRegisterFindClass(classname);
  if (!ic)
    return -1;

  if (!names || !n)
    return iupTableCount(ic->attrib_func);

  name = iupTableFirst(ic->attrib_func);
  while (name)
  {
    names[i] = name;
    i++;
    if (i == n)
      break;

    name = iupTableNext(ic->attrib_func);
  }

  return i;
}

void IupSetClassDefaultAttribute(const char* classname, const char *name, const char* default_value)
{
  Iclass* ic;
  IattribFunc* afunc;

  iupASSERT(classname!=NULL);
  if (!classname)
    return;

  iupASSERT(name!=NULL);
  if (!name)
    return;

  ic = iupRegisterFindClass(name);
  if (!ic)
    return;

  afunc = (IattribFunc*)iupTableGet(ic->attrib_func, name);
  if (afunc && (!(afunc->flags & IUPAF_NO_DEFAULTVALUE) || !(afunc->flags & IUPAF_NO_STRING) || !(afunc->flags & IUPAF_HAS_ID)))
  {
    if (default_value == IUPAF_SAMEASSYSTEM)
      afunc->default_value = afunc->system_default;
    else
      afunc->default_value = default_value;

    if (iClassIsGlobalDefault(afunc->default_value))
      afunc->call_global_default = 1;
    else
      afunc->call_global_default = 0;
  }
  else if (default_value)
    iupClassRegisterAttribute(ic, name, NULL, NULL, default_value, NULL, IUPAF_DEFAULT);
}

void IupSaveClassAttributes(Ihandle* ih)
{
  Iclass* ic;
  char *name;

  iupASSERT(iupObjectCheck(ih));
  if (!iupObjectCheck(ih))
    return;

  ic = ih->iclass;

  name = iupTableFirst(ic->attrib_func);
  while (name)
  {
    IattribFunc* afunc = (IattribFunc*)iupTableGet(ih->iclass->attrib_func, name);
    if (afunc && !(afunc->flags & IUPAF_NO_STRING))
    {
      int inherit;
      char *def_value;
      char *value = iupClassObjectGetAttribute(ih, name, &def_value, &inherit);
      if (value && value != iupAttribGet(ih, name))
        iupAttribStoreStr(ih, name, value);
    }

    name = iupTableNext(ic->attrib_func);
  }
}

void iupClassObjectEnsureDefaultAttributes(Ihandle* ih)
{
  Iclass* ic;
  char *name;

  ic = ih->iclass;

  name = iupTableFirst(ic->attrib_func);
  while (name)
  {
    IattribFunc* afunc = (IattribFunc*)iupTableGetCurr(ic->attrib_func);
    if (afunc && afunc->set && (afunc->default_value || afunc->system_default) &&
        (!(afunc->flags & IUPAF_NO_DEFAULTVALUE) || !(afunc->flags & IUPAF_NO_STRING) || !(afunc->flags & IUPAF_HAS_ID)))
    {
      if ((!iupStrEqualNoCase(afunc->default_value, afunc->system_default)) || 
          (afunc->call_global_default && iupGlobalDefaultColorChanged(afunc->default_value)))
      {
        if ((!ih->handle && (afunc->flags & IUPAF_NOT_MAPPED)) ||
            (ih->handle && !(afunc->flags & IUPAF_NOT_MAPPED) && !iupAttribGet(ih, name)))
          afunc->set(ih, iClassGetDefaultValue(afunc));
      }
    }

    name = iupTableNext(ic->attrib_func);
  }
}

char* iupClassGetDefaultAttribute(const char* classname, const char *attrib_name)
{
  Iclass* ic;
  IattribFunc* afunc;

  iupASSERT(classname!=NULL);
  if (!classname)
    return NULL;

  iupASSERT(attrib_name!=NULL);
  if (!attrib_name)
    return NULL;

  ic = iupRegisterFindClass(classname);
  if (!ic)
    return NULL;

  afunc = (IattribFunc*)iupTableGet(ic->attrib_func, attrib_name);
  if (afunc)
    return (char*)afunc->default_value;
  else
    return NULL;
}
