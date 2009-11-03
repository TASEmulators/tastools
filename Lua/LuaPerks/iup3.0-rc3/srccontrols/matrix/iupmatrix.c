/** \file
 * \brief IupMatrix control core
 *
 * See Copyright Notice in "iup.h"
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdarg.h>
#include <string.h>

#include "iup.h"
#include "iupcbs.h"
#include "iupcontrols.h"

#include <cd.h>
#include <cdiup.h>
#include <cddbuf.h>

#include "iup_object.h"
#include "iup_attrib.h"
#include "iup_str.h"
#include "iup_drv.h"
#include "iup_drvfont.h"
#include "iup_stdcontrols.h"
#include "iup_controls.h"
#include "iup_register.h"

#include "iupmat_def.h"
#include "iupmat_getset.h"
#include "iupmat_scroll.h"
#include "iupmat_aux.h"
#include "iupmat_mem.h"
#include "iupmat_mouse.h"
#include "iupmat_focus.h"
#include "iupmat_key.h"
#include "iupmat_numlc.h"
#include "iupmat_colres.h"
#include "iupmat_mark.h"
#include "iupmat_edit.h"
#include "iupmat_draw.h"


int iupMatrixIsValid(Ihandle* ih, int check_cells)
{
  if (!ih->data->cddbuffer)
    return 0;
  if (check_cells && ((ih->data->columns.num == 0) || (ih->data->lines.num == 0)))
    return 0;
  return 1;
}

static int iMatrixSetOriginAttrib(Ihandle* ih, const char* value)
{
  int lin = -1, col = -1;

 /* Get the parameters. The '*' indicates that want to keep the table in
    the same line or column */
  if (iupStrToIntInt(value, &lin, &col, ':') != 2)
  {
    if (lin != -1)
      col = ih->data->columns.first;
    else if (col != -1)
      lin = ih->data->lines.first;
    else
      return 0;
  }

  /* Check if the cell exists */
  if (!iupMatrixCheckCellPos(ih, lin, col))
    return 0;

  /* Can not be a title */
  if((lin < 1) || (col < 1))
    return 0;

  ih->data->columns.first = col;
  ih->data->lines.first = lin;

  iupMatrixAuxUpdateVisiblePos(ih, IMAT_PROCESS_COL);
  iupMatrixAuxUpdateVisiblePos(ih, IMAT_PROCESS_LIN);

  iupMatrixDraw(ih, 1);
  return 0;
}

static int iMatrixSetShowAttrib(Ihandle* ih, const char* value)
{
  int lin = -1, col = -1;

 /* Get the parameters. The '*' indicates that want to keep the table in
    the same line or column */
  if (iupStrToIntInt(value, &lin, &col, ':') != 2)
  {
    if (lin != -1)
      col = ih->data->columns.first;
    else if (col != -1)
      lin = ih->data->lines.first;
    else
      return 0;
  }

  /* Check if the cell exists */
  if (!iupMatrixCheckCellPos(ih, lin, col))
    return 0;

  /* Can not be a title */
  if((lin < 1) || (col < 1))
    return 0;

  if (!iupMatrixAuxIsCellFullVisible(ih, lin, col))
    iupMatrixScrollToVisible(ih, lin, col);

  return 0;
}

static char* iMatrixGetOriginAttrib(Ihandle* ih)
{
  char* val = iupStrGetMemory(100);
  sprintf(val, "%d:%d", ih->data->lines.first, ih->data->columns.first);
  return val;
}

static int iMatrixSetFocusCellAttrib(Ihandle* ih, const char* value)
{
  int lin = 0, col = 0;
  if (iupStrToIntInt(value, &lin, &col, ':') == 2)
  {
    if (!iupMatrixCheckCellPos(ih, lin, col))
      return 0;

    if (lin <= 0 || col <= 0) /* title can NOT have the focus */
      return 0;
    if (lin >= ih->data->lines.num || col >= ih->data->columns.num)
      return 0;

    iupMatrixFocusSet(ih, lin, col);

    if (ih->data->cddbuffer)
      iupMatrixDrawUpdate(ih);
  }

  return 0;
}

static char* iMatrixGetFocusCellAttrib(Ihandle* ih)
{
  char* str = iupStrGetMemory(100);
  sprintf(str, "%d:%d", ih->data->lines.focus_cell, ih->data->columns.focus_cell);
  return str;
}

static int iMatrixSetUseTitleSizeAttrib(Ihandle* ih, const char* value)
{
  /* can be set only before map */
  if (ih->handle)
    return 0;

  if (iupStrBoolean(value))
    ih->data->use_title_size = 1;
  else 
    ih->data->use_title_size = 0;

  return 0;
}

static char* iMatrixGetUseTitleSizeAttrib(Ihandle* ih)
{
  if (ih->data->use_title_size)
    return "YES";
  else
    return "NO";
}

static int iMatrixSetValueAttrib(Ihandle* ih, const char* value)
{
  if (IupGetInt(ih->data->datah, "VISIBLE"))
    IupStoreAttribute(ih->data->datah, "VALUE", value);
  else 
    iupMatrixCellSetValue(ih, ih->data->lines.focus_cell, ih->data->columns.focus_cell, value);
  return 0;
}

