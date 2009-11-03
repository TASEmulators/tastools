/** \file
 * \brief Ihandle Class Base Functions
 *
 * See Copyright Notice in "iup.h"
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <memory.h>

#include "iup.h"
#include "iupcbs.h"

#include "iup_object.h"
#include "iup_drv.h"
#include "iup_drvfont.h"
#include "iup_str.h"
#include "iup_attrib.h"
#include "iup_assert.h"


void iupBaseCallValueChangedCb(Ihandle* ih)
{
  IFn vc_cb = (IFn)IupGetCallback(ih, "VALUECHANGED_CB");
  if (vc_cb)
    vc_cb(ih);
}

int iupBaseTypeVoidMapMethod(Ihandle* ih)
{
  ih->handle = (InativeHandle*)-1; /* fake value just to indicate that it is already mapped */
  return IUP_NOERROR;
}

char* iupBaseGetWidAttrib(Ihandle *ih)
{
  return (char*)ih->handle;
}

void iupBaseUpdateSizeFromFont(Ihandle* ih)
{
  char* value = iupAttribGet(ih, "SIZE");
  if (!value)
    return;

  iupBaseSetSizeAttrib(ih, value);
}

int iupBaseSetSizeAttrib(Ihandle* ih, const char* value)
{
  if (!value)
  {
    ih->userwidth = 0;
    ih->userheight = 0;
  }
  else
  {
    /* if not specified, the value is 0 */
    int w = 0, h = 0;
    int charwidth, charheight;
    iupdrvFontGetCharSize(ih, &charwidth, &charheight);
    iupStrToIntInt(value, &w, &h, 'x');
    if (w < 0) w = 0;
    if (h < 0) h = 0;

    ih->userwidth = iupWIDTH2RASTER(w, charwidth);
    ih->userheight = iupHEIGHT2RASTER(h, charheight);
  }
  return 1;  /* always save in the hash table, so when FONT is changed SIZE can be updated */
}

char* iupBaseGetSizeAttrib(Ihandle* ih)
{
  char* str;
  int charwidth, charheight, width, height;

  if (ih->handle)
  {
    width = ih->currentwidth;
    height = ih->currentheight;
  }
  else
  {
    width = ih->userwidth;
    height = ih->userheight;
  }

  iupdrvFontGetCharSize(ih, &charwidth, &charheight);
  if (charwidth == 0 || charheight == 0)
    return NULL;  /* if font failed get from the hash table */

  str = iupStrGetMemory(50);
  sprintf(str, "%dx%d", iupRASTER2WIDTH(width, charwidth), 
                        iupRASTER2HEIGHT(height, charheight));
  return str;
}

int iupBaseSetRasterSizeAttrib(Ihandle* ih, const char* value)
{
  if (!value)
  {
    ih->userwidth = 0;
    ih->userheight = 0;
  }
  else
  {
    /* if not specified, the value is 0 */
    int w = 0, h = 0;
    iupStrToIntInt(value, &w, &h, 'x');
    if (w < 0) w = 0;
    if (h < 0) h = 0;
    ih->userwidth = w;
    ih->userheight = h;
  }
  iupAttribSetStr(ih, "SIZE", NULL); /* clear SIZE in hash table */
  return 0;
}

char* iupBaseGetRasterSizeAttrib(Ihandle* ih)
{
  char* str;
  int width, height;

  if (ih->handle)
  {
    width = ih->currentwidth;
    height = ih->currentheight;
  }
  else
  {
    width = ih->userwidth;
    height = ih->userheight;
  }

  if (!width && !height)
    return NULL;

  str = iupStrGetMemory(50);
  sprintf(str, "%dx%d", width, height);
  return str;
}

char* iupBaseGetCharSizeAttrib(Ihandle* ih)
{
  char* str;
  int charwidth, charheight;

  iupdrvFontGetCharSize(ih, &charwidth, &charheight);
  if (charwidth == 0 || charheight == 0)
    return NULL;

  str = iupStrGetMemory(50);
  sprintf(str, "%dx%d", charwidth, charheight);
  return str;
}

