/** \file
 * \brief iupmatrix control
 * scrolling
 *
 * See Copyright Notice in "iup.h"
 */

#include "iup.h"
#include "iupcbs.h"

#include <cd.h>

#include "iup_object.h"
#include "iup_stdcontrols.h"

#include "iupmat_def.h"
#include "iupmat_scroll.h"
#include "iupmat_focus.h"
#include "iupmat_aux.h"
#include "iupmat_edit.h"
#include "iupmat_draw.h"


/* Macros used by "dir" parameter of the iMatrixScrollLine and iMatrixScrollColumn functions */
#define IMAT_SCROLL_LEFT    0
#define IMAT_SCROLL_RIGHT   1
#define IMAT_SCROLL_UP      2
#define IMAT_SCROLL_DOWN    3


/**************************************************************************/
/* Private functions                                                      */
/**************************************************************************/


static int iMatrixScrollIsFullVisibleLast(ImatLinColData *p)
{
  int i, sum = 0;

  for(i = p->first; i <= p->last; i++)
    sum  += p->sizes[i];

  if (sum > p->visible_size)
    return 0;
  else
    return 1;
}

/* Scroll columns/lines in the left/top side of the matriz until the last column/line is FULLY visible.
   -> m : Define the mode of operation: lines or columns [IMAT_PROCESS_LIN|IMAT_PROCESS_COL] */
static void iMatrixScrollToVisible(Ihandle* ih, int m, int index)
{
  ImatLinColData* p;

  if (m == IMAT_PROCESS_LIN)
    p = &(ih->data->lines);
  else
    p = &(ih->data->columns);

  if (index < p->first)
  {
    p->first = index;
    return;
  }
  else if (index > p->last)
  {
    /* Increment the first column/line until the index is visible */
    while(index > p->last && p->last != (p->num - 1))
    {
      p->first++;
      iupMatrixAuxUpdateLast(p);
    } 
  }

  if (index == p->last)
  {
    /* must increment util the last is fully visible */
    while(index == p->last && p->last != (p->num - 1) && !iMatrixScrollIsFullVisibleLast(p))
    {
      p->first++;
      iupMatrixAuxUpdateLast(p);
    } 
  }
}

/* Callback to report to the user which visualization area of
   the matrix changed. */
static void iMatrixScrollCallScrollTopCb(Ihandle* ih)
{
  IFnii cb = (IFnii)IupGetCallback(ih, "SCROLLTOP_CB");
  if (cb) 
    cb(ih, ih->data->lines.first, ih->data->columns.first);
}

static int iMatrixScrollGetNextNonEmpty(Ihandle* ih, int m, int index)
{
  ImatLinColData* p;

  if (m == IMAT_PROCESS_LIN)
    p = &(ih->data->lines);
  else
    p = &(ih->data->columns);

  while(index < p->num && p->sizes[index] == 0)
    index++;

  if (index > p->num-1)
  {
    if (p->num == 1)
      return 1;
    else
      return p->num-1;
  }
  else
    return index;
}

static int iMatrixScrollGetPrevNonEmpty(Ihandle* ih, int m, int index)
{
  ImatLinColData* p;

  if (m == IMAT_PROCESS_LIN)
    p = &(ih->data->lines);
  else
    p = &(ih->data->columns);

  while(index > 0 && p->sizes[index] == 0)
    index--;

  if (index < 1)
    return 1;
  else
    return index;
}

static void iMatrixScrollSetFocusScrollToVisible(Ihandle* ih, int m, int index)
{
  if (m == IMAT_PROCESS_COL)
    iupMatrixFocusSet(ih, ih->data->lines.focus_cell, index);
  else
    iupMatrixFocusSet(ih, index, ih->data->columns.focus_cell);

  /* set for both because current focus maybe hidden */
  iMatrixScrollToVisible(ih, IMAT_PROCESS_COL, ih->data->columns.focus_cell);
  iMatrixScrollToVisible(ih, IMAT_PROCESS_LIN, ih->data->lines.focus_cell);
}

/**************************************************************************/
/*                      Exported functions                                */
/**************************************************************************/


