/** \file
 * \brief global attributes enviroment
 *
 * See Copyright Notice in "iup.h"
 */

#include <stdlib.h>      
#include <stdio.h>      
#include <string.h>      

#include "iup.h" 

#include "iup_table.h"
#include "iup_globalattrib.h"
#include "iup_drv.h"
#include "iup_drvfont.h"
#include "iup_assert.h"
#include "iup_str.h"


static Itable *iglobal_table = NULL;

void iupGlobalAttribInit(void)
{
  iglobal_table = iupTableCreate(IUPTABLE_STRINGINDEXED);
}

void iupGlobalAttribFinish(void)
{
  iupTableDestroy(iglobal_table);
  iglobal_table = NULL;
}

static int iGlobalChangingDefaultColor(const char *name)
{
  if (iupStrEqual(name, "DLGBGCOLOR") ||
      iupStrEqual(name, "DLGFGCOLOR") ||
      iupStrEqual(name, "MENUBGCOLOR") ||
      iupStrEqual(name, "MENUFGCOLOR") ||
      iupStrEqual(name, "TXTBGCOLOR") ||
      iupStrEqual(name, "TXTFGCOLOR"))
  {
    char str[50] = "_IUP_USER_DEFAULT_";
    strcat(str, name);
    iupTableSet(iglobal_table, str, (void*)"1", IUPTABLE_POINTER);  /* mark as changed by the User */
    return 1;
  }
  return 0;
}

int iupGlobalDefaultColorChanged(const char *name)
{
  char str[50] = "_IUP_USER_DEFAULT_";
  strcat(str, name);
  return iupTableGet(iglobal_table, str) != NULL;
}

void iupGlobalSetDefaultColorAttrib(const char* name, int r, int g, int b)
{
  if (!iupGlobalDefaultColorChanged(name))
  {
    char value[50];
    sprintf(value, "%3d %3d %3d", r, g, b);
    iupTableSet(iglobal_table, name, (void*)value, IUPTABLE_STRING);
  }
}

void IupSetGlobal(const char *name, const char *value)
{
  iupASSERT(name!=NULL);
  if (!name) return;

  if (iupStrEqual(name, "DEFAULTFONTSIZE"))
  {
    iupSetDefaultFontSizeGlobalAttrib(value);
    return;
  }

  if (iGlobalChangingDefaultColor(name) || iupdrvSetGlobal(name, value))
  {
    if (!value)
      iupTableRemove(iglobal_table, name);
    else
      iupTableSet(iglobal_table, name, (void*)value, IUPTABLE_POINTER);
  }
}

void IupStoreGlobal(const char *name, const char *value)
{
  iupASSERT(name!=NULL);
  if (!name) return;

  if (iupStrEqual(name, "DEFAULTFONTSIZE"))
  {
    iupSetDefaultFontSizeGlobalAttrib(value);
    return;
  }

  if (iGlobalChangingDefaultColor(name) || iupdrvSetGlobal(name, value))
  {
    if (!value)
      iupTableRemove(iglobal_table, name);
    else
      iupTableSet(iglobal_table, name, (void*)value, IUPTABLE_STRING);
  }
}

char *IupGetGlobal(const char *name)
{
  char* value;
  
  iupASSERT(name!=NULL);
  if (!name) 
    return NULL;

  if (iupStrEqual(name, "DEFAULTFONTSIZE"))
    return iupGetDefaultFontSizeGlobalAttrib();

  value = iupdrvGetGlobal(name);

  if (!value)
    value = (char*)iupTableGet(iglobal_table, name);

  return value;
}

int iupGlobalIsPointer(const char* name)
{
  static struct {
    const char *name;
  } ptr_table[] = {
#ifdef WIN32
    {"HINSTANCE"},
#else
    {"XDISPLAY"},
    {"XSCREEN"},
    {"APPSHELL"},
#endif
  };
#define PTR_TABLE_SIZE ((sizeof ptr_table)/(sizeof ptr_table[0]))

  if (name)
  {
    int i;
    for (i = 0; i < PTR_TABLE_SIZE; i++)
    {
      if (iupStrEqualNoCase(name, ptr_table[i].name))
        return 1;
    }
  }

  return 0;
}
