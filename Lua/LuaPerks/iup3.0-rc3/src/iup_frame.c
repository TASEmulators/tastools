/** \file
 * \brief Frame Control.
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
#include "iup_frame.h"


int iupFrameGetTitleHeight(Ihandle* ih)
{
  int charheight;
  iupdrvFontGetCharSize(ih, NULL, &charheight);
  return charheight;
}

static void iFrameGetDecorSize(Ihandle* ih, int *width, int *height)
{
  *width  = 5;
  *height = 5;

  if (iupAttribGet(ih, "_IUPFRAME_HAS_TITLE") || iupAttribGet(ih, "TITLE"))
  {
    (*height) += iupFrameGetTitleHeight(ih);
  }
}

static char* iFrameGetClientSizeAttrib(Ihandle* ih)
{
  int width, height, decorwidth, decorheight;
  char* str = iupStrGetMemory(20);
  width = ih->currentwidth;
  height = ih->currentheight;
  iFrameGetDecorSize(ih, &decorwidth, &decorheight);
  width -= decorwidth;
  height -= decorheight;
  if (width < 0) width = 0;
  if (height < 0) height = 0;
  sprintf(str, "%dx%d", width, height);
  return str;
}

static int iFrameCreateMethod(Ihandle* ih, void** params)
{
  if (params)
  {
    Ihandle** iparams = (Ihandle**)params;
    if (*iparams)
      IupAppend(ih, *iparams);
  }

  return IUP_NOERROR;
}

static void iFrameComputeNaturalSizeMethod(Ihandle* ih, int *w, int *h, int *expand)
{
  int decorwidth, decorheight;
  Ihandle* child = ih->firstchild;

  iFrameGetDecorSize(ih, &decorwidth, &decorheight);
  *w = decorwidth;
  *h = decorheight;

  if (child)
  {
    /* update child natural size first */
    iupBaseComputeNaturalSize(child);

    *expand = child->expand;
    *w += child->naturalwidth;
    *h += child->naturalheight;
  }
}

static void iFrameSetChildrenCurrentSizeMethod(Ihandle* ih, int shrink)
{
  int width, height, decorwidth, decorheight;

  iFrameGetDecorSize(ih, &decorwidth, &decorheight);

  width = ih->currentwidth-decorwidth;
  height = ih->currentheight-decorheight;
  if (width < 0) width = 0;
  if (height < 0) height = 0;

  iupBaseSetCurrentSize(ih->firstchild, width, height, shrink);
}

static void iFrameSetChildrenPositionMethod(Ihandle* ih, int x, int y)
{
  /* IupFrame is the native parent of its children,
     so the position is restarted at (0,0) */

  iupdrvFrameGetDecorOffset(ih, &x, &y);

  /* Child coordinates are relative to client left-top corner. */
  iupBaseSetPosition(ih->firstchild, x, y);
}


/******************************************************************************/


Ihandle* IupFrame(Ihandle* child)
{
  void *params[2];
  params[0] = (void*)child;
  params[1] = NULL;
  return IupCreatev("frame", params);
}

Iclass* iupFrameGetClass(void)
{
  Iclass* ic = iupClassNew(NULL);

  ic->name = "frame";
  ic->format = "H"; /* one optional ihandle */
  ic->nativetype = IUP_TYPECONTROL;
  ic->childtype = IUP_CHILD_ONE;
  ic->is_interactive = 0;

  /* Class functions */
  ic->Create = iFrameCreateMethod;

  ic->ComputeNaturalSize = iFrameComputeNaturalSizeMethod;
  ic->SetChildrenCurrentSize = iFrameSetChildrenCurrentSizeMethod;
  ic->SetChildrenPosition = iFrameSetChildrenPositionMethod;

  ic->LayoutUpdate = iupdrvBaseLayoutUpdateMethod;
  ic->UnMap = iupdrvBaseUnMapMethod;

  /* Common Callbacks */
  iupClassRegisterCallback(ic, "MAP_CB", "");
  iupClassRegisterCallback(ic, "UNMAP_CB", "");

  /* Common */
  iupBaseRegisterCommonAttrib(ic);

  /* Visual */
  iupBaseRegisterVisualAttrib(ic);

  /* Base Container */
  iupClassRegisterAttribute(ic, "CLIENTSIZE", iFrameGetClientSizeAttrib, NULL, NULL, NULL, IUPAF_READONLY|IUPAF_NO_INHERIT);
  iupClassRegisterAttribute(ic, "EXPAND", iupBaseContainerGetExpandAttrib, NULL, IUPAF_SAMEASSYSTEM, "YES", IUPAF_NOT_MAPPED|IUPAF_NO_INHERIT);

  /* IupFrame only */
  iupClassRegisterAttribute(ic, "SUNKEN", NULL, NULL, NULL, NULL, IUPAF_NO_INHERIT);

  iupdrvFrameInitClass(ic);

  return ic;
}