/* Move using the cells of matrix.
   Receive as a parameter a pointer to a function that will make the work,
   in fact. This is done to avoid a test to each of the manipulation cursor
   functions, verifying if it is necessary to call or not the scroll
   callback. This is only done here.
   -> func - pointer to the function that will make the movement
   -> mode - parameter passed to func, specify if the movement request is of
             the scrollbar or the keyboard
   -> pos  - parameter passed to func, that will be the handle position function
             of the scrollbar, returning the scrollbar thumb position...
             if func is other function, this parameter will be ignored
   -> m    - parameter passed to func, specify which is the mode of operation:
             lines or columns [IMAT_PROCESS_LIN|IMAT_PROCESS_COL]
*/
void iupMatrixScrollMoveCursor(iupMatrixScrollMoveF func, Ihandle* ih, int mode, float pos, int m)
{
  int old_lines_first = ih->data->lines.first;
  int old_columns_first = ih->data->columns.first;

  iupMatrixEditForceHidden(ih);

  func(ih, mode, pos, m);

  if (ih->data->lines.first != old_lines_first || ih->data->columns.first != old_columns_first)
  {
    if (ih->data->columns.first != old_columns_first)
      iupMatrixAuxUpdateVisiblePos(ih, IMAT_PROCESS_COL);

    if (ih->data->lines.first != old_lines_first)
      iupMatrixAuxUpdateVisiblePos(ih, IMAT_PROCESS_LIN);

    iMatrixScrollCallScrollTopCb(ih);

    iupMatrixDraw(ih, 0);
  }
}

void iupMatrixScrollToVisible(Ihandle* ih, int lin, int col)
{
  int old_lines_first = ih->data->lines.first;
  int old_columns_first = ih->data->columns.first;

  iMatrixScrollToVisible(ih, IMAT_PROCESS_COL, col);
  iMatrixScrollToVisible(ih, IMAT_PROCESS_LIN, lin);

  if (ih->data->lines.first != old_lines_first || ih->data->columns.first != old_columns_first)
  {
    if (ih->data->columns.first != old_columns_first)
      iupMatrixAuxUpdateVisiblePos(ih, IMAT_PROCESS_COL);

    if (ih->data->lines.first != old_lines_first)
      iupMatrixAuxUpdateVisiblePos(ih, IMAT_PROCESS_LIN);

    iMatrixScrollCallScrollTopCb(ih);

    iupMatrixDraw(ih, 1);
  }
}

/* This function is called when the "home" key is pressed.
   In the first time, go to the beginning of the line.
   In the second time, go to the beginning of the page.
   In the third time, go to the beginning of the matrix.
   -> mode and pos : DO NOT USED.
*/
void iupMatrixScrollHome(Ihandle* ih, int unused_mode, float unused_pos, int unused_m)
{
  (void)unused_m;
  (void)unused_mode;
  (void)unused_pos;

  /* called only for mode==IMAT_SCROLLKEY */

  if(ih->data->homekeycount == 0)  /* go to the beginning of the line */
  {
    ih->data->columns.first = iMatrixScrollGetNextNonEmpty(ih, IMAT_PROCESS_COL, 1);
    iMatrixScrollSetFocusScrollToVisible(ih, IMAT_PROCESS_COL, ih->data->columns.first);
  }
  else if(ih->data->homekeycount == 1)   /* go to the beginning of the visible page */
  {
    iupMatrixFocusSet(ih, ih->data->lines.first, ih->data->columns.first);

    /* set for both because current focus maybe hidden */
    iMatrixScrollToVisible(ih, IMAT_PROCESS_COL, ih->data->columns.focus_cell);
    iMatrixScrollToVisible(ih, IMAT_PROCESS_LIN, ih->data->lines.focus_cell);
  }
  else if(ih->data->homekeycount == 2)   /* go to the beginning of the matrix 1:1 */
  {
    ih->data->columns.first = iMatrixScrollGetNextNonEmpty(ih, IMAT_PROCESS_COL, 1);
    ih->data->lines.first = iMatrixScrollGetNextNonEmpty(ih, IMAT_PROCESS_LIN, 1);

    iupMatrixFocusSet(ih, ih->data->lines.first, ih->data->columns.first);

    /* set for both because current focus maybe hidden */
    iMatrixScrollToVisible(ih, IMAT_PROCESS_COL, ih->data->columns.focus_cell);
    iMatrixScrollToVisible(ih, IMAT_PROCESS_LIN, ih->data->lines.focus_cell);
  }
}