static char* iMatrixGetValueAttrib(Ihandle* ih)
{
  if (IupGetInt(ih->data->datah, "VISIBLE"))
    return iupMatrixEditGetValue(ih);
  else 
    return iupMatrixCellGetValue(ih, ih->data->lines.focus_cell, ih->data->columns.focus_cell);
}

static int iMatrixSetCaretAttrib(Ihandle* ih, const char* value)
{
  IupStoreAttribute(ih->data->texth, "CARET", value);
  return 1;
}

static char* iMatrixGetCaretAttrib(Ihandle* ih)
{
  return IupGetAttribute(ih->data->texth, "CARET");
}

static int iMatrixSetSelectionAttrib(Ihandle* ih, const char* value)
{
  IupStoreAttribute(ih->data->texth, "SELECTION", value);
  return 1;
}

static char* iMatrixGetSelectionAttrib(Ihandle* ih)
{
  return IupGetAttribute(ih->data->texth, "SELECTION");
}

static int iMatrixSetMultilineAttrib(Ihandle* ih, const char* value)
{
  IupStoreAttribute(ih->data->texth, "MULTILINE", value);
  if (iupStrBoolean(value))
    IupSetAttribute(ih->data->texth, "SCROLLBAR", "NO");
  return 1;
}

static char* iMatrixGetMultilineAttrib(Ihandle* ih)
{
  return IupGetAttribute(ih->data->texth, "MULTILINE");
}

static char* iMatrixGetNumLinAttrib(Ihandle* ih)
{
  char* num = iupStrGetMemory(100);
  sprintf(num, "%d", ih->data->lines.num-1);
  return num;
}

static char* iMatrixGetNumColAttrib(Ihandle* ih)
{
  char* num = iupStrGetMemory(100);
  sprintf(num, "%d", ih->data->columns.num-1);
  return num;
}

static int iMatrixSetMarkModeAttrib(Ihandle* ih, const char* value)
{
  if (iupStrEqualNoCase(value, "CELL"))
    ih->data->mark_mode = IMAT_MARK_CELL;
  else if (iupStrEqualNoCase(value, "LIN"))
    ih->data->mark_mode = IMAT_MARK_LIN;
  else if (iupStrEqualNoCase(value, "COL"))
    ih->data->mark_mode = IMAT_MARK_COL;
  else if(iupStrEqualNoCase(value, "LINCOL"))
    ih->data->mark_mode = IMAT_MARK_LINCOL;
  else 
    ih->data->mark_mode = IMAT_MARK_NO;

  if (ih->handle)
  {
    iupMatrixMarkClearAll(ih, 0);
    iupMatrixDraw(ih, 1);
  }
  return 0;
}

static char* iMatrixGetMarkModeAttrib(Ihandle* ih)
{
  char* mark2str[] = {"NO", "LIN", "COL", "LINCOL", "CELL"};
  return mark2str[ih->data->mark_mode];
}

static int iMatrixSetMarkAreaAttrib(Ihandle* ih, const char* value)
{
  if (iupStrEqualNoCase(value, "NOT_CONTINUOUS"))
    ih->data->mark_continuous = 0;
  else 
    ih->data->mark_continuous = 1;

  if (ih->handle)
  {
    iupMatrixMarkClearAll(ih, 0);
    iupMatrixDraw(ih, 1);
  }
  return 0;
}

static char* iMatrixGetMarkAreaAttrib(Ihandle* ih)
{
  if (ih->data->mark_continuous)
    return "CONTINUOUS";
  else
    return "NOT_CONTINUOUS";
}

static int iMatrixSetMarkMultipleAttrib(Ihandle* ih, const char* value)
{
  if (iupStrBoolean(value))
    ih->data->mark_multiple = 1;
  else 
    ih->data->mark_multiple = 0;

  if (ih->handle)
  {
    iupMatrixMarkClearAll(ih, 0);
    iupMatrixDraw(ih, 1);
  }
  return 0;
}

static char* iMatrixGetMarkMultipleAttrib(Ihandle* ih)
{
  if (ih->data->mark_multiple)
    return "YES";
  else
    return "NO";
}

static int iMatrixSetEditModeAttrib(Ihandle* ih, const char* value)
{
  if (iupStrBoolean(value))
    iupMatrixEditShow(ih);
  else
  {
    iupMatrixEditHide(ih);
    iupMatrixDrawUpdate(ih);
  }
  return 1;
}

static char* iMatrixGetEditModeAttrib(Ihandle* ih)
{
  if (iupMatrixEditIsVisible(ih))
    return "YES";
  else
    return "NO";
}

static int iMatrixSetActiveAttrib(Ihandle* ih, const char* value)
{
  iupBaseSetActiveAttrib(ih, value);
  iupMatrixDraw(ih, 1);
  return 0;
}

static int iMatrixSetWidthAttrib(Ihandle* ih, const char* name_id, const char* value)
{
  int col = 0;
  if (iupStrToInt(name_id, &col))
  {
    (void)value;
    ih->data->need_calcsize = 1;
    IupUpdate(ih);
  }
  return 1;
}

static char* iMatrixGetWidthAttrib(Ihandle* ih, const char* name_id)
{
  int col;
  if (iupStrToInt(name_id, &col))
    return iupMatrixGetSize(ih, col, IMAT_PROCESS_COL, 0);
  return NULL;
}

