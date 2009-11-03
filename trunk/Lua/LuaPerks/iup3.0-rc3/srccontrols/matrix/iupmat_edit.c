/** \file
* \brief iupmatrix edit
* Functions used to edit a node name in place.
*
* See Copyright Notice in "iup.h"
*/

#include <stdlib.h>
#include <string.h>

#include "iup.h"
#include "iupcbs.h"
#include "iupcontrols.h"

#include <cd.h>

#include "iup_object.h"
#include "iup_attrib.h"
#include "iup_str.h"
#include "iup_stdcontrols.h"
#include "iup_childtree.h"

#include "iupmat_def.h"
#include "iupmat_scroll.h"
#include "iupmat_aux.h"
#include "iupmat_edit.h"
#include "iupmat_key.h"
#include "iupmat_getset.h"
#include "iupmat_draw.h"


static int iMatrixEditCallDropdownCb(Ihandle* ih, int lin, int col)
{
  IFnnii cb = (IFnnii)IupGetCallback(ih, "DROP_CB");
  if(cb)
  {
    int ret;
    char* value = iupMatrixCellGetValue(ih, lin, col);
    if (!value) value = "";

    iupAttribSetStr(ih->data->droph, "PREVIOUSVALUE", value);
    IupSetAttribute(ih->data->droph, "VALUE", "1");

    ret = cb(ih, ih->data->droph, lin, col);

    /* check if the user set an invalid value */
    if (IupGetInt(ih->data->droph, "VALUE") == 0)
      IupSetAttribute(ih->data->droph, "VALUE", "1");

    if(ret == IUP_DEFAULT)
      return 1;
  }

  return 0;
}

static int iMatrixEditDropDownAction_CB(Ihandle* ih, char* t, int i, int v)
{
  Ihandle* ih_matrix = ih->parent;
  IFniinsii cb = (IFniinsii)IupGetCallback(ih_matrix, "DROPSELECT_CB");
  if(cb)
  {
    int ret = cb(ih_matrix, ih_matrix->data->lines.focus_cell, ih_matrix->data->columns.focus_cell, ih, t, i, v);

    /* If the user returns IUP_CONTINUE in a dropselect_cb 
    the value is accepted and the matrix leaves edition mode. */
    if (ret == IUP_CONTINUE)
    {
      iupMatrixEditHide(ih_matrix);
      iupMatrixDrawUpdate(ih);
    }
  }

  return IUP_DEFAULT;
}

static void iMatrixEditChooseElement(Ihandle* ih, int lin, int col)
{
  int drop = iMatrixEditCallDropdownCb(ih, lin, col);
  if(drop)
    ih->data->datah = ih->data->droph;
  else
  {
    char* value;

    ih->data->datah = ih->data->texth;

    /* dropdown values are set by the user in DROP_CB.
    text value is set here from cell contents. */
    value = iupMatrixCellGetValue(ih, ih->data->lines.focus_cell, ih->data->columns.focus_cell);
    if (!value) value = "";
    IupSetAttribute(ih->data->texth, "VALUE", value);
  }
}

static int iMatrixEditCallEditionCb(Ihandle* ih, int mode, int update)
{
  int ret = iupMatrixAuxCallEditionCbLinCol(ih, ih->data->lines.focus_cell, ih->data->columns.focus_cell, mode, update);

  if (update && ret == IUP_DEFAULT && mode == 0)  /* leaving edition mode */
    iupMatrixCellUpdateValue(ih);

  return ret;
}

static int iMatrixEditCancel(Ihandle* ih, int focus, int update, int ignore)
{
  if (IupGetInt(ih->data->datah, "VISIBLE"))
  {
    int ret;

    /* Avoid calling EDITION_CB twice. Usually because a killfocus. */
    if (iupAttribGet(ih, "_IUPMAT_CALL_EDITION"))
      return IUP_DEFAULT;

    iupAttribSetStr(ih, "_IUPMAT_CALL_EDITION", "1");
    ret = iMatrixEditCallEditionCb(ih, 0, update);
    iupAttribSetStr(ih, "_IUPMAT_CALL_EDITION", NULL);

    if (ret == IUP_IGNORE && ignore)
      return IUP_IGNORE;

    IupSetAttribute(ih->data->datah, "VISIBLE", "NO");
    IupSetAttribute(ih->data->datah, "ACTIVE",  "NO");

    if (focus)
    {
      IupSetFocus(ih);
      ih->data->has_focus = 1; /* set this so even if getfocus_cb is not called the focus is drawn */
    }
#ifdef SunOS
    /* Usually when the edit control is hidden the matrix is automatically repainted by the system, except in SunOS. */
    iupMatrixDrawUpdate(ih);
#endif
  }

  return IUP_DEFAULT;
}