/* This function is called when the "end" key is pressed.
   In the first time, go to the end of the line.
   In the second time, go to the end of the page.
   In the third time, go to the end of the matrix.
   -> mode and pos : DO NOT USED.
*/
void iupMatrixScrollEnd(Ihandle* ih, int unused_mode, float unused_pos, int unused_m)
{
  (void)unused_m;
  (void)unused_mode;
  (void)unused_pos;

  /* called only for mode==IMAT_SCROLLKEY */

  if(ih->data->endkeycount == 0)  /* go to the end of the line */
  {
    int last_col = iMatrixScrollGetPrevNonEmpty(ih, IMAT_PROCESS_COL, ih->data->columns.num-1);
    iMatrixScrollSetFocusScrollToVisible(ih, IMAT_PROCESS_COL, last_col);
  }
  else if(ih->data->endkeycount == 1)   /* go to the end of the visible page */
  {
    iupMatrixFocusSet(ih, ih->data->lines.last, ih->data->columns.last);

    /* set for both because current focus maybe hidden */
    iMatrixScrollToVisible(ih, IMAT_PROCESS_COL, ih->data->columns.focus_cell);
    iMatrixScrollToVisible(ih, IMAT_PROCESS_LIN, ih->data->lines.focus_cell);
  }
  else if(ih->data->endkeycount == 2)   /* go to the end of the matrix */
  {
    int last_col = iMatrixScrollGetPrevNonEmpty(ih, IMAT_PROCESS_COL, ih->data->columns.num-1);
    int last_lin = iMatrixScrollGetPrevNonEmpty(ih, IMAT_PROCESS_LIN, ih->data->lines.num-1);

    iupMatrixFocusSet(ih, last_lin, last_col);

    /* set for both because current focus maybe hidden */
    iMatrixScrollToVisible(ih, IMAT_PROCESS_COL, ih->data->columns.focus_cell);
    iMatrixScrollToVisible(ih, IMAT_PROCESS_LIN, ih->data->lines.focus_cell);
  }
}

/* This function is called to move a cell to the left or up.
   -> mode : indicate if the command was from the keyboard or the scrollbar. If scrollbar,
             do not change the focus.
   -> pos  : DO NOT USED
   -> m    : define the mode of operation: lines or columns [IMAT_PROCESS_LIN|IMAT_PROCESS_COL]
*/
void iupMatrixScrollLeftUp(Ihandle* ih, int mode, float pos, int m)
{
  ImatLinColData* p;
  (void)pos;

  if(m == IMAT_PROCESS_LIN)
    p = &(ih->data->lines);
  else
    p = &(ih->data->columns);

  if (mode == IMAT_SCROLLKEY)
  {
    int next = iMatrixScrollGetPrevNonEmpty(ih, m, p->focus_cell-1);
    iMatrixScrollSetFocusScrollToVisible(ih, m, next);
  }
  else  /* IMAT_SCROLLBAR */
  {
    p->first = iMatrixScrollGetPrevNonEmpty(ih, m, p->first-1);
  }
}

/* This function is called to move a cell to the right or down.
   -> mode : indicate if the command from the keyboard or the scrollbar. If scrollbar,
             do not change the focus.
   -> pos  : DO NOT USED
   -> m    : define the mode of operation: lines or columns [IMAT_PROCESS_LIN|IMAT_PROCESS_COL]
*/
void iupMatrixScrollRightDown(Ihandle* ih, int mode, float pos, int m)
{
  ImatLinColData* p;
  (void)pos;

  if(m == IMAT_PROCESS_LIN)
    p = &(ih->data->lines);
  else
    p = &(ih->data->columns);

  if (mode == IMAT_SCROLLKEY)
  {
    int next = iMatrixScrollGetNextNonEmpty(ih, m, p->focus_cell+1);
    iMatrixScrollSetFocusScrollToVisible(ih, m, next);
  }
  else  /* IMAT_SCROLLBAR */
  {
    p->first = iMatrixScrollGetNextNonEmpty(ih, m, p->first+1);
  }
}

/* This function is called to move a page to the left or up.
   -> mode : indicate if the command was from the keyboard or the scrollbar. If scrollbar,
             do not change the focus.
   -> pos  : DO NOT USED
   -> m    : define the mode of operation: lines (PgLeft) or columns (PgUp) [IMAT_PROCESS_LIN|IMAT_PROCESS_COL]
*/
void iupMatrixScrollPgLeftUp(Ihandle* ih, int mode, float pos, int m)
{
  ImatLinColData* p;
  (void)pos;

  if(m == IMAT_PROCESS_LIN)
    p = &(ih->data->lines);
  else
    p = &(ih->data->columns);

  if (mode == IMAT_SCROLLKEY)
  {
    int next = iMatrixScrollGetPrevNonEmpty(ih, m, p->focus_cell - (p->last - p->first));
    iMatrixScrollSetFocusScrollToVisible(ih, m, next);
  }
  else  /* IMAT_SCROLLBAR */
  {
    p->first = iMatrixScrollGetPrevNonEmpty(ih, m, p->first - (p->last - p->first));
  }
}