static char* iBaseGetPositionAttrib(Ihandle* ih)
{
  char* str = iupStrGetMemory(50);
  sprintf(str, "%d,%d", ih->x, ih->y);
  return str;
}

static int iBaseSetPositionAttrib(Ihandle* ih, const char* value)
{
  if (ih->is_floating)
    iupStrToIntInt(value, &ih->x, &ih->y, ',');
  return 0;
}

char* iupBaseGetActiveAttrib(Ihandle *ih)
{
  if (iupdrvIsActive(ih))
    return "YES";
  else
    return "NO";
}

static int iBaseNativeParentIsActive(Ihandle* ih)
{
  if (!ih->parent)
    return 1;

  if (ih->parent->iclass->nativetype == IUP_TYPEVOID)
    return iBaseNativeParentIsActive(ih->parent);
  else 
    return iupdrvIsActive(ih->parent);
}

int iupBaseSetActiveAttrib(Ihandle* ih, const char* value)
{
  if (iupStrBoolean(value))
  {
    if (iBaseNativeParentIsActive(ih))
      iupdrvSetActive(ih, 1);
  }
  else
    iupdrvSetActive(ih, 0);
  return 0;
}

char* iupBaseGetVisibleAttrib(Ihandle* ih)
{
  if (iupdrvIsVisible(ih))
    return "YES";
  else
    return "NO";
}

int iupBaseSetVisibleAttrib(Ihandle* ih, const char* value)
{
  iupdrvSetVisible(ih, iupStrBoolean(value));
  return 0;
}

char* iupBaseNativeParentGetBgColorAttrib(Ihandle* ih)
{
  /* Used only by those who need a transparent background */
  char* color = iupAttribGetInheritNativeParent(ih, "BGCOLOR");
  if (!color) color = iupAttribGetInheritNativeParent(ih, "BACKGROUND");
  if (!color) color = IupGetGlobal("DLGBGCOLOR");
  return color;
}

char* iupBaseNativeParentGetBgColor(Ihandle* ih)
{
  /* Used in SetBgColorAttrib */
  char* color = iupAttribGetInheritNativeParent(ih, "BGCOLOR");
  if (!color) color = IupGetGlobal("DLGBGCOLOR");
  return color;
}

int iupBaseGetScrollbar(Ihandle* ih)
{
  int sb = IUP_SB_NONE;  /* NO scrollbar by default */
  char* value = IupGetAttribute(ih, "SCROLLBAR");
  if (value)
  {
    if (iupStrEqualNoCase(value, "YES"))
      sb = IUP_SB_HORIZ | IUP_SB_VERT;
    else if (iupStrEqualNoCase(value, "HORIZONTAL"))
      sb = IUP_SB_HORIZ;
    else if (iupStrEqualNoCase(value, "VERTICAL"))
      sb = IUP_SB_VERT;
  }
  return sb;
}

static int iBaseSetNormalizerGroupAttrib(Ihandle* ih, const char* value)
{
  Ihandle* ih_normalizer = IupGetHandle(value);
  if (!ih_normalizer)
  {
    ih_normalizer = IupNormalizer(NULL);
    IupSetHandle(value, ih_normalizer);
  }

  IupSetAttribute(ih_normalizer, "ADDCONTROL_HANDLE", (char*)ih);
  return 1;
}

static Ihandle* iBaseFindChild(Ihandle* ih, const char* name)
{
  Ihandle* child = ih->firstchild;
  while (child)
  {
    char* child_name = iupAttribGet(child, "NAME");
    if (child_name && iupStrEqualNoCase(name, child_name))
      return child;

    if (child->firstchild)
    {
      Ihandle* c = iBaseFindChild(child, name);
      if (c) return c;
    }

    child = child->brother;
  }
  return NULL;
}