static int iMatrixSetHeightAttrib(Ihandle* ih, const char* name_id, const char* value)
{
  int lin = 0;
  if (iupStrToInt(name_id, &lin))
  {
    (void)value;
    ih->data->need_calcsize = 1;
    IupUpdate(ih);
  }
  return 1;
}

static char* iMatrixGetHeightAttrib(Ihandle* ih, const char* name_id)
{
  int lin;
  if (iupStrToInt(name_id, &lin))
    return iupMatrixGetSize(ih, lin, IMAT_PROCESS_LIN, 0);
  return NULL;
}

static int iMatrixSetRasterWidthAttrib(Ihandle* ih, const char* name_id, const char* value)
{
  int col = 0;
  if (iupStrToInt(name_id, &col))
  {
    (void)value;
    ih->data->need_calcsize = 1;
    IupUpdate(ih);
  }
  return 1;
}

static char* iMatrixGetRasterWidthAttrib(Ihandle* ih, const char* name_id)
{
  int col;
  if (iupStrToInt(name_id, &col))
    return iupMatrixGetSize(ih, col, IMAT_PROCESS_COL, 1);
  return NULL;
}

static int iMatrixSetRasterHeightAttrib(Ihandle* ih, const char* name_id, const char* value)
{
  int lin = 0;
  if (iupStrToInt(name_id, &lin))
  {
    (void)value;
    ih->data->need_calcsize = 1;
    IupUpdate(ih);
  }
  return 1;
}

static char* iMatrixGetRasterHeightAttrib(Ihandle* ih, const char* name_id)
{
  int lin;
  if (iupStrToInt(name_id, &lin))
    return iupMatrixGetSize(ih, lin, IMAT_PROCESS_LIN, 1);
  return NULL;
}

static char* iMatrixGetAlignmentAttrib(Ihandle* ih, const char* name_id)
{
  char* align;
  char str[50];
  sprintf(str, "ALIGNMENT%s", name_id);
  align = iupAttribGet(ih, str);
  if (!align)
  {
    int col;
    if (iupStrToInt(name_id, &col))
    {
      if (col == 0)
        return "ALEFT";
      else
        return "ACENTER";
    }
  }
    
  return NULL;
}

static int iMatrixSetIdValueAttrib(Ihandle* ih, const char* name_id, const char* value)
{
  int lin = 0, col = 0;
  if (iupStrToIntInt(name_id, &lin, &col, ':') == 2)
  {
    if (iupMatrixCheckCellPos(ih, lin, col))
      iupMatrixCellSetValue(ih, lin, col, value);
  }
  return 0;
}

static char* iMatrixGetIdValueAttrib(Ihandle* ih, const char* name_id)
{
  int lin, col;
  if (iupStrToIntInt(name_id, &lin, &col, ':') == 2)
  {
    if (iupMatrixCheckCellPos(ih, lin, col))
      return iupMatrixCellGetValue(ih, lin, col);
  }
  return NULL;
}

static int iMatrixSetFlagsAttrib(Ihandle* ih, const char* name_id, const char* value, unsigned char attr)
{
  if (name_id[0]==0)
    return 1;
  if (name_id[0]=='*' && name_id[1]==':')
  {
    int col;
    name_id += 2; /* skip '*' and ':' */
    if (iupStrToInt(name_id, &col))
      iupMatrixCellSetFlag(ih, -1, col, attr, value!=NULL);
  }
  else if (name_id[strlen(name_id)-1]=='*')
  {
    int lin;
    if (iupStrToInt(name_id, &lin))
      iupMatrixCellSetFlag(ih, lin, -1, attr, value!=NULL);
  }
  else 
  {
    int lin, col;
    if (iupStrToIntInt(name_id, &lin, &col, ':') == 2)
      iupMatrixCellSetFlag(ih, lin, col, attr, value!=NULL);
  }
  return 1;
}

static int iMatrixSetBgColorAttrib(Ihandle* ih, const char* name_id, const char* value)
{
  return iMatrixSetFlagsAttrib(ih, name_id, value, IUPMAT_BGCOLOR);
}

static int iMatrixSetFgColorAttrib(Ihandle* ih, const char* name_id, const char* value)
{
  return iMatrixSetFlagsAttrib(ih, name_id, value, IUPMAT_FGCOLOR);
}

static int iMatrixSetFontAttrib(Ihandle* ih, const char* name_id, const char* value)
{
  return iMatrixSetFlagsAttrib(ih, name_id, value, IUPMAT_FONT);
}

static int iMatrixSetFrameHorizColorAttrib(Ihandle* ih, const char* name_id, const char* value)
{
  ih->data->checkframecolor = value!=NULL;
  return iMatrixSetFlagsAttrib(ih, name_id, value, IUPMAT_FRAMEHCOLOR);
}

static int iMatrixSetFrameVertColorAttrib(Ihandle* ih, const char* name_id, const char* value)
{
  ih->data->checkframecolor = value!=NULL;
  return iMatrixSetFlagsAttrib(ih, name_id, value, IUPMAT_FRAMEVCOLOR);
}

static char* iMatrixGetFontAttrib(Ihandle* ih, const char* name_id)
{
  if (name_id[0]==0)
    return iupGetFontAttrib(ih);
  return NULL;
}

