/** \file
 * \brief IupFontDlg pre-defined dialog
 *
 * See Copyright Notice in "iup.h"
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <memory.h>
#include <stdarg.h>
#include <limits.h>

#include "iup.h"

#include "iup_object.h"
#include "iup_attrib.h"
#include "iup_str.h"
#include "iup_stdcontrols.h"


Ihandle* IupFontDlg(void)
{
  return IupCreate("fontdlg");
}

Iclass* iupFontDlgGetClass(void)
{
  Iclass* ic = iupClassNew(iupDialogGetClass());

  ic->name = "fontdlg";
  ic->nativetype = IUP_TYPEDIALOG;
  ic->is_interactive = 1;

  /* reset not used native dialog methods */
  ic->parent->LayoutUpdate = NULL;
  ic->parent->SetChildrenPosition = NULL;
  ic->parent->Map = NULL;
  ic->parent->UnMap = NULL;

  /* IupFontDialog only */
  iupClassRegisterAttribute(ic, "STATUS", NULL, NULL, NULL, NULL, IUPAF_NO_INHERIT|IUPAF_READONLY);
  iupClassRegisterAttribute(ic, "VALUE", NULL, NULL, NULL, NULL, IUPAF_NO_INHERIT);

  iupdrvFontDlgInitClass(ic);

  return ic;
}