static int iMatrixEditKillFocus_CB(Ihandle* ih)
{
  Ihandle* ih_matrix = ih->parent;
  if (iupStrEqualNoCase(IupGetGlobal("DRIVER"), "Motif"))
  {
    if (iupAttribGet(ih_matrix, "_IUPMAT_DOUBLE_CLICK"))
      return IUP_DEFAULT;
  }

  iupMatrixEditForceHidden(ih_matrix);
  return IUP_DEFAULT;
}

int iupMatrixEditHide(Ihandle* ih)
{
  return iMatrixEditCancel(ih, 1, 1, 1); /* set focus + update + use ignore */
}

void iupMatrixEditForceHidden(Ihandle* ih)
{
  iMatrixEditCancel(ih, 0, 1, 0); /* no focus + update + no ignore */
}

int iupMatrixEditIsVisible(Ihandle* ih)
{
  if (!IupGetInt(ih, "ACTIVE"))
    return 0;

  if (!IupGetInt(ih->data->datah, "VISIBLE"))
    return 0;

  return 1;
}

int iupMatrixEditShow(Ihandle* ih)
{
  char* mask;
  int w, h, x, y;

  /* work around for Windows when using Multiline */
  if (iupAttribGet(ih, "_IUPMAT_IGNORE_SHOW"))
  {
    iupAttribSetStr(ih, "_IUPMAT_IGNORE_SHOW", NULL);
    return 0;
  }

  /* not active */
  if(!IupGetInt(ih, "ACTIVE"))
    return 0;

  /* already visible */
  if(IupGetInt(ih->data->datah, "VISIBLE"))
    return 0;

  /* notify application */
  if (iMatrixEditCallEditionCb(ih, 1, 0) == IUP_IGNORE)
    return 0;

  /* select edit control */
  iMatrixEditChooseElement(ih, ih->data->lines.focus_cell, ih->data->columns.focus_cell);

  /* position the cell to make it visible */
  /* If the focus is not visible, a scroll is done for that the focus to be visible */
  if (!iupMatrixAuxIsCellFullVisible(ih, ih->data->lines.focus_cell, ih->data->columns.focus_cell))
    iupMatrixScrollToVisible(ih, ih->data->lines.focus_cell, ih->data->columns.focus_cell);

  /* set attributes */
  iupMatrixPrepareDrawData(ih);
  IupStoreAttribute(ih->data->datah, "BGCOLOR", iupMatrixGetBgColor(ih, ih->data->lines.focus_cell, ih->data->columns.focus_cell));
  IupStoreAttribute(ih->data->datah, "FGCOLOR", iupMatrixGetFgColor(ih, ih->data->lines.focus_cell, ih->data->columns.focus_cell));
  IupSetAttribute(ih->data->datah, "FONT", iupMatrixGetFont(ih, ih->data->lines.focus_cell, ih->data->columns.focus_cell));

  mask = IupMatGetAttribute(ih,"MASK", ih->data->lines.focus_cell, ih->data->columns.focus_cell);
  if (mask)
  {
    IupSetAttribute(ih->data->datah, "MASKCASEI", IupMatGetAttribute(ih,"MASKCASEI", ih->data->lines.focus_cell, ih->data->columns.focus_cell));
    IupSetAttribute(ih->data->datah, "MASK", mask);
  }
  else
  {
    mask = IupMatGetAttribute(ih,"MASKINT", ih->data->lines.focus_cell, ih->data->columns.focus_cell);
    if (mask)
      IupSetAttribute(ih->data->datah, "MASKINT", mask);
    else
    {
      mask = IupMatGetAttribute(ih,"MASKFLOAT", ih->data->lines.focus_cell, ih->data->columns.focus_cell);
      if (mask)
        IupSetAttribute(ih->data->datah, "MASKFLOAT", mask);
    }
  }

  /* calc size */
  iupMatrixAuxGetVisibleCellDim(ih, ih->data->lines.focus_cell, ih->data->columns.focus_cell, &x, &y, &w, &h);

  ih->data->datah->x = x;
  ih->data->datah->y = y;
  if (IupGetGlobal("GTKVERSION"))
  {
    /* In GTK, IupCanvas is not the actual container of the IupText/IupList */
    ih->data->datah->x += ih->x;
    ih->data->datah->y += ih->y;
  }

  ih->data->datah->currentwidth  = w;
  ih->data->datah->currentheight = h;
  iupClassObjectLayoutUpdate(ih->data->datah);

  /* activate and show */
  IupSetAttribute(ih->data->datah, "ACTIVE",  "YES");
  IupSetAttribute(ih->data->datah, "VISIBLE", "YES");
  IupSetFocus(ih->data->datah);

  return 1;
}

