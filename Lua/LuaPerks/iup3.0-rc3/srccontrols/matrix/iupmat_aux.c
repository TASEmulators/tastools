/** \file
 * \brief iupmatrix control
 * auxiliary functions
 *
 * See Copyright Notice in "iup.h"
 */

#include <stdio.h>
#include <stdlib.h>

#include "iup.h"
#include "iupcbs.h"

#include <cd.h>

#include "iup_object.h"
#include "iup_attrib.h"
#include "iup_str.h"
#include "iup_drvfont.h"
#include "iup_stdcontrols.h"

#include "iupmat_def.h"
#include "iupmat_aux.h"
#include "iupmat_getset.h"


static int iMatrixAuxIsFullVisibleLast(Ihandle* ih, int m)
{
  int i, sum = 0;
  ImatLinColData *p;

  if (m == IMAT_PROCESS_LIN)
    p = &(ih->data->lines);
  else
    p = &(ih->data->columns);

  for(i = p->first; i <= p->last; i++)
    sum  += p->sizes[i];

  if (sum > p->visible_size)
    return 0;
  else
    return 1;
}

int iupMatrixAuxIsCellFullVisible(Ihandle* ih, int lin, int col)
{
  if(((lin >= ih->data->lines.first) &&
      (lin <= ih->data->lines.last) &&
      (col >= ih->data->columns.first) &&
      (col <= ih->data->columns.last)))
  {
    if (col == ih->data->columns.last && !iMatrixAuxIsFullVisibleLast(ih, IMAT_PROCESS_COL))
      return 0;
    if (lin == ih->data->lines.last && !iMatrixAuxIsFullVisibleLast(ih, IMAT_PROCESS_LIN))
      return 0;

    return 1;
  }

  return 0;
}

int iupMatrixAuxIsCellVisible(Ihandle* ih, int lin, int col)
{
  if(((lin >= ih->data->lines.first) &&
      (lin <= ih->data->lines.last) &&
      (col >= ih->data->columns.first) &&
      (col <= ih->data->columns.last)))
  {
    return 1;
  }

  return 0;
}

void iupMatrixAuxGetVisibleCellDim(Ihandle* ih, int lin, int col, int* x, int* y, int* w, int* h)
{
  int i;

  /* find the position where the column starts */
  *x = ih->data->columns.sizes[0];
  for(i = ih->data->columns.first; i < col; i++)
    *x += ih->data->columns.sizes[i];

  /* find the column size */
  *w = ih->data->columns.sizes[col] - 1;

  /* find the position where the line starts */
  *y = ih->data->lines.sizes[0];
  for(i = ih->data->lines.first; i < lin; i++)
    *y += ih->data->lines.sizes[i];

  /* find the line size */
  *h = ih->data->lines.sizes[lin] - 1;
}

/* Calculate the size, in pixels, of the invisible columns/lines,
   the left/above of the first column/line.
   In fact the start position of the visible area.
   Depends on the first visible column/line.
   -> m : choose will operate on lines or columns [IMAT_PROCESS_LIN|IMAT_PROCESS_COL]
*/
void iupMatrixAuxUpdateVisiblePos(Ihandle* ih, int m)
{
  char* POS;
  int i, sb, visible_pos;
  ImatLinColData *p;

  if (m == IMAT_PROCESS_LIN)
  {
    p = &(ih->data->lines);
    sb = IUP_SB_VERT;
    POS = "POSY";
  }
  else
  {
    p = &(ih->data->columns);
    sb = IUP_SB_HORIZ;
    POS = "POSX";
  }

  visible_pos = 0;
  for(i = 1; i < p->first; i++)
    visible_pos += p->sizes[i];

  if (ih->data->canvas.sb & sb)
  {
    float pos;

    if (p->total_size)
    {
      while ((visible_pos + p->visible_size > p->total_size) && p->first>1)
      {
        /* invalid position, must recalculate first */
        p->first--;
        visible_pos -= p->sizes[p->first];
      }

      pos = (float)visible_pos/(float)p->total_size;
    }
    else
      pos = 0;

    iupMatrixAuxUpdateLast(p);
    IupSetfAttribute(ih, POS, "%.5f", (double)pos);
  }
  else
    iupMatrixAuxUpdateLast(p);
}

/* Calculate which is the last visible column/line of the matrix. 
   Depends on the first visible column/line.  */
