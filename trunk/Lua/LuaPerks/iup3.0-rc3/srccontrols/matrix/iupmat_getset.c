/** \file
 * \brief iupmatrix setget control
 * attributes set and get
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
#include "iup_drvfont.h"
#include "iup_str.h"
#include "iup_stdcontrols.h"
#include "iup_controls.h"
#include "iup_childtree.h"

#include "iupmat_def.h"
#include "iupmat_getset.h"
#include "iupmat_edit.h"
#include "iupmat_draw.h"
#include "iupmat_aux.h"


int iupMatrixCheckCellPos(Ihandle* ih, int lin, int col)
{
  /* Check if the cell exists */
  if ((lin < 0) || (col < 0) || (lin > ih->data->lines.num-1) || (col > ih->data->columns.num-1))
    return 0;

  return 1;
}

void iupMatrixCellSetValue(Ihandle* ih, int lin, int col, const char* value)
{
  if (ih->data->callback_mode)
    return;

  if (ih->data->cells[lin][col].value)
    free(ih->data->cells[lin][col].value);

  ih->data->cells[lin][col].value = iupStrDup(value);

  if (lin==0 || col==0)
    ih->data->need_calcsize = 1;
}

char* iupMatrixCellGetValue (Ihandle* ih, int lin, int col)
{
  if (!ih->handle)
  {
    char str[100];
    sprintf(str, "%d:%d", lin, col);
    return iupAttribGet(ih, str);
  }
  else
  {
    if (ih->data->callback_mode)
    {
      sIFnii value_cb = (sIFnii)IupGetCallback(ih, "VALUE_CB");
      return value_cb(ih, lin, col);
    }
    else
      return ih->data->cells[lin][col].value;
  }
}

void iupMatrixCellSetFlag(Ihandle* ih, int lin, int col, unsigned char attr, int set)
{
  if (!ih->handle)
    return;

  if (lin==-1)
  {
    if ((col < 0) || (col > ih->data->columns.num-1))
      return;

    if (set)
      ih->data->columns.flags[col] |= attr;
    else
      ih->data->columns.flags[col] &= ~attr;
  }
  else if (col == -1)
  {
    if ((lin < 0) || (lin > ih->data->lines.num-1))
      return;

    if (set)
      ih->data->lines.flags[lin] |= attr;
    else
      ih->data->lines.flags[lin] &= ~attr;
  }
  else
  {
    if (!ih->data->callback_mode)
    {
      if ((lin < 0) || (col < 0) || (lin > ih->data->lines.num-1) || (col > ih->data->columns.num-1))
        return;

      if (set)
        ih->data->cells[lin][col].flags |= attr;
      else
        ih->data->cells[lin][col].flags &= ~attr;
    }
  }
}

void iupMatrixCellUpdateValue(Ihandle* ih)
{
  IFniis value_edit_cb;
  char *value = iupMatrixEditGetValue(ih);

  iupMatrixCellSetValue(ih, ih->data->lines.focus_cell, ih->data->columns.focus_cell, value);

  value_edit_cb = (IFniis)IupGetCallback(ih, "VALUE_EDIT_CB");
  if (value_edit_cb)
    value_edit_cb(ih, ih->data->lines.focus_cell, ih->data->columns.focus_cell, value);

  iupMatrixPrepareDrawData(ih);
  iupMatrixDrawCells(ih, ih->data->lines.focus_cell, ih->data->columns.focus_cell, ih->data->lines.focus_cell, ih->data->columns.focus_cell);
}