static char* iMatrixGetBgColorAttrib(Ihandle* ih, const char* name_id)
{
  if (name_id[0]==0)
  {
    /* check the hash table */
    char *color = iupAttribGet(ih, "BGCOLOR");

    /* If not defined return the default for normal cells */
    if (!color)
      color = IupGetGlobal("TXTBGCOLOR");

    return color;
  }
  return NULL;
}

static char* iMatrixGetNumColVisibleAttrib(Ihandle* ih)
{
  char* buffer = iupStrGetMemory(50);
  sprintf(buffer, "%d", ih->data->columns.last - ih->data->columns.first);
  return buffer;
}

static char* iMatrixGetNumLinVisibleAttrib(Ihandle* ih)
{
  char* buffer = iupStrGetMemory(50);
  sprintf(buffer, "%d", ih->data->lines.last - ih->data->lines.first);
  return buffer;
}

static char* iMatrixGetMaskDataAttrib(Ihandle* ih)
{
  /* Used only by the OLD iupmask API */
  if (IupGetInt(ih->data->datah, "VISIBLE"))
    return IupGetAttribute(ih->data->datah,"OLD_MASK_DATA");
  else
    return NULL;
}


/*****************************************************************************/
/*   Callbacks registered to the Canvas                                      */
/*****************************************************************************/


static int iMatrixResize_CB(Ihandle* ih)
{
  if (!ih->data->cddbuffer)
  {
    /* update canvas size */
    cdCanvasActivate(ih->data->cdcanvas);

    /* this can fail if canvas size is zero */
    ih->data->cddbuffer = cdCreateCanvas(CD_DBUFFER, ih->data->cdcanvas);
  }

  if (!ih->data->cddbuffer)
    return IUP_DEFAULT;

  /* update size */
  cdCanvasActivate(ih->data->cddbuffer);
  cdCanvasGetSize(ih->data->cddbuffer, &ih->data->w, &ih->data->h, NULL, NULL);

  iupMatrixEditForceHidden(ih);

  ih->data->need_calcsize = 1;
  iupMatrixDraw(ih, 0);

  return IUP_DEFAULT;
}

static int iMatrixRedraw_CB(Ihandle* ih)
{
  if (!ih->data->cddbuffer)
    return IUP_DEFAULT;

  if (!ih->data->first_redraw)
  {
    ih->data->first_redraw = 1;
    iupMatrixDraw(ih, 0);
  }

  iupMatrixDrawUpdate(ih);

  return IUP_DEFAULT;
}


/***************************************************************************/


static int iMatrixCreateMethod(Ihandle* ih, void **params)
{
  if (params && params[0])
  {
    char* action_cb = (char*)params[0];
    iupAttribStoreStr(ih, "ACTION_CB", action_cb);
  }

  /* free the data allocated by IupCanvas */
  if (ih->data) free(ih->data);
  ih->data = iupALLOCCTRLDATA();

  /* change the IupCanvas default values */
  iupAttribSetStr(ih, "SCROLLBAR", "YES");
  iupAttribSetStr(ih, "BORDER", "NO");
  iupAttribSetStr(ih, "CURSOR", "IupMatrixCrossCursor");

  /* IupCanvas callbacks */
  IupSetCallback(ih, "ACTION",      (Icallback)iMatrixRedraw_CB);
  IupSetCallback(ih, "RESIZE_CB",   (Icallback)iMatrixResize_CB);
  IupSetCallback(ih, "BUTTON_CB",   (Icallback)iupMatrixMouseButton_CB);
  IupSetCallback(ih, "MOTION_CB",   (Icallback)iupMatrixMouseMove_CB);
  IupSetCallback(ih, "KEYPRESS_CB", (Icallback)iupMatrixKeyPress_CB);
  IupSetCallback(ih, "FOCUS_CB",    (Icallback)iupMatrixFocus_CB);
  IupSetCallback(ih, "SCROLL_CB",   (Icallback)iupMatrixScroll_CB);

  /* Create the edit fields */
  iupMatrixEditCreate(ih);

  /* defaults */
  ih->data->datah = ih->data->texth;
  ih->data->mark_continuous = 1;
  ih->data->columns.num = 1;
  ih->data->lines.num = 1;
  ih->data->need_calcsize = 1;
  ih->data->lines.first = 1;
  ih->data->columns.first = 1;
  ih->data->lines.focus_cell = 1;
  ih->data->columns.focus_cell = 1;
  ih->data->mark_lin1 = -1;
  ih->data->mark_col1 = -1;
  ih->data->mark_lin2 = -1;
  ih->data->mark_col2 = -1;
  ih->data->use_title_size = 0;

  return IUP_NOERROR;
}

static int iMatrixMapMethod(Ihandle* ih)
{
  ih->data->cdcanvas = cdCreateCanvas(CD_IUP, ih);
  if (!ih->data->cdcanvas)
    return IUP_ERROR;

  /* this can fail if canvas size is zero */
  ih->data->cddbuffer = cdCreateCanvas(CD_DBUFFER, ih->data->cdcanvas);

  if (IupGetCallback(ih, "VALUE_CB"))
  {
    ih->data->callback_mode = 1;

    if (!IupGetCallback(ih, "VALUE_EDIT_CB"))
      iupAttribSetStr(ih, "READONLY", "YES");
  }

  iupMatrixMemAlloc(ih);

  return IUP_NOERROR;
}