void iupMatrixAuxUpdateLast(ImatLinColData *p)
{
  int i, sum = 0;

  if (p->visible_size > 0)
  {
    /* Find which is the last column/line
       Start in the first visible and continue adding the widths
       up to the visible size   */
    for(i = p->first; i < p->num; i++)
    {
      sum  += p->sizes[i];
      if(sum >= p->visible_size)
        break;
    }

    if (i == p->num)
      p->last = i-1;
    else
      p->last = i;
  }
  else
  {
    /* There is no space for any column, set the last column as 0 */
    p->last = 0;
  }
}

int iupMatrixAuxGetColumnWidth(Ihandle* ih, int col)
{
  int width = 0, pixels = 0;
  char* str = iupStrGetMemory(100);
  char* value;

  /* can only be called for valid columns */

  sprintf(str, "WIDTH%d", col);
  value = iupAttribGet(ih, str);
  if (!value)
  {
    sprintf(str, "RASTERWIDTH%d", col);
    value = iupAttribGet(ih, str);
    if (value)
      pixels = 1;
  }

  if (!value)
  {
    /* Use the titles to define the size */
    if (col == 0)
    {
      if (!ih->data->callback_mode || ih->data->use_title_size)
      {
        /* find the largest title */
        int lin, max_width = 0;
        for(lin = 0; lin < ih->data->lines.num; lin++)
        {
          char* title_value = iupMatrixCellGetValue(ih, lin, 0);
          if (title_value)
          {
            iupdrvFontGetMultiLineStringSize(ih, title_value, &width, NULL);
            if (width > max_width)
              max_width = width;
          }
        }
        width = max_width;
      }
    }
    else if (ih->data->use_title_size)
    {
      char* title_value = iupMatrixCellGetValue(ih, 0, col);
      if (title_value)
        iupdrvFontGetMultiLineStringSize(ih, title_value, &width, NULL);
    }
    if (width)
      return width + IMAT_PADDING_W + IMAT_FRAME_W;

    if (col != 0)
      value = iupAttribGetStr(ih, "WIDTHDEF");
  }

  if (iupStrToInt(value, &width))
  {
    if (width <= 0)
      return 0;
    else
    {
      if (pixels)
        return width + IMAT_PADDING_W + IMAT_FRAME_W;
      else
      {
        int charwidth;
        iupdrvFontGetCharSize(ih, &charwidth, NULL);
        return iupWIDTH2RASTER(width, charwidth) + IMAT_PADDING_W + IMAT_FRAME_W;
      }
    }
  }
  return 0;
}

int iupMatrixAuxGetLineHeight(Ihandle* ih, int lin)
{
  int height = 0, pixels = 0;
  char* str = iupStrGetMemory(100);
  char* value;

  /* can only be called for valid lines */

  sprintf(str, "HEIGHT%d", lin);
  value = iupAttribGet(ih, str);
  if(!value)
  {
    sprintf(str, "RASTERHEIGHT%d", lin);
    value = iupAttribGet(ih, str);
    if(value)
      pixels = 1;
  }

  if (!value)
  {
    /* Use the titles to define the size */
    if (lin == 0)
    {
      if (!ih->data->callback_mode || ih->data->use_title_size)
      {
        /* find the highest title */
        int col, max_height = 0;
        for(col = 0; col < ih->data->columns.num; col++)
        {
          char* title_value = iupMatrixCellGetValue(ih, 0, col);
          if (title_value && title_value[0])
          {
            iupdrvFontGetMultiLineStringSize(ih, title_value, NULL, &height);
            if (height > max_height)
              max_height = height;
          }
        }
        height = max_height;
      }
    }
    else if (ih->data->use_title_size)
    {
      char* title_value = iupMatrixCellGetValue(ih, lin, 0);
      if (title_value && title_value[0])
        iupdrvFontGetMultiLineStringSize(ih, title_value, NULL, &height);
    }
    if (height)
      return height + IMAT_PADDING_H + IMAT_FRAME_H;

    if (lin != 0)
      value = iupAttribGetStr(ih, "HEIGHTDEF");
  }
  
  if (iupStrToInt(value, &height))
  {
    if (height <= 0)
      return 0;
    else
    {
      if (pixels)
        return height + IMAT_PADDING_H + IMAT_FRAME_H;
      else
      {
        int charheight;
        iupdrvFontGetCharSize(ih, NULL, &charheight);
        return iupHEIGHT2RASTER(height, charheight) + IMAT_PADDING_H + IMAT_FRAME_H;
      }
    }
  }
  return 0;
}

/* Fill the sizes vector with the width/heigh of all the columns/lines.
   Calculate the value of total_size */