void iupMatrixPrepareDrawData(Ihandle* ih)
{
  /* FGCOLOR, BGCOLOR and FONT */
  iupAttribStoreStr(ih, "_IUPMAT_BGCOLOR_PARENT", iupControlBaseGetParentBgColor(ih));
  iupAttribStoreStr(ih, "_IUPMAT_BGCOLOR", IupGetAttribute(ih, "BGCOLOR"));
  iupAttribStoreStr(ih, "_IUPMAT_FGCOLOR", IupGetAttribute(ih, "FGCOLOR"));
  iupAttribStoreStr(ih, "_IUPMAT_FONT", IupGetAttribute(ih, "FONT"));

  ih->data->bgcolor_parent = iupAttribGet(ih, "_IUPMAT_BGCOLOR_PARENT");
  ih->data->bgcolor = iupAttribGet(ih, "_IUPMAT_BGCOLOR");
  ih->data->fgcolor = iupAttribGet(ih, "_IUPMAT_FGCOLOR");
  ih->data->font = iupAttribGet(ih, "_IUPMAT_FONT");

  ih->data->font_cb = (sIFnii)IupGetCallback(ih, "FONT_CB");
  ih->data->fgcolor_cb = (IFniiIII)IupGetCallback(ih, "FGCOLOR_CB");
  ih->data->bgcolor_cb = (IFniiIII)IupGetCallback(ih, "BGCOLOR_CB");
}

static char* iMatrixGetCellAttrib(Ihandle* ih, unsigned char attr, int lin, int col, int native_parent)
{
  char* value = NULL;
  char attrib_id[100];
  const char* attrib = NULL;
  char* attrib_global = NULL; 

  if (attr == IUPMAT_FONT)
  {
    attrib = "FONT";
    attrib_global = ih->data->font;
  }
  else if (attr == IUPMAT_BGCOLOR)
  {
    attrib = "BGCOLOR";
    attrib_global = ih->data->bgcolor;
  }
  else if (attr == IUPMAT_FGCOLOR)
  {
    attrib = "FGCOLOR";
    attrib_global = ih->data->fgcolor;
  }

  /* 1 -  check for this cell */
  if (ih->data->callback_mode || ih->data->cells[lin][col].flags & attr)
  {
    sprintf(attrib_id, "%s%d:%d", attrib, lin, col);
    value = iupAttribGet(ih, attrib_id);
  }
  if (!value)
  {
    /* 2 - check for this line, if not title col */
    if (col != 0)
    {
      if (ih->data->lines.flags[lin] & attr)
      {
        sprintf(attrib_id, "%s%d:*", attrib, lin);
        value = iupAttribGet(ih, attrib_id);
      }
    }

    if (!value)
    {
      /* 3 - check for this column, if not title line */
      if (lin != 0)
      {
        if (ih->data->columns.flags[col] & attr)
        {
          sprintf(attrib_id,"%s*:%d", attrib, col);
          value = iupAttribGet(ih, attrib_id);
        }
      }

      if (!value)
      {
        /* 4 - check for the matrix or native parent */
        if (native_parent)
          value = ih->data->bgcolor_parent;
        else
          value = attrib_global;
      }
    }
  }

  return value;
}

static int iMatrixCallColorCB(Ihandle* ih, IFniiIII cb, int lin, int col, unsigned char *r, unsigned char *g, unsigned char *b)
{
  int ir, ig, ib, ret;
  ret = cb(ih, lin, col, &ir, &ig, &ib);
  *r = iupBYTECROP(ir);
  *g = iupBYTECROP(ig);
  *b = iupBYTECROP(ib);
  return ret;
}

char* iupMatrixGetFgColor(Ihandle* ih, int lin, int col)
{
  unsigned char r = 0, g = 0, b = 0;
  /* called from Edit only */
  if (!ih->data->fgcolor_cb || (iMatrixCallColorCB(ih, ih->data->fgcolor_cb, lin, col, &r, &g, &b) == IUP_IGNORE))
  {
    char* fgcolor = iMatrixGetCellAttrib(ih, IUPMAT_FGCOLOR, lin, col, 0);
    if (!fgcolor) 
    {
      if (lin ==0 || col == 0)
        fgcolor = IupGetGlobal("DLGFGCOLOR");
      else
        fgcolor = IupGetGlobal("TXTFGCOLOR");
    }
    return fgcolor;
  }
  else
  {
    char* buffer = iupStrGetMemory(30);
    sprintf(buffer, "%d %d %d", r, g, b);
    return buffer;
  }
}