static void iMatrixUnMapMethod(Ihandle* ih)
{
  if(ih->data->cddbuffer)
  {
    cdKillCanvas(ih->data->cddbuffer);
    ih->data->cddbuffer = NULL;
  }

  if(ih->data->cdcanvas)
  {
    cdKillCanvas(ih->data->cdcanvas);
    ih->data->cdcanvas = NULL;
  }

  iupMatrixMemRelease(ih);
}

static int iMatrixGetNaturalWidth(Ihandle* ih)
{
  int width = 0, num, col;

  num = iupAttribGetInt(ih, "NUMCOL_VISIBLE")+1;  /* include the title column */

  if (iupAttribGetInt(ih, "NUMCOL_VISIBLE_LAST"))
  {
    int start = ih->data->columns.num - (num-1); /* title is computed apart */
    if (start<1) start=1;
    width += iupMatrixAuxGetColumnWidth(ih, 0); /* compute title */
    for(col = start; col < ih->data->columns.num; col++)
      width += iupMatrixAuxGetColumnWidth(ih, col);
  }
  else
  {
    if (num > ih->data->columns.num)
      num = ih->data->columns.num;
    for(col = 0; col < num; col++)
      width += iupMatrixAuxGetColumnWidth(ih, col);
  }

  return width;
}

static int iMatrixGetNaturalHeight(Ihandle* ih)
{
  int height = 0, num, lin;

  num = iupAttribGetInt(ih, "NUMLIN_VISIBLE")+1;  /* include the title line */

  if (iupAttribGetInt(ih, "NUMLIN_VISIBLE_LAST"))
  {
    int start = ih->data->lines.num - (num-1);   /* title is computed apart */
    if (start<1) start=1;
    height += iupMatrixAuxGetLineHeight(ih, 0);  /* compute title */
    for(lin = start; lin < ih->data->lines.num; lin++)
      height += iupMatrixAuxGetLineHeight(ih, lin);
  }
  else
  {
    if (num > ih->data->lines.num)
      num = ih->data->lines.num;
    for(lin = 0; lin < num; lin++)
      height += iupMatrixAuxGetLineHeight(ih, lin);
  }

  return height;
}

static void iMatrixComputeNaturalSizeMethod(Ihandle* ih, int *w, int *h, int *expand)
{
  int natural_w = 0, natural_h = 0;
  (void)expand; /* unset if not a container */

  if (!ih->handle)
    ih->data->canvas.sb = iupBaseGetScrollbar(ih);

  /* add scrollbar */
  if (ih->data->canvas.sb)
  {
    int sb_size = iupdrvGetScrollbarSize();
    if (ih->data->canvas.sb & IUP_SB_HORIZ)
      natural_w += sb_size;
    if (ih->data->canvas.sb & IUP_SB_VERT)
      natural_h += sb_size;
  }

  *w = natural_w + iMatrixGetNaturalWidth(ih);
  *h = natural_h + iMatrixGetNaturalHeight(ih);
}

static void iMatrixCreateCursor(void)
{
  Ihandle *imgcursor;
  unsigned char matrx_img_cur_excel[15*15] = 
  {
    0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,
    0,0,0,0,1,2,2,2,2,1,1,0,0,0,0,
    0,0,0,0,1,2,2,2,2,1,1,0,0,0,0,
    0,0,0,0,1,2,2,2,2,1,1,0,0,0,0,
    1,1,1,1,1,2,2,2,2,1,1,1,1,1,0,
    1,2,2,2,2,2,2,2,2,2,2,2,2,1,1,
    1,2,2,2,2,2,2,2,2,2,2,2,2,1,1,
    1,2,2,2,2,2,2,2,2,2,2,2,2,1,1,
    1,1,1,1,1,2,2,2,2,1,1,1,1,1,1,
    0,1,1,1,1,2,2,2,2,1,1,1,1,1,1,
    0,0,0,0,1,2,2,2,2,1,1,0,0,0,0,
    0,0,0,0,1,2,2,2,2,1,1,0,0,0,0,
    0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,
    0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
  };

  imgcursor = IupImage(15, 15, matrx_img_cur_excel);
  IupSetAttribute(imgcursor, "0", "BGCOLOR"); 
  IupSetAttribute(imgcursor, "1", "0 0 0"); 
  IupSetAttribute(imgcursor, "2", "255 255 255"); 
  IupSetAttribute(imgcursor, "HOTSPOT", "7:7");     /* Centered Hotspot           */
  IupSetHandle("IupMatrixCrossCursor", imgcursor); 
  IupSetHandle("matrx_img_cur_excel",  imgcursor);  /* for backward compatibility */
}