Ihandle* IupGetDialogChild(Ihandle* ih, const char* name)
{
  Ihandle *child, *dialog;
  char attrib[1024] = "_IUP_DIALOG_CHILD_";

  iupASSERT(iupObjectCheck(ih));
  if (!iupObjectCheck(ih))
    return NULL;

  if (!name)
    return NULL;

  dialog = IupGetDialog(ih);
  if (dialog) ih = dialog;
 
  strcat(attrib, name);
  child = (Ihandle*)iupAttribGet(ih, attrib);
  if (child) return child;

  if (ih->firstchild)
  {
    child = iBaseFindChild(ih, name);
    if (child) return child;
  }
  return NULL;
}

int iupBaseSetNameAttrib(Ihandle* ih, const char* value)
{
  Ihandle* dialog = IupGetDialog(ih);
  if (dialog)
  {
    char attrib[1024] = "_IUP_DIALOG_CHILD_";
    strcat(attrib, value);
    iupAttribSetStr(dialog, attrib, (char*)ih);
  }
  return 1;
}

static int iBaseSetFloatingAttrib(Ihandle* ih, const char* value)
{
  ih->is_floating = iupStrBoolean(value);
  return 0;
}

static char* iBaseGetFloatingAttrib(Ihandle* ih)
{
  if (ih->is_floating)
    return "YES";
  else
    return "NO";
}

static int iBaseSetMaxSizeAttrib(Ihandle* ih, const char* value)
{
  if (value)
    ih->has_maxsize = 1;
  else
    ih->has_maxsize = 0;
  return 1;
}

static int iBaseSetMinSizeAttrib(Ihandle* ih, const char* value)
{
  if (value)
    ih->has_minsize = 1;
  else
    ih->has_minsize = 0;
  return 1;
}

static int iBaseSetExpandAttrib(Ihandle* ih, const char* value)
{
  if (iupStrEqualNoCase(value, "YES"))
    ih->expand = IUP_EXPAND_BOTH;
  else if (iupStrEqualNoCase(value, "HORIZONTAL"))
    ih->expand = IUP_EXPAND_WIDTH;
  else if (iupStrEqualNoCase(value, "VERTICAL"))
    ih->expand = IUP_EXPAND_HEIGHT;
  else
    ih->expand = IUP_EXPAND_NONE;
  return 0;
}

static char* iBaseGetExpandAttrib(Ihandle* ih)
{
  if (ih->expand & IUP_EXPAND_BOTH)
    return "YES";
  else if (ih->expand & IUP_EXPAND_WIDTH)
    return "HORIZONTAL";
  else if (ih->expand & IUP_EXPAND_HEIGHT)
    return "VERTICAL";
  else
    return "NO";
}

void iupBaseContainerUpdateExpand(Ihandle* ih)
{
  char *expand = iupAttribGetInherit(ih, "EXPAND");
  if (!expand)
    ih->expand = IUP_EXPAND_BOTH;  /* default for containers is YES */
  else
  {
    if (iupStrEqualNoCase(expand, "NO"))
      ih->expand = IUP_EXPAND_NONE;
    else if (iupStrEqualNoCase(expand, "HORIZONTAL"))
      ih->expand = IUP_EXPAND_WIDTH;
    else if (iupStrEqualNoCase(expand, "VERTICAL"))
      ih->expand = IUP_EXPAND_HEIGHT;
    else
      ih->expand = IUP_EXPAND_BOTH;  /* default for containers is YES */
  }
}

/* EXPAND is registered with IUP_NO_INHERIT because it is not inheritable,
   but it is inheritable for containers. 
   So if you set at a container it will not affect its children,
   but children that are containers will get the same value. */
char* iupBaseContainerGetExpandAttrib(Ihandle* ih)
{
  return iupAttribGetInherit(ih, "EXPAND");
}

