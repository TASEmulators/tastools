/** \file
 * \brief iupmatrix. definitions.
 *
 * See Copyright Notice in "iup.h"
 */
 
#ifndef __IUPMAT_DEF_H 
#define __IUPMAT_DEF_H

#ifdef __cplusplus
extern "C" {
#endif


#define IMAT_PROCESS_COL 1  /* Process the columns */
#define IMAT_PROCESS_LIN 2  /* Process the lines */

/***************************************************************************/
/* Decoration size in pixels                                               */
/***************************************************************************/
#define IMAT_PADDING_W   6
#define IMAT_PADDING_H   6
#define IMAT_FRAME_W   2
#define IMAT_FRAME_H   2

/* Cell flags */
#define IUPMAT_FONT    1   /* Has FONTL:C attribute */
#define IUPMAT_FGCOLOR 2   /* Has FGCOLORL:C attribute */
#define IUPMAT_BGCOLOR 4   /* Has BGCOLORL:C attribute */
#define IUPMAT_MARK    8   /* Is marked */
#define IUPMAT_FRAMEHCOLOR 16   /* Has FRAMEHORIZCOLORL:C */
#define IUPMAT_FRAMEVCOLOR 32   /* Has FRAMEVERTCOLORL:C */

/***************************************************************************/
/* Structures stored in each matrix                                        */
/***************************************************************************/
typedef struct _ImatCell
{
  char *value;           /* Cell value                              */
  unsigned char flags;  
} ImatCell;


typedef struct _ImatLinColData
{
  int* sizes;         /* Width/height of the columns/lines  (allocated after map)   */
  unsigned char* flags;    /* Attribute flags for the columns/lines (allocated after map) */

  int num;         /* Number of columns/lines in the matrix, default/minimum=1, always includes the title */
  int num_alloc;   /* Number of columns/lines allocated, default=5 */

  int first;       /* First visible column/line */
  int last;        /* Last visible column/line  */

  /* used to configure the scrollbar */
  int total_size;     /* Sum of the widths/heights of the columns/lines, not including the title */
  int visible_size;   /* Width/height of the visible window, not including the title */

  int focus_cell;  /* index of the current cell */
} ImatLinColData;

struct _IcontrolData
{
  iupCanvas canvas;  /* from IupCanvas (must reserve it) */

  ImatCell** cells;  /* Cell value, this will be NULL if in callback mode (allocated after map) */

  Ihandle* texth;     /* Text handle                    */
  Ihandle* droph;     /* Dropdown handle                */
  Ihandle* datah;     /* Current active edition element, may be equal to texth or droph */

  cdCanvas* cddbuffer;
  cdCanvas* cdcanvas;

  ImatLinColData lines;
  ImatLinColData columns;

  int has_focus;
  int w, h;             /* canvas size */
  int callback_mode;
  int use_title_size;   /* use title contents when calculating cell size */
  int need_calcsize;
  int first_redraw;

  /* attributes */
  int mark_continuous, mark_mode, mark_multiple;
  int checkframecolor;

  /* Mouse and Keyboard AUX */
  int leftpressed;  /* left mouse button is pressed */
  int homekeycount, endkeycount;  /* numbers of times that key was pressed */

  /* ColRes AUX */
  int colres_dragging,   /* indicates if it is being made a column resize  */
      colres_drag_col,   /* column being resized */
      colres_drag_col_start_x, /* position of the start of the column being resized */
      colres_drag_col_last_x;  /* previous position */

  /* Mark AUX */
  int mark_lin1, mark_col1,  /* used to store the start cell when a block is being marked */
      mark_lin2, mark_col2,  /* used to store the end cell when a block was marked */
      mark_full1,            /* indicate if full lines or columns is being selected */
      mark_full2;

  /* Draw AUX, valid only after iupMatrixPrepareDrawData */
  sIFnii font_cb;
  IFniiIII fgcolor_cb;
  IFniiIII bgcolor_cb;
  char *bgcolor, *bgcolor_parent, *fgcolor, *font;  /* not need to free */
};


int iupMatrixIsValid(Ihandle* ih, int check_cells);

#define iupMatrixInvertYAxis(_ih, _y) ((_ih)->data->h-1 - (_y))


#ifdef __cplusplus
}
#endif

#endif