Iclass* iupMatrixGetClass(void)
{
  Iclass* ic = iupClassNew(iupCanvasGetClass());

  ic->name = "matrix";
  ic->format = "A"; /* one optional callback name */
  ic->nativetype = IUP_TYPECANVAS;
  ic->childtype = IUP_CHILDNONE;
  ic->is_interactive = 1;
  ic->has_attrib_id = 1;   /* has attributes with IDs that must be parsed */

  /* Class functions */
  ic->Create  = iMatrixCreateMethod;
  ic->Map     = iMatrixMapMethod;
  ic->UnMap   = iMatrixUnMapMethod;
  ic->ComputeNaturalSize = iMatrixComputeNaturalSizeMethod;

  /* Do not need to set base attributes because they are inherited from IupCanvas */

  /* IupMatrix Callbacks */
  /* --- Interaction --- */
  iupClassRegisterCallback(ic, "ACTION_CB",  "iiiis");
  iupClassRegisterCallback(ic, "CLICK_CB",   "iis");
  iupClassRegisterCallback(ic, "RELEASE_CB", "iis");
  iupClassRegisterCallback(ic, "MOUSEMOVE_CB", "ii");
  iupClassRegisterCallback(ic, "ENTERITEM_CB", "ii");
  iupClassRegisterCallback(ic, "LEAVEITEM_CB", "ii");
  iupClassRegisterCallback(ic, "SCROLLTOP_CB", "ii");
  /* --- Drawing --- */
  iupClassRegisterCallback(ic, "BGCOLOR_CB", "iiIII");
  iupClassRegisterCallback(ic, "FGCOLOR_CB", "iiIII");
  iupClassRegisterCallback(ic, "FONT_CB", "ii=s");
  iupClassRegisterCallback(ic, "DRAW_CB", "iiiiii");
  iupClassRegisterCallback(ic, "DROPCHECK_CB", "ii");
  /* --- Editing --- */
  iupClassRegisterCallback(ic, "DROP_CB", "nii");
  iupClassRegisterCallback(ic, "DROPSELECT_CB", "iinsii");
  iupClassRegisterCallback(ic, "EDITION_CB", "iii");
  /* --- Callback Mode --- */
  iupClassRegisterCallback(ic, "VALUE_CB", "ii=s");
  iupClassRegisterCallback(ic, "VALUE_EDIT_CB", "iis");
  iupClassRegisterCallback(ic, "MARK_CB", "ii");
  iupClassRegisterCallback(ic, "MARKEDIT_CB", "iii");

  /* IupMatrix Attributes - CELL */
  iupClassRegisterAttributeId(ic, "IDVALUE", iMatrixGetIdValueAttrib, iMatrixSetIdValueAttrib, IUPAF_NO_INHERIT);
  iupClassRegisterAttribute(ic, "FOCUS_CELL", iMatrixGetFocusCellAttrib, iMatrixSetFocusCellAttrib, IUPAF_SAMEASSYSTEM, "1:1", IUPAF_NOT_MAPPED|IUPAF_NO_INHERIT); /* can be NOT mapped */
  iupClassRegisterAttribute(ic, "VALUE", iMatrixGetValueAttrib, iMatrixSetValueAttrib, NULL, NULL, IUPAF_NO_DEFAULTVALUE|IUPAF_NO_INHERIT);
  iupClassRegisterAttributeId(ic, "FGCOLOR", NULL, iMatrixSetFgColorAttrib, IUPAF_NOT_MAPPED);
  iupClassRegisterAttributeId(ic, "FONT", iMatrixGetFontAttrib, iMatrixSetFontAttrib, IUPAF_NOT_MAPPED);
  iupClassRegisterAttributeId(ic, "FRAMEHORIZCOLOR", NULL, iMatrixSetFrameHorizColorAttrib, IUPAF_NOT_MAPPED);
  iupClassRegisterAttributeId(ic, "FRAMEVERTCOLOR", NULL, iMatrixSetFrameVertColorAttrib, IUPAF_NOT_MAPPED);

  /* IupMatrix Attributes - COLUMN */
  iupClassRegisterAttributeId(ic, "ALIGNMENT", iMatrixGetAlignmentAttrib, NULL, IUPAF_NOT_MAPPED|IUPAF_NO_INHERIT);
  iupClassRegisterAttributeId(ic, "SORTSIGN", NULL, NULL, IUPAF_NOT_MAPPED|IUPAF_NO_INHERIT);

  /* IupMatrix Attributes - SIZE */
  iupClassRegisterAttribute(ic, "NUMLIN", iMatrixGetNumLinAttrib, iupMatrixSetNumLinAttrib, NULL, NULL, IUPAF_NOT_MAPPED|IUPAF_NO_INHERIT);
  iupClassRegisterAttribute(ic, "NUMCOL", iMatrixGetNumColAttrib, iupMatrixSetNumColAttrib, NULL, NULL, IUPAF_NOT_MAPPED|IUPAF_NO_INHERIT);
  iupClassRegisterAttribute(ic, "NUMLIN_VISIBLE", iMatrixGetNumLinVisibleAttrib, NULL, IUPAF_SAMEASSYSTEM, "3", IUPAF_NOT_MAPPED|IUPAF_NO_INHERIT);
  iupClassRegisterAttribute(ic, "NUMCOL_VISIBLE", iMatrixGetNumColVisibleAttrib, NULL, IUPAF_SAMEASSYSTEM, "4", IUPAF_NOT_MAPPED|IUPAF_NO_INHERIT);
  iupClassRegisterAttribute(ic, "NUMLIN_VISIBLE_LAST", NULL, NULL, NULL, NULL, IUPAF_NOT_MAPPED|IUPAF_NO_INHERIT);
  iupClassRegisterAttribute(ic, "NUMCOL_VISIBLE_LAST", NULL, NULL, NULL, NULL, IUPAF_NOT_MAPPED|IUPAF_NO_INHERIT);
  iupClassRegisterAttribute(ic, "WIDTHDEF", NULL, NULL, IUPAF_SAMEASSYSTEM, "80", IUPAF_NOT_MAPPED|IUPAF_NO_INHERIT);
  iupClassRegisterAttribute(ic, "HEIGHTDEF", NULL, NULL, IUPAF_SAMEASSYSTEM, "8", IUPAF_NOT_MAPPED|IUPAF_NO_INHERIT);
  iupClassRegisterAttributeId(ic, "WIDTH", iMatrixGetWidthAttrib, iMatrixSetWidthAttrib, IUPAF_NOT_MAPPED|IUPAF_NO_INHERIT);
  iupClassRegisterAttributeId(ic, "HEIGHT", iMatrixGetHeightAttrib, iMatrixSetHeightAttrib, IUPAF_NOT_MAPPED|IUPAF_NO_INHERIT);
  iupClassRegisterAttributeId(ic, "RASTERWIDTH", iMatrixGetRasterWidthAttrib, iMatrixSetRasterWidthAttrib, IUPAF_NOT_MAPPED|IUPAF_NO_INHERIT);
  iupClassRegisterAttributeId(ic, "RASTERHEIGHT", iMatrixGetRasterHeightAttrib, iMatrixSetRasterHeightAttrib, IUPAF_NOT_MAPPED|IUPAF_NO_INHERIT);

  /* IupMatrix Attributes - MARK */
  iupClassRegisterAttribute(ic, "MARKED", iupMatrixGetMarkedAttrib, iupMatrixSetMarkedAttrib, NULL, NULL, IUPAF_NO_INHERIT);  /* noticed that MARKED must be mapped */
  iupClassRegisterAttributeId(ic, "MARK", iupMatrixGetMarkAttrib, iupMatrixSetMarkAttrib, IUPAF_NO_INHERIT);  /* noticed that for MARK the matrix must be mapped */
  iupClassRegisterAttribute(ic, "MARK_MODE", iMatrixGetMarkModeAttrib, iMatrixSetMarkModeAttrib, NULL, NULL, IUPAF_NOT_MAPPED|IUPAF_NO_INHERIT);
  iupClassRegisterAttribute(ic, "MARKMODE", iMatrixGetMarkModeAttrib, iMatrixSetMarkModeAttrib, NULL, NULL, IUPAF_NOT_MAPPED|IUPAF_NO_INHERIT);
  iupClassRegisterAttribute(ic, "AREA", iMatrixGetMarkAreaAttrib, iMatrixSetMarkAreaAttrib, IUPAF_SAMEASSYSTEM, "CONTINUOUS", IUPAF_NOT_MAPPED|IUPAF_NO_INHERIT);
  iupClassRegisterAttribute(ic, "MARKAREA", iMatrixGetMarkAreaAttrib, iMatrixSetMarkAreaAttrib, IUPAF_SAMEASSYSTEM, "CONTINUOUS", IUPAF_NOT_MAPPED|IUPAF_NO_INHERIT);
  iupClassRegisterAttribute(ic, "MULTIPLE", iMatrixGetMarkMultipleAttrib, iMatrixSetMarkMultipleAttrib, NULL, NULL, IUPAF_NOT_MAPPED|IUPAF_NO_INHERIT);
  iupClassRegisterAttribute(ic, "MARKMULTIPLE", iMatrixGetMarkMultipleAttrib, iMatrixSetMarkMultipleAttrib, NULL, NULL, IUPAF_NOT_MAPPED|IUPAF_NO_INHERIT);

  /* IupMatrix Attributes - ACTION (only mapped) */
  iupClassRegisterAttribute(ic, "ADDLIN", NULL, iupMatrixSetAddLinAttrib, NULL, NULL, IUPAF_NOT_MAPPED|IUPAF_WRITEONLY|IUPAF_NO_INHERIT);
  iupClassRegisterAttribute(ic, "DELLIN", NULL, iupMatrixSetDelLinAttrib, NULL, NULL, IUPAF_NOT_MAPPED|IUPAF_WRITEONLY|IUPAF_NO_INHERIT);
  iupClassRegisterAttribute(ic, "ADDCOL", NULL, iupMatrixSetAddColAttrib, NULL, NULL, IUPAF_NOT_MAPPED|IUPAF_WRITEONLY|IUPAF_NO_INHERIT);
  iupClassRegisterAttribute(ic, "DELCOL", NULL, iupMatrixSetDelColAttrib, NULL, NULL, IUPAF_NOT_MAPPED|IUPAF_WRITEONLY|IUPAF_NO_INHERIT);
  iupClassRegisterAttribute(ic, "ORIGIN", iMatrixGetOriginAttrib, iMatrixSetOriginAttrib, NULL, NULL, IUPAF_NO_INHERIT);
  iupClassRegisterAttribute(ic, "SHOW", NULL, iMatrixSetShowAttrib, NULL, NULL, IUPAF_WRITEONLY|IUPAF_NO_INHERIT);
  iupClassRegisterAttribute(ic, "EDIT_MODE", iMatrixGetEditModeAttrib, iMatrixSetEditModeAttrib, NULL, NULL, IUPAF_NO_INHERIT);
  iupClassRegisterAttribute(ic, "REDRAW", NULL, iupMatrixDrawSetRedrawAttrib, NULL, NULL, IUPAF_WRITEONLY|IUPAF_NO_INHERIT);

  /* IupMatrix Attributes - EDITION */
  iupClassRegisterAttribute(ic, "CARET", iMatrixGetCaretAttrib, iMatrixSetCaretAttrib, NULL, NULL, IUPAF_NOT_MAPPED|IUPAF_NO_INHERIT);
  iupClassRegisterAttribute(ic, "SELECTION", iMatrixGetSelectionAttrib, iMatrixSetSelectionAttrib, NULL, NULL, IUPAF_NOT_MAPPED|IUPAF_NO_INHERIT);
  iupClassRegisterAttribute(ic, "MULTILINE", iMatrixGetMultilineAttrib, iMatrixSetMultilineAttrib, NULL, NULL, IUPAF_NO_INHERIT);
  iupClassRegisterAttributeId(ic, "MASK", NULL, NULL, IUPAF_NOT_MAPPED|IUPAF_NO_INHERIT);

  /* IupMatrix Attributes - GENERAL */
  iupClassRegisterAttribute(ic, "USETITLESIZE", iMatrixGetUseTitleSizeAttrib, iMatrixSetUseTitleSizeAttrib, IUPAF_SAMEASSYSTEM, "NO", IUPAF_NOT_MAPPED|IUPAF_NO_INHERIT);
  iupClassRegisterAttribute(ic, "FRAMECOLOR", NULL, NULL, IUPAF_SAMEASSYSTEM, "100 100 100", IUPAF_NO_INHERIT);
  iupClassRegisterAttribute(ic, "READONLY", NULL, NULL, NULL, NULL, IUPAF_NOT_MAPPED|IUPAF_NO_INHERIT);
  iupClassRegisterAttribute(ic, "RESIZEMATRIX", NULL, NULL, NULL, NULL, IUPAF_NOT_MAPPED|IUPAF_NO_INHERIT);
  iupClassRegisterAttribute(ic, "HIDEFOCUS", NULL, NULL, NULL, NULL, IUPAF_NOT_MAPPED|IUPAF_NO_INHERIT);

  /* Overwrite IupCanvas Attributes */
  iupClassRegisterAttribute(ic, "ACTIVE", iupBaseGetActiveAttrib, iMatrixSetActiveAttrib, IUPAF_SAMEASSYSTEM, "YES", IUPAF_DEFAULT);
  iupClassRegisterAttributeId(ic, "BGCOLOR", iMatrixGetBgColorAttrib, iMatrixSetBgColorAttrib, IUPAF_NOT_MAPPED);

  /* IupMatrix Attributes - MASK */
  iupClassRegisterAttribute(ic, "OLD_MASK_DATA", iMatrixGetMaskDataAttrib, NULL, NULL, NULL, IUPAF_NO_STRING|IUPAF_READONLY|IUPAF_NOT_MAPPED|IUPAF_NO_INHERIT);

  if (!IupGetHandle("IupMatrixCrossCursor"))
    iMatrixCreateCursor();

  return ic;
}