void iupBaseRegisterCommonAttrib(Iclass* ic)
{
  iupClassRegisterAttribute(ic, "WID", iupBaseGetWidAttrib, NULL, NULL, NULL, IUPAF_READONLY|IUPAF_NO_INHERIT|IUPAF_NO_STRING);
  iupClassRegisterAttribute(ic, "NAME", NULL, iupBaseSetNameAttrib, NULL, NULL, IUPAF_NO_DEFAULTVALUE|IUPAF_NOT_MAPPED|IUPAF_NO_INHERIT);
  iupClassRegisterAttribute(ic, "FLOATING", iBaseGetFloatingAttrib, iBaseSetFloatingAttrib, NULL, NULL, IUPAF_NOT_MAPPED|IUPAF_NO_INHERIT);
  iupClassRegisterAttribute(ic, "EXPAND", iBaseGetExpandAttrib, iBaseSetExpandAttrib, NULL, NULL, IUPAF_NOT_MAPPED|IUPAF_NO_INHERIT);
  iupClassRegisterAttribute(ic, "NORMALIZERGROUP", NULL, iBaseSetNormalizerGroupAttrib, NULL, NULL, IUPAF_NOT_MAPPED|IUPAF_NO_INHERIT);

  /* make sure everyone has the correct default value */
  iupClassRegisterAttribute(ic, "VISIBLE", NULL, NULL, IUPAF_SAMEASSYSTEM, "YES", IUPAF_DEFAULT);
  iupClassRegisterAttribute(ic, "ACTIVE", NULL, NULL, IUPAF_SAMEASSYSTEM, "YES", IUPAF_DEFAULT);
  if (ic->is_interactive)
    iupClassRegisterAttribute(ic, "CANFOCUS", NULL, NULL, IUPAF_SAMEASSYSTEM, "YES", IUPAF_NO_INHERIT);
  else
    iupClassRegisterAttribute(ic, "CANFOCUS", NULL, NULL, IUPAF_SAMEASSYSTEM, "NO", IUPAF_NO_INHERIT);

  iupClassRegisterAttribute(ic, "SIZE", iupBaseGetSizeAttrib, iupBaseSetSizeAttrib, NULL, NULL, IUPAF_NO_DEFAULTVALUE|IUPAF_NOT_MAPPED|IUPAF_NO_INHERIT);
  iupClassRegisterAttribute(ic, "RASTERSIZE", iupBaseGetRasterSizeAttrib, iupBaseSetRasterSizeAttrib, NULL, NULL, IUPAF_NO_DEFAULTVALUE|IUPAF_NOT_MAPPED|IUPAF_NO_INHERIT);
  iupClassRegisterAttribute(ic, "CHARSIZE", iupBaseGetCharSizeAttrib, NULL, NULL, NULL, IUPAF_NO_DEFAULTVALUE|IUPAF_READONLY|IUPAF_NOT_MAPPED|IUPAF_NO_INHERIT);

  iupClassRegisterAttribute(ic, "POSITION", iBaseGetPositionAttrib, iBaseSetPositionAttrib, NULL, NULL, IUPAF_NO_DEFAULTVALUE|IUPAF_NOT_MAPPED|IUPAF_NO_INHERIT);
  iupClassRegisterAttribute(ic, "MAXSIZE", NULL, iBaseSetMaxSizeAttrib, IUPAF_SAMEASSYSTEM, "65535x65535", IUPAF_NOT_MAPPED|IUPAF_NO_INHERIT);
  iupClassRegisterAttribute(ic, "MINSIZE", NULL, iBaseSetMinSizeAttrib, IUPAF_SAMEASSYSTEM, "0x0", IUPAF_NOT_MAPPED|IUPAF_NO_INHERIT);

  iupClassRegisterAttribute(ic, "STANDARDFONT", NULL, iupdrvSetStandardFontAttrib, IUPAF_SAMEASSYSTEM, "DEFAULTFONT", IUPAF_NOT_MAPPED);  /* use inheritance to retrieve standard fonts */
  iupClassRegisterAttribute(ic, "FONT", iupGetFontAttrib, iupSetFontAttrib, IUPAF_SAMEASSYSTEM, "DEFAULTFONT", IUPAF_NOT_MAPPED|IUPAF_NO_INHERIT);

  iupClassRegisterAttribute(ic, "FONTSTYLE", iupGetFontStyleAttrib, iupSetFontStyleAttrib, NULL, NULL, IUPAF_NOT_MAPPED|IUPAF_NO_INHERIT);
  iupClassRegisterAttribute(ic, "FONTSIZE", iupGetFontSizeAttrib, iupSetFontSizeAttrib, NULL, NULL, IUPAF_NOT_MAPPED|IUPAF_NO_INHERIT);
  iupClassRegisterAttribute(ic, "FONTFACE", iupGetFontFaceAttrib, NULL, NULL, NULL, IUPAF_READONLY|IUPAF_NOT_MAPPED|IUPAF_NO_INHERIT);

  iupdrvBaseRegisterCommonAttrib(ic);
}

