/** \file
 * \brief Radio Control.
 *
 * See Copyright Notice in "iup.h"
 */

#include <stdio.h>
#include <stdlib.h>

#include "iup.h"
#include "iupcbs.h"

#include "iup_object.h"
#include "iup_attrib.h"
#include "iup_str.h"
#include "iup_drv.h"
#include "iup_drvfont.h"
#include "iup_stdcontrols.h"
#include "iup_layout.h"


Ihandle *iupRadioFindToggleParent(Ihandle* ih_toggle)
{
  Ihandle *p;
  for (p=ih_toggle; p->parent; p=p->parent)
  {
    if (p->iclass->nativetype == IUP_TYPEVOID &&
        iupStrEqual(p->iclass->name, "radio"))
      return p;
  }

  return NULL;
}

static int iRadioFindToggleChild(Ihandle* ih, Ihandle* ih_toggle)
{
  Ihandle* child;

  if (ih == ih_toggle) /* found child that match the toggle */
    return 1;

  for (child = ih->firstchild; child; child = child->brother)
  {
    if (iRadioFindToggleChild(child, ih_toggle))
      return 1;
  }

  return 0;
}

static Ihandle* iRadioGetToggleChildOn(Ihandle* ih)
{
  Ihandle* child;

  if (iupStrEqual(ih->iclass->name, "toggle") &&   /* found child that is a toggle and it is ON */
      IupGetInt(ih, "VALUE"))
    return ih;

  for (child = ih->firstchild; child; child = child->brother)
  {
    Ihandle* ih_toggle = iRadioGetToggleChildOn(child);
    if (ih_toggle)
      return ih_toggle;
  }

  return NULL;
}

/******************************************************************************/


static int iRadioSetValueHandleAttrib(Ihandle* ih, const char* value)
{
  Ihandle* ih_toggle = (Ihandle*)value;
  if (!iupObjectCheck(ih_toggle))
    return 0;

  if (!iupStrEqual(ih_toggle->iclass->name, "toggle"))
    return 0;

  if (iRadioFindToggleChild(ih->firstchild, ih_toggle))
    IupSetAttribute(ih_toggle, "VALUE", "ON");
 
  return 0;
}

char* iRadioGetValueHandleAttrib(Ihandle* ih)
{
  return (char*)iRadioGetToggleChildOn(ih->firstchild);
}

static int iRadioSetValueAttrib(Ihandle* ih, const char* value)
{
  Ihandle *ih_toggle;

  if (!value)
    return 0;

  ih_toggle = IupGetHandle(value);
  if (!ih_toggle)
    return 0;

  iRadioSetValueHandleAttrib(ih, (char*)ih_toggle);
  return 0;
}

char* iRadioGetValueAttrib(Ihandle* ih)
{
  Ihandle *ih_toggle = (Ihandle*)iRadioGetValueHandleAttrib(ih);
  if (!ih_toggle)
    return NULL;

  return IupGetName(ih_toggle);
}

/******************************************************************************/


static int iRadioCreateMethod(Ihandle* ih, void** params)
{
  if (params)
  {
    Ihandle** iparams = (Ihandle**)params;
    if (*iparams)
      IupAppend(ih, *iparams);
  }
  return IUP_NOERROR;
}

static void iRadioComputeNaturalSizeMethod(Ihandle* ih, int *w, int *h, int *expand)
{
  Ihandle* child = ih->firstchild;
  if (child)
  {
    /* update child natural size first */
    iupBaseComputeNaturalSize(child);

    *expand = child->expand;
    *w = child->naturalwidth;
    *h = child->naturalheight;
  }
}

static void iRadioSetChildrenCurrentSizeMethod(Ihandle* ih, int shrink)
{
  iupBaseSetCurrentSize(ih->firstchild, ih->currentwidth, ih->currentheight, shrink);
}

static void iRadioSetChildrenPositionMethod(Ihandle* ih, int x, int y)
{
  iupBaseSetPosition(ih->firstchild, x, y);
}


/******************************************************************************/


Ihandle* IupRadio(Ihandle* child)
{
  void *params[2];
  params[0] = (void*)child;
  params[1] = NULL;
  return IupCreatev("radio", params);
}

Iclass* iupRadioGetClass(void)
{
  Iclass* ic = iupClassNew(NULL);

  ic->name = "radio";
  ic->format = "H"; /* one optional ihandle */
  ic->nativetype = IUP_TYPEVOID;
  ic->childtype = IUP_CHILD_ONE;
  ic->is_interactive = 0;

  /* Class functions */
  ic->Create = iRadioCreateMethod;
  ic->Map = iupBaseTypeVoidMapMethod;
  ic->ComputeNaturalSize = iRadioComputeNaturalSizeMethod;
  ic->SetChildrenCurrentSize = iRadioSetChildrenCurrentSizeMethod;
  ic->SetChildrenPosition = iRadioSetChildrenPositionMethod;

  /* Common */
  iupBaseRegisterCommonAttrib(ic);

  /* Base Container */
  iupClassRegisterAttribute(ic, "EXPAND", iupBaseContainerGetExpandAttrib, NULL, IUPAF_SAMEASSYSTEM, "YES", IUPAF_NOT_MAPPED|IUPAF_NO_INHERIT);
  iupClassRegisterAttribute(ic, "CLIENTSIZE", iupBaseGetRasterSizeAttrib, NULL, NULL, NULL, IUPAF_READONLY|IUPAF_NOT_MAPPED|IUPAF_NO_INHERIT);

  /* Radio only */
  iupClassRegisterAttribute(ic, "VALUE", iRadioGetValueAttrib, iRadioSetValueAttrib, NULL, NULL, IUPAF_NOT_MAPPED|IUPAF_NO_INHERIT);
  iupClassRegisterAttribute(ic, "VALUE_HANDLE", iRadioGetValueHandleAttrib, iRadioSetValueHandleAttrib, NULL, NULL, IUPAF_NOT_MAPPED|IUPAF_NO_INHERIT|IUPAF_NO_STRING);

  return ic;
}