/*****************************************************************************************************/


Ihandle* IupMatrix(const char* action)
{
  void *params[2];
  params[0] = (void*)action;
  params[1] = NULL;
  return IupCreatev("matrix", params);
}

void IupMatSetAttribute(Ihandle* ih, const char* a, int l, int c, char* v)
{
  char* attr = iupStrGetMemory(100);
  sprintf(attr, "%s%d:%d", a, l, c);
  IupSetAttribute(ih, attr, v);
}

void IupMatStoreAttribute(Ihandle* ih, const char* a, int l, int c, char* v)
{
  char* attr = iupStrGetMemory(100);
  sprintf(attr, "%s%d:%d", a, l, c);
  IupStoreAttribute(ih, attr, v);
}

char* IupMatGetAttribute(Ihandle* ih, const char* a, int l, int c)
{
  char* attr = iupStrGetMemory(100);
  sprintf(attr, "%s%d:%d", a, l, c);
  return IupGetAttribute(ih, attr);
}

int IupMatGetInt(Ihandle* ih, const char* a, int l, int c)
{
  char* attr = iupStrGetMemory(100);
  sprintf(attr, "%s%d:%d", a, l, c);
  return IupGetInt(ih, attr);
}

float IupMatGetFloat(Ihandle* ih, const char* a, int l, int c)
{
  char* attr = iupStrGetMemory(100);
  sprintf(attr, "%s%d:%d", a, l, c);
  return IupGetFloat(ih, attr);
}

void IupMatSetfAttribute(Ihandle* ih, const char* a, int l, int c, char* f, ...)
{
  static char v[SHRT_MAX];
  char* attr = iupStrGetMemory(100);
  va_list arglist;
  sprintf(attr, "%s%d:%d", a, l, c);
  va_start(arglist, f);
  vsprintf(v, f, arglist);
  va_end(arglist);
  IupStoreAttribute(ih, attr, v);
}
