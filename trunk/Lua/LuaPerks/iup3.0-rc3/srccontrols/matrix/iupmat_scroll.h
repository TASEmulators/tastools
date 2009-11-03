/** \file
 * \brief iupmatrix control
 * scrolling.
 *
 * See Copyright Notice in "iup.h"
 */
 
#ifndef __IUPMAT_SCROLL_H 
#define __IUPMAT_SCROLL_H

#ifdef __cplusplus
extern "C" {
#endif

int  iupMatrixScroll_CB(Ihandle* ih, int action, float x, float y);

void iupMatrixScrollToVisible(Ihandle* ih, int lin, int col);

typedef void (*iupMatrixScrollMoveF)(Ihandle* ih, int mode, float pos, int m);
void  iupMatrixScrollMoveCursor(iupMatrixScrollMoveF func, Ihandle* ih, int mode, float pos, int m);

/* Used only by the macros bellow */
void iupMatrixScrollHome       (Ihandle* ih, int, float, int);
void iupMatrixScrollEnd        (Ihandle* ih, int, float, int);
void iupMatrixScrollLeftUp     (Ihandle* ih, int, float, int);
void iupMatrixScrollRightDown  (Ihandle* ih, int, float, int);
void iupMatrixScrollPgLeftUp   (Ihandle* ih, int, float, int);
void iupMatrixScrollPgRightDown(Ihandle* ih, int, float, int);
void iupMatrixScrollPos        (Ihandle* ih, int, float, int);
void iupMatrixScrollCr         (Ihandle* ih, int, float, int);

/* Mode used to "walk" inside the matrix.
   It shows if the movement request was from the scrollbar or from a key.
   Possible values for the "mode" parameter of the iupMatrixScrollMoveCursor function.
 */
#define IMAT_SCROLLBAR    0
#define IMAT_SCROLLKEY    1

/* Macros to help during the call of iupMatrixScrollMoveCursor function */

/* used in the keyboard processing module */
#define iupMatrixScrollKeyHome(ih)    iupMatrixScrollMoveCursor(iupMatrixScrollHome       , ih, IMAT_SCROLLKEY, 0, 0)
#define iupMatrixScrollKeyEnd(ih)     iupMatrixScrollMoveCursor(iupMatrixScrollEnd        , ih, IMAT_SCROLLKEY, 0, 0)
#define iupMatrixScrollKeyPgUp(ih)    iupMatrixScrollMoveCursor(iupMatrixScrollPgLeftUp   , ih, IMAT_SCROLLKEY, 0, IMAT_PROCESS_LIN)
#define iupMatrixScrollKeyPgDown(ih)  iupMatrixScrollMoveCursor(iupMatrixScrollPgRightDown, ih, IMAT_SCROLLKEY, 0, IMAT_PROCESS_LIN)
#define iupMatrixScrollKeyDown(ih)    iupMatrixScrollMoveCursor(iupMatrixScrollRightDown  , ih, IMAT_SCROLLKEY, 0, IMAT_PROCESS_LIN)
#define iupMatrixScrollKeyRight(ih)   iupMatrixScrollMoveCursor(iupMatrixScrollRightDown  , ih, IMAT_SCROLLKEY, 0, IMAT_PROCESS_COL)
#define iupMatrixScrollKeyUp(ih)      iupMatrixScrollMoveCursor(iupMatrixScrollLeftUp     , ih, IMAT_SCROLLKEY, 0, IMAT_PROCESS_LIN)
#define iupMatrixScrollKeyLeft(ih)    iupMatrixScrollMoveCursor(iupMatrixScrollLeftUp     , ih, IMAT_SCROLLKEY, 0, IMAT_PROCESS_COL)
#define iupMatrixScrollKeyCr(ih)      iupMatrixScrollMoveCursor(iupMatrixScrollCr         , ih, IMAT_SCROLLKEY, 0, 0)

/* Used by the scrollbar callback only */
#define iupMatrixScrollUp(ih)         iupMatrixScrollMoveCursor(iupMatrixScrollLeftUp     , ih, IMAT_SCROLLBAR, 0, IMAT_PROCESS_LIN)
#define iupMatrixScrollLeft(ih)       iupMatrixScrollMoveCursor(iupMatrixScrollLeftUp     , ih, IMAT_SCROLLBAR, 0, IMAT_PROCESS_COL)
#define iupMatrixScrollDown(ih)       iupMatrixScrollMoveCursor(iupMatrixScrollRightDown  , ih, IMAT_SCROLLBAR, 0, IMAT_PROCESS_LIN)
#define iupMatrixScrollRight(ih)      iupMatrixScrollMoveCursor(iupMatrixScrollRightDown  , ih, IMAT_SCROLLBAR, 0, IMAT_PROCESS_COL)
#define iupMatrixScrollPgUp(ih)       iupMatrixScrollMoveCursor(iupMatrixScrollPgLeftUp   , ih, IMAT_SCROLLBAR, 0, IMAT_PROCESS_LIN)
#define iupMatrixScrollPgLeft(ih)     iupMatrixScrollMoveCursor(iupMatrixScrollPgLeftUp   , ih, IMAT_SCROLLBAR, 0, IMAT_PROCESS_COL)
#define iupMatrixScrollPgDown(ih)     iupMatrixScrollMoveCursor(iupMatrixScrollPgRightDown, ih, IMAT_SCROLLBAR, 0, IMAT_PROCESS_LIN)
#define iupMatrixScrollPgRight(ih)    iupMatrixScrollMoveCursor(iupMatrixScrollPgRightDown, ih, IMAT_SCROLLBAR, 0, IMAT_PROCESS_COL)
#define iupMatrixScrollPosVer(ih, y)  iupMatrixScrollMoveCursor(iupMatrixScrollPos        , ih, IMAT_SCROLLBAR, y, IMAT_PROCESS_LIN)
#define iupMatrixScrollPosHor(ih, x)  iupMatrixScrollMoveCursor(iupMatrixScrollPos        , ih, IMAT_SCROLLBAR, x, IMAT_PROCESS_COL)


#ifdef __cplusplus
}
#endif

#endif