static int iMatrixEditTextAction_CB(Ihandle* ih, int c, char* after)
{
  Ihandle* ih_matrix = ih->parent;
  IFniiiis cb = (IFniiiis) IupGetCallback(ih_matrix, "ACTION_CB");
  if (cb && c!=0) /* only for valid characters */
  {
    int oldc = c;
    c = cb(ih_matrix, c, ih_matrix->data->lines.focus_cell, ih_matrix->data->columns.focus_cell, 1, after);
    if (c == IUP_IGNORE || c == IUP_CLOSE || c == IUP_CONTINUE)
      return c;
    else if(c == IUP_DEFAULT)
      c = oldc;
    return c;
  }

  return IUP_DEFAULT;
}

static int iMatrixEditTextKeyAny_CB(Ihandle* ih, int c)
{
  Ihandle* ih_matrix = ih->parent;
  IFniiiis cb = (IFniiiis) IupGetCallback(ih_matrix, "ACTION_CB");
  if (cb && c==0) /* only for other characters */
  {
    int oldc = c;
    c = cb(ih_matrix, c, ih_matrix->data->lines.focus_cell, ih_matrix->data->columns.focus_cell, 1, IupGetAttribute(ih, "VALUE"));
    if(c == IUP_IGNORE || c == IUP_CLOSE || c == IUP_CONTINUE)
      return c;
    else if(c == IUP_DEFAULT)
      c = oldc;
  }

  switch (c)
  {
    case K_cUP:
    case K_cDOWN:
    case K_cLEFT:
    case K_cRIGHT:     
      if (iupMatrixEditHide(ih_matrix) == IUP_DEFAULT)
      {
        iupMatrixProcessKeyPress(ih_matrix, c);  
        return IUP_IGNORE;
      }
      break;
    case K_UP:
      if (IupGetInt(ih, "CARET") == 1)
      {
        /* if at the first line of the text */
        if (iupMatrixEditHide(ih_matrix) == IUP_DEFAULT)
        {
          iupMatrixProcessKeyPress(ih_matrix, c);  
          return IUP_IGNORE;
        }
      }
      break;
    case K_DOWN:
      { 
        char* value = IupGetAttribute(ih, "VALUE");
        if (value)
        {
          /* if at the last line of the text */
          if (iupStrLineCount(value) == IupGetInt(ih, "CARET"))
          {
            if (iupMatrixEditHide(ih_matrix) == IUP_DEFAULT)
            {
              iupMatrixProcessKeyPress(ih_matrix, c);  
              return IUP_IGNORE;
            }
          }
        }
      }
      break;
    case K_LEFT:
      if (IupGetInt(ih, "CARETPOS") == 0)
      {
        /* if at the first character */
        if (iupMatrixEditHide(ih_matrix) == IUP_DEFAULT)
        {
          iupMatrixProcessKeyPress(ih_matrix, c);  
          return IUP_IGNORE;
        }
      }
      break;
    case K_RIGHT:
      { 
        char* value = IupGetAttribute(ih, "VALUE");
        if (value)
        {
          /* if at the last character */
          if ((int)strlen(value) == IupGetInt(ih, "CARETPOS"))
          {
            if (iupMatrixEditHide(ih_matrix) == IUP_DEFAULT)
            {
              iupMatrixProcessKeyPress(ih_matrix, c);  
              return IUP_IGNORE;
            }
          }
        }
      }
      break;
    case K_ESC:
      iMatrixEditCancel(ih_matrix, 1, 0, 0); /* set focus + NO update + NO ignore */
      return IUP_IGNORE;
    case K_CR:
      if (iupMatrixEditHide(ih_matrix) == IUP_DEFAULT)
      {
        if (iupStrEqualNoCase(IupGetGlobal("DRIVER"), "Win32") && IupGetInt(ih, "MULTILINE"))
        {
          /* work around for Windows when using Multiline */
          iupAttribSetStr(ih_matrix, "_IUPMAT_IGNORE_SHOW", "1");
        }

        if (iupMatrixAuxCallLeaveCellCb(ih_matrix) != IUP_IGNORE)
        {
          iupMatrixScrollKeyCr(ih_matrix);
          iupMatrixAuxCallEnterCellCb(ih_matrix);
        }
        iupMatrixDrawUpdate(ih_matrix);
        return IUP_IGNORE;
      }
      break;
  }

  return IUP_CONTINUE;
}

