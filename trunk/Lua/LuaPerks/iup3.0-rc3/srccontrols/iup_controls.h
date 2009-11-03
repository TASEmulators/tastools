/** \file
 * \brief Additional Controls Class Initialization functions.
 *
 * See Copyright Notice in "iup.h"
 */
 
#ifndef __IUP_CONTROLS_H 
#define __IUP_CONTROLS_H


#ifdef __cplusplus
extern "C" {
#endif


Iclass* iupDialGetClass(void);
Iclass* iupCellsGetClass(void);
Iclass* iupColorbarGetClass(void);
Iclass* iupColorBrowserGetClass(void);
Iclass* iupMatrixGetClass(void);
Iclass* iupGaugeGetClass(void);
Iclass* iupTabsGetClass(void);
Iclass* iupColorBrowserDlgGetClass(void);

char *iupControlBaseGetParentBgColor (Ihandle* ih);
char *iupControlBaseGetBgColorAttrib(Ihandle* ih);


#ifdef __cplusplus
}
#endif

#endif
