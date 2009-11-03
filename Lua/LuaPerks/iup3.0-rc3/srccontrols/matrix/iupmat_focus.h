/** \file
 * \brief iupmatrix focus control.
 *
 * See Copyright Notice in "iup.h"
 */
 
#ifndef __IUPMAT_FOCUS_H 
#define __IUPMAT_FOCUS_H

#ifdef __cplusplus
extern "C" {
#endif

int  iupMatrixFocus_CB(Ihandle* ih, int focus);
void iupMatrixFocusSet(Ihandle* ih, int lin, int col);

#ifdef __cplusplus
}
#endif

#endif