static void iMatrixAuxFillSizeVec(Ihandle* ih, int m)
{
  int i;
  ImatLinColData *p;

  if (m == IMAT_PROCESS_LIN)
    p = &(ih->data->lines);
  else
    p = &(ih->data->columns);

  /* Calculate total width/height of the matrix and the width/height of each column */
  p->total_size = 0;
  for(i = 0; i < p->num; i++)
  {
    if (m == IMAT_PROCESS_LIN)
      p->sizes[i] = iupMatrixAuxGetLineHeight(ih, i);
    else
      p->sizes[i] = iupMatrixAuxGetColumnWidth(ih, i);

    if (i > 0)
      p->total_size += p->sizes[i];
  }
}

static void iMatrixAuxUpdateVisibleSize(Ihandle* ih, int m)
{
  char* D;
  ImatLinColData *p;
  int canvas_size;

  if (m == IMAT_PROCESS_LIN)
  {
    D = "DY";
    p = &(ih->data->lines);
    canvas_size = ih->data->h;
  }
  else
  {
    D = "DX";
    p = &(ih->data->columns);
    canvas_size = ih->data->w;
  }

  /* Matrix useful area is the current size minus the title area */
  p->visible_size = canvas_size - p->sizes[0];
  if (p->visible_size > p->total_size)
    p->visible_size = p->total_size;

  if (p->total_size)
    IupSetfAttribute(ih, D, "%f", (double)p->visible_size/(double)p->total_size);
  else
    IupSetAttribute(ih, D, "1.0");
}

void iupMatrixAuxCalcSizes(Ihandle* ih)
{
  iMatrixAuxFillSizeVec(ih, IMAT_PROCESS_COL);
  iMatrixAuxFillSizeVec(ih, IMAT_PROCESS_LIN);

  iMatrixAuxUpdateVisibleSize(ih, IMAT_PROCESS_COL);
  iMatrixAuxUpdateVisibleSize(ih, IMAT_PROCESS_LIN);

  /* when removing lines the first can be positioned after the last line */
  if (ih->data->lines.first > ih->data->lines.num-1) 
  {
    if (ih->data->lines.num==1)
      ih->data->lines.first = 1;
    else
      ih->data->lines.first = ih->data->lines.num-1;
  }
  if (ih->data->columns.first > ih->data->columns.num-1) 
  {
    if (ih->data->columns.num == 1)
      ih->data->columns.first = 1;
    else
      ih->data->columns.first = ih->data->columns.num-1;
  }

  iupMatrixAuxUpdateVisiblePos(ih, IMAT_PROCESS_COL);
  iupMatrixAuxUpdateVisiblePos(ih, IMAT_PROCESS_LIN);

  ih->data->need_calcsize = 0;
}

int iupMatrixAuxGetLinColFromXY(Ihandle* ih, int x, int y, int* l, int* c)
{
  int size, lin, col;

  size = ih->data->columns.sizes[0]; /* always visible when non zero */
  if (x < size)
    col = 0;  /* It is in the column of titles */
  else
  {
    for(col = ih->data->columns.first; col <= ih->data->columns.last; col++)  /* for all visible columns */
    {
      size += ih->data->columns.sizes[col];
      if (x < size)
        break;
    }
    if (col > ih->data->columns.last)
      col = -1;
  }

  size = ih->data->lines.sizes[0]; /* always visible when non zero */
  if (y < size)
    lin = 0;  /* It is in the line of titles */
  else
  {
    for(lin = ih->data->lines.first; lin <= ih->data->lines.last; lin++)  /* for all visible lines */
    {
      size += ih->data->lines.sizes[lin];
      if (y < size)
        break;
    }
    if(lin > ih->data->lines.last)
      lin = -1;
  }

  if (col == -1 || lin == -1)
    return 0;

  *l = lin;
  *c = col;
  return 1;
}

int iupMatrixAuxCallLeaveCellCb(Ihandle* ih)
{
  IFnii cb = (IFnii)IupGetCallback(ih, "LEAVEITEM_CB");
  if(cb)
    return cb(ih, ih->data->lines.focus_cell, ih->data->columns.focus_cell);
  return IUP_DEFAULT;
}

void iupMatrixAuxCallEnterCellCb(Ihandle* ih)
{
  IFnii cb = (IFnii)IupGetCallback(ih, "ENTERITEM_CB");
  if (cb)
    cb(ih, ih->data->lines.focus_cell, ih->data->columns.focus_cell);
}

int iupMatrixAuxCallEditionCbLinCol(Ihandle* ih, int lin, int col, int mode, int update)
{
  IFniiii cb;

  if (iupAttribGetBoolean(ih, "READONLY"))
    return IUP_IGNORE;

  cb = (IFniiii)IupGetCallback(ih, "EDITION_CB");
  if(cb)
    return cb(ih, lin, col, mode, update);
  return IUP_DEFAULT;
}
