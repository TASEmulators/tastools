/** \file
 * \brief iupmatrix focus control
 *
 * See Copyright Notice in "iup.h"
 */
 
#include <stdlib.h>

#include "iup.h"
#include "iupcbs.h"

#include <cd.h>

#include "iup_object.h"
#include "iup_attrib.h"
#include "iup_str.h"
#include "iup_stdcontrols.h"

#include "iupmat_def.h"
#include "iupmat_aux.h"
#include "iupmat_focus.h"
#include "iupmat_draw.h"


void iupMatrixFocusSet(Ihandle* ih, int lin, int col)
{
  ih->data->lines.focus_cell = lin;
  ih->data->columns.focus_cell = col;
}

int iupMatrixFocus_CB(Ihandle* ih, int focus)
{
  int rc = IUP_DEFAULT;

  if (!iupMatrixIsValid(ih, 1))
    return IUP_DEFAULT;

  if (iupStrEqualNoCase(IupGetGlobal("DRIVER"), "Motif"))
  {
    if (focus && iupAttribGet(ih, "_IUPMAT_DOUBLE_CLICK"))
    {
      iupAttribSetStr(ih, "_IUPMAT_DOUBLE_CLICK", NULL);
      return IUP_DEFAULT;
    }
  }

  ih->data->has_focus = focus;
  iupMatrixDrawUpdate(ih);

  if (focus)
    iupMatrixAuxCallEnterCellCb(ih);
  else
    iupMatrixAuxCallLeaveCellCb(ih);

  return rc;
}


