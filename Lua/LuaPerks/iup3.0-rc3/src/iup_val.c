/** \file
 * \brief Valuator Control
 *
 * See Copyright Notice in "iup.h"
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "iup.h"
#include "iupcbs.h"
#include "iupkey.h"
#include "iupcontrols.h"

#include "iup_object.h"
#include "iup_attrib.h"
#include "iup_str.h"
#include "iup_drv.h"
#include "iup_stdcontrols.h"
#include "iup_layout.h"
#include "iup_val.h"


void iupValCropValue(Ihandle* ih)
{
  if (ih->data->val > ih->data->vmax) 
    ih->data->val = ih->data->vmax;
  else if (ih->data->val < ih->data->vmin) 
    ih->data->val = ih->data->vmin;
}

char* iupValGetShowTicksAttrib(Ihandle* ih)
{
  char* str = iupStrGetMemory(30);
  sprintf(str, "%d", ih->data->show_ticks);
  return str;
}

char* iupValGetValueAttrib(Ihandle* ih)
{
  char* str = iupStrGetMemory(30);
  sprintf(str, "%g", ih->data->val);
  return str;
}

char* iupValGetStepAttrib(Ihandle* ih)
{
  char* str = iupStrGetMemory(30);
  sprintf(str, "%g", ih->data->step);
  return str;
}

char* iupValGetPageStepAttrib(Ihandle* ih)
{
  char* str = iupStrGetMemory(30);
  sprintf(str, "%g", ih->data->pagestep);
  return str;
}

static int iValSetMaxAttrib(Ihandle* ih, const char* value)
{
  ih->data->vmax = atof(value);
  iupValCropValue(ih);
  return 0; /* do not store value in hash table */
}

static char* iValGetMaxAttrib(Ihandle* ih)
{
  char* str = iupStrGetMemory(30);
  sprintf(str, "%g", ih->data->vmax);
  return str;
}

static int iValSetMinAttrib(Ihandle* ih, const char* value)
{
  ih->data->vmin = atof(value);
  iupValCropValue(ih);
  return 0; /* do not store value in hash table */
}

static char* iValGetMinAttrib(Ihandle* ih)
{
  char* str = iupStrGetMemory(30);
  sprintf(str, "%g", ih->data->vmin);
  return str;
}

static int iValSetTypeAttrib(Ihandle* ih, const char *value)
{
  int min_w, min_h;

  /* valid only before map */
  if (ih->handle)
    return 0;

  iupdrvValGetMinSize(ih, &min_w, &min_h);

  if (iupStrEqualNoCase(value, "VERTICAL"))
  {
    /* val natural vertical size is MinWx100 */
    IupSetfAttribute(ih, "RASTERSIZE", "%dx%d", min_w, 100);
    ih->data->type = IVAL_VERTICAL;
  }
  else /* "HORIZONTAL" */
  {
    /* val natural horizontal size is 100xMinH */
    IupSetfAttribute(ih, "RASTERSIZE", "%dx%d", 100, min_h);
    ih->data->type = IVAL_HORIZONTAL;
  }

  return 0; /* do not store value in hash table */
}

static char* iValGetTypeAttrib(Ihandle* ih)
{
  if (ih->data->type == IVAL_HORIZONTAL)
    return "HORIZONTAL";
  else /* (ih->data->type == IVAL_VERTICAL) */
    return "VERTICAL";
}

static int iValSetInvertedAttrib(Ihandle* ih, const char *value)
{
  /* valid only before map */
  if (ih->handle)
    return 0;

  if (iupStrBoolean(value))
    ih->data->inverted = 1;
  else
    ih->data->inverted = 0;

  return 0; /* do not store value in hash table */
}

static char* iValGetInvertedAttrib(Ihandle* ih)
{
  if (ih->data->inverted)
    return "YES";
  else
    return "NO";
}

static int iValCreateMethod(Ihandle* ih, void **params)
{
  char* type = "HORIZONTAL";
  if (params && params[0])
    type = params[0];

  ih->data = iupALLOCCTRLDATA();

  iValSetTypeAttrib(ih, type);
  if (ih->data->type == IVAL_VERTICAL)
    ih->data->inverted = 1;  /* default is YES when vertical */

  ih->data->vmax = 1.00;
  ih->data->step = 0.01;
  ih->data->pagestep = 0.10;

  return IUP_NOERROR; 
}

Iclass* iupValGetClass(void)
{
  Iclass* ic = iupClassNew(NULL);

  ic->name = "val";
  ic->format = "S"; /* one optional string */
  ic->nativetype = IUP_TYPECONTROL;
  ic->childtype = IUP_CHILDNONE;
  ic->is_interactive = 1;

  /* Class functions */
  ic->Create  = iValCreateMethod;
  ic->LayoutUpdate = iupdrvBaseLayoutUpdateMethod;
  ic->UnMap = iupdrvBaseUnMapMethod;

  /* Callbacks */
  iupClassRegisterCallback(ic, "VALUECHANGED_CB", "");

  /* Common Callbacks */
  iupBaseRegisterCommonCallbacks(ic);

  /* Common */
  iupBaseRegisterCommonAttrib(ic);

  /* Visual */
  iupBaseRegisterVisualAttrib(ic);

  /* IupVal only */
  iupClassRegisterAttribute(ic, "MAX", iValGetMaxAttrib, iValSetMaxAttrib, IUPAF_SAMEASSYSTEM, "1.0", IUPAF_NOT_MAPPED);
  iupClassRegisterAttribute(ic, "MIN", iValGetMinAttrib, iValSetMinAttrib, IUPAF_SAMEASSYSTEM, "0.0", IUPAF_NOT_MAPPED);
  iupClassRegisterAttribute(ic, "TYPE", iValGetTypeAttrib, iValSetTypeAttrib, IUPAF_SAMEASSYSTEM, "HORIZONTAL", IUPAF_NOT_MAPPED|IUPAF_NO_INHERIT);
  iupClassRegisterAttribute(ic, "INVERTED", iValGetInvertedAttrib, iValSetInvertedAttrib, NULL, NULL, IUPAF_NOT_MAPPED|IUPAF_NO_INHERIT);

  iupdrvValInitClass(ic);

  return ic;
}

Ihandle *IupVal(const char *type)
{
  void *params[2];
  params[0] = (void*)type;
  params[1] = NULL;
  return IupCreatev("val", params);
}