void iupBaseRegisterVisualAttrib(Iclass* ic)
{
  iupClassRegisterAttribute(ic, "VISIBLE", iupBaseGetVisibleAttrib, iupBaseSetVisibleAttrib, IUPAF_SAMEASSYSTEM, "YES", IUPAF_NO_INHERIT);  /* VISIBLE inheritance comes from the native system */
  iupClassRegisterAttribute(ic, "ACTIVE", iupBaseGetActiveAttrib, iupBaseSetActiveAttrib, IUPAF_SAMEASSYSTEM, "YES", IUPAF_DEFAULT);

  iupClassRegisterAttribute(ic, "ZORDER", NULL, iupdrvBaseSetZorderAttrib, NULL, NULL, IUPAF_WRITEONLY|IUPAF_NO_INHERIT);
  iupClassRegisterAttribute(ic, "X", iupdrvBaseGetXAttrib, NULL, NULL, NULL, IUPAF_READONLY|IUPAF_NO_INHERIT);
  iupClassRegisterAttribute(ic, "Y", iupdrvBaseGetYAttrib, NULL, NULL, NULL, IUPAF_READONLY|IUPAF_NO_INHERIT);

  iupClassRegisterAttribute(ic, "TIP", NULL, iupdrvBaseSetTipAttrib, NULL, NULL, IUPAF_NO_DEFAULTVALUE|IUPAF_NO_INHERIT);
  iupClassRegisterAttribute(ic, "TIPVISIBLE", NULL, iupdrvBaseSetTipVisibleAttrib, NULL, NULL, IUPAF_NO_INHERIT);
  iupClassRegisterAttribute(ic, "TIPDELAY", NULL, NULL, IUPAF_SAMEASSYSTEM, "5000", IUPAF_NOT_MAPPED);          /* 5 seconds */
  iupClassRegisterAttribute(ic, "TIPBGCOLOR", NULL, NULL, IUPAF_SAMEASSYSTEM, "255 255 225", IUPAF_NOT_MAPPED); /* Light Yellow */
  iupClassRegisterAttribute(ic, "TIPFGCOLOR", NULL, NULL, IUPAF_SAMEASSYSTEM, "0 0 0", IUPAF_NOT_MAPPED);       /* black */
}

void iupBaseRegisterCommonCallbacks(Iclass* ic)
{
  iupClassRegisterAttribute(ic, "MAP_CB", NULL, NULL, "", NULL, IUPAF_NO_INHERIT);
  iupClassRegisterAttribute(ic, "UNMAP_CB", NULL, NULL, "", NULL, IUPAF_NO_INHERIT);
  iupClassRegisterAttribute(ic, "GETFOCUS_CB", NULL, NULL, "", NULL, IUPAF_NO_INHERIT);
  iupClassRegisterAttribute(ic, "KILLFOCUS_CB", NULL, NULL, "", NULL, IUPAF_NO_INHERIT);
  iupClassRegisterAttribute(ic, "ENTERWINDOW_CB", NULL, NULL, "", NULL, IUPAF_NO_INHERIT);
  iupClassRegisterAttribute(ic, "LEAVEWINDOW_CB", NULL, NULL, "", NULL, IUPAF_NO_INHERIT);
  iupClassRegisterAttribute(ic, "HELP_CB", NULL, NULL, "", NULL, IUPAF_NO_INHERIT);
  iupClassRegisterAttribute(ic, "K_ANY", NULL, NULL, "i", NULL, IUPAF_NO_INHERIT);
}