void iupMatrixGetFgRGB(Ihandle* ih, int lin, int col, unsigned char *r, unsigned char *g, unsigned char *b)
{
  /* called from Draw only */
  if (!ih->data->fgcolor_cb || (iMatrixCallColorCB(ih, ih->data->fgcolor_cb, lin, col, r, g, b) == IUP_IGNORE))
  {
    char* fgcolor = iMatrixGetCellAttrib(ih, IUPMAT_FGCOLOR, lin, col, 0);
    if (!fgcolor) 
    {
      if (lin ==0 || col == 0)
        fgcolor = IupGetGlobal("DLGFGCOLOR");
      else
        fgcolor = IupGetGlobal("TXTFGCOLOR");
    }
    iupStrToRGB(fgcolor, r, g, b);
  }
}

char* iupMatrixGetBgColor(Ihandle* ih, int lin, int col)
{
  unsigned char r = 0, g = 0, b = 0;
  /* called from Edit only */
  if (!ih->data->bgcolor_cb || (iMatrixCallColorCB(ih, ih->data->bgcolor_cb, lin, col, &r, &g, &b) == IUP_IGNORE))
  {
    int native_parent = 0;
    if (lin == 0 || col == 0)
      native_parent = 1;
    return iMatrixGetCellAttrib(ih, IUPMAT_BGCOLOR, lin, col, native_parent);
  }
  else
  {
    char* buffer = iupStrGetMemory(30);
    sprintf(buffer, "%d %d %d", r, g, b);
    return buffer;
  }
}

#define IMAT_DARKER(_x)    (((_x)*9)/10)

void iupMatrixGetBgRGB(Ihandle* ih, int lin, int col, unsigned char *r, unsigned char *g, unsigned char *b)
{
  /* called from Draw only */
  if (!ih->data->bgcolor_cb || (iMatrixCallColorCB(ih, ih->data->bgcolor_cb, lin, col, r, g, b) == IUP_IGNORE))
  {
    int native_parent = 0;
    if (lin == 0 || col == 0)
      native_parent = 1;
    iupStrToRGB(iMatrixGetCellAttrib(ih, IUPMAT_BGCOLOR, lin, col, native_parent), r, g, b);
    if (native_parent)
    {
      int ir = IMAT_DARKER(*r), ig=IMAT_DARKER(*g), ib=IMAT_DARKER(*b); /* use a darker version of the background by 10% */
      *r = iupBYTECROP(ir);
      *g = iupBYTECROP(ig);
      *b = iupBYTECROP(ib);
    }
  }
}

char* iupMatrixGetFont(Ihandle* ih, int lin, int col)
{
  char* font = NULL;
  /* called from Draw and Edit only */
  if (ih->data->font_cb)
    font = ih->data->font_cb(ih, lin, col);
  if (!font)
    font = iMatrixGetCellAttrib(ih, IUPMAT_FONT, lin, col, 0);
  return font;
}

char *iupMatrixGetSize(Ihandle* ih, int index, int m, int pixels)
{
  char* str;
  int size;
  ImatLinColData *lincol_data;

  if(m == IMAT_PROCESS_LIN)
    lincol_data = &(ih->data->lines);
  else
    lincol_data = &(ih->data->columns);

  if (index < 0 || index > lincol_data->num-1)
    return NULL;

  if (m == IMAT_PROCESS_LIN)
    size = iupMatrixAuxGetLineHeight(ih, index);
  else
    size = iupMatrixAuxGetColumnWidth(ih, index);

  str = iupStrGetMemory(100);

  if (size)
  {
    /* remove the decoration added in iupMatrixAuxGet */
    if (m == IMAT_PROCESS_COL)
      size -= IMAT_PADDING_W + IMAT_FRAME_W;
    else
      size -= IMAT_PADDING_H + IMAT_FRAME_H;

    if (!pixels)
    {
      int charwidth, charheight;
      iupdrvFontGetCharSize(ih, &charwidth, &charheight);
      if (m == IMAT_PROCESS_COL)
        size = iupRASTER2WIDTH(size, charwidth);
      else
        size = iupRASTER2HEIGHT(size, charheight);
    }
  }

  sprintf(str, "%d", size);
  return str;
}