static int iMatrixEditDropDownKeyAny_CB(Ihandle* ih, int c)
{
  Ihandle* ih_matrix = ih->parent;
  IFniiiis cb = (IFniiiis)IupGetCallback(ih_matrix, "ACTION_CB");
  if (cb)
  {
    int oldc = c;
    c = cb(ih_matrix, c, ih_matrix->data->lines.focus_cell, ih_matrix->data->columns.focus_cell, 1, "");
    if (c == IUP_IGNORE || c == IUP_CLOSE  || c == IUP_CONTINUE)
      return c;
    else if(c == IUP_DEFAULT)
      c = oldc;
  }

  switch (c)
  {
    case K_CR:
      if (iupMatrixEditHide(ih_matrix) == IUP_DEFAULT)
      {
        if (iupMatrixAuxCallLeaveCellCb(ih_matrix) != IUP_IGNORE)
        {
          iupMatrixScrollKeyCr(ih_matrix);
          iupMatrixAuxCallEnterCellCb(ih_matrix);
        }
        iupMatrixDrawUpdate(ih_matrix);
        return IUP_IGNORE;
      }
      break;
    case K_ESC:
      iMatrixEditCancel(ih_matrix, 1, 0, 0); /* set focus + NO update + NO ignore */
      return IUP_IGNORE;
  }

  return IUP_CONTINUE;
}

char* iupMatrixEditGetValue(Ihandle* ih)
{
  if (ih->data->datah == ih->data->droph)
    return IupGetAttribute(ih->data->droph, IupGetAttribute(ih->data->droph, "VALUE"));
  else
    return IupGetAttribute(ih->data->texth, "VALUE");
}

void iupMatrixEditCreate(Ihandle* ih)
{
  /******** EDIT *************/
  ih->data->texth = IupText(NULL);
  iupChildTreeAppend(ih, ih->data->texth);

  IupSetCallback(ih->data->texth, "ACTION",       (Icallback)iMatrixEditTextAction_CB);
  IupSetCallback(ih->data->texth, "K_ANY",        (Icallback)iMatrixEditTextKeyAny_CB);
  IupSetCallback(ih->data->texth, "KILLFOCUS_CB", (Icallback)iMatrixEditKillFocus_CB);
  IupSetAttribute(ih->data->texth, "VALUE",  "");
  IupSetAttribute(ih->data->texth, "VISIBLE", "NO");
  IupSetAttribute(ih->data->texth, "ACTIVE",  "NO");


  /******** DROPDOWN *************/
  ih->data->droph = IupList(NULL);
  iupChildTreeAppend(ih, ih->data->droph);

  IupSetCallback(ih->data->droph, "ACTION",       (Icallback)iMatrixEditDropDownAction_CB);
  IupSetCallback(ih->data->droph, "KILLFOCUS_CB", (Icallback)iMatrixEditKillFocus_CB);
  IupSetCallback(ih->data->droph, "K_ANY",        (Icallback)iMatrixEditDropDownKeyAny_CB);
  IupSetAttribute(ih->data->droph, "DROPDOWN", "YES");
  IupSetAttribute(ih->data->droph, "MULTIPLE", "NO");
  IupSetAttribute(ih->data->droph, "VISIBLE", "NO");
  IupSetAttribute(ih->data->droph, "ACTIVE",  "NO");
}