/* This function is called to move a page to the right or down.
   -> mode : indicate if the command was from the keyboard or the scrollbar. If scrollbar,
             do not change the focus.
   -> pos  : DO NOT USED
   -> m    : define the mode of operation: lines (PgDown) or columns (PgRight) [IMAT_PROCESS_LIN|IMAT_PROCESS_COL]
*/
void iupMatrixScrollPgRightDown(Ihandle* ih, int mode, float pos, int m)
{
  ImatLinColData* p;
  (void)pos;

  if(m == IMAT_PROCESS_LIN)
    p = &(ih->data->lines);
  else
    p = &(ih->data->columns);

  if (mode == IMAT_SCROLLKEY)
  {
    int next = iMatrixScrollGetNextNonEmpty(ih, IMAT_PROCESS_COL, p->focus_cell + (p->last - p->first));
    iMatrixScrollSetFocusScrollToVisible(ih, m, next);
  }
  else  /* IMAT_SCROLLBAR */
  {
    p->first = iMatrixScrollGetPrevNonEmpty(ih, m, p->first + (p->last - p->first));
  }
}

void iupMatrixScrollCr(Ihandle* ih, int unused_mode, float unused_pos, int unused_m)
{
  int oldlin = ih->data->lines.focus_cell;
  int oldcol = ih->data->columns.focus_cell;
  (void)unused_m;
  (void)unused_mode;
  (void)unused_pos;

  /* called only for mode==IMAT_SCROLLKEY */

  /* try the normal processing of next cell down */
  iupMatrixScrollRightDown(ih, IMAT_SCROLLKEY, 0, IMAT_PROCESS_LIN);

  if(ih->data->lines.focus_cell == oldlin && ih->data->columns.focus_cell == oldcol)
  {
    /* If focus was not changed, it was because it is in the last line of the column.
       Go to the next column of the same line. */
    iupMatrixScrollRightDown(ih, IMAT_SCROLLKEY, 0, IMAT_PROCESS_COL);
  }
}

/* This function is called when a drag is performed in the scrollbar.
   -> x    : scrollbar thumb position, value between 0 and 1
   -> mode : DO NOT USED
   -> m    : define the mode of operation: lines or columns [IMAT_PROCESS_LIN|IMAT_PROCESS_COL]
*/
void iupMatrixScrollPos(Ihandle* ih, int mode, float pos, int m)
{
  int scroll_pos, index, vp;
  float d;
  ImatLinColData* p;
  (void)mode;

  if (m == IMAT_PROCESS_LIN)
  {
    p = &(ih->data->lines);
    d = IupGetFloat(ih, "DY");
  }
  else
  {
    p = &(ih->data->columns);
    d = IupGetFloat(ih, "DX");
  }

  if (p->num == 1)
  {
    p->first = 1;
    return;
  }

  scroll_pos = (int)(pos * p->total_size + 0.5);

  vp = 0;
  for(index = 1; index < p->num; index++)
  {
    vp += p->sizes[index];
    if (vp > scroll_pos)
     break;
  }

  if (index == p->num)
  {
    if (p->num == 1)
      index = 1;
    else
      index = p->num-1;
  }

  p->first = index;
}

int iupMatrixScroll_CB(Ihandle* ih, int action, float x, float y)
{
  if (!iupMatrixIsValid(ih, 0))
    return IUP_DEFAULT;

  switch(action)
  {
    case IUP_SBUP      : iupMatrixScrollUp(ih);       break;
    case IUP_SBDN      : iupMatrixScrollDown(ih);     break;
    case IUP_SBPGUP    : iupMatrixScrollPgUp(ih);     break;
    case IUP_SBPGDN    : iupMatrixScrollPgDown(ih);   break;
    case IUP_SBRIGHT   : iupMatrixScrollRight(ih);    break;
    case IUP_SBLEFT    : iupMatrixScrollLeft(ih);     break;
    case IUP_SBPGRIGHT : iupMatrixScrollPgRight(ih);  break;
    case IUP_SBPGLEFT  : iupMatrixScrollPgLeft(ih);   break;
    case IUP_SBPOSV    : iupMatrixScrollPosVer(ih,y); break;
    case IUP_SBPOSH    : iupMatrixScrollPosHor(ih,x); break;
    case IUP_SBDRAGV   : iupMatrixScrollPosVer(ih,y); break;
    case IUP_SBDRAGH   : iupMatrixScrollPosHor(ih,x); break;
  }

  iupMatrixDrawUpdate(ih);

  return IUP_DEFAULT;
}
