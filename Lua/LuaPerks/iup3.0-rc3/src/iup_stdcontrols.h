/** \file
 * \brief Standard Controls Class Initialization functions.
 *
 * See Copyright Notice in "iup.h"
 */
 
#ifndef __IUP_STDCONTROLS_H 
#define __IUP_STDCONTROLS_H

#ifdef __cplusplus
extern "C" {
#endif


Iclass* iupDialogGetClass(void);
Iclass* iupMessageDlgGetClass(void);
Iclass* iupColorDlgGetClass(void);
Iclass* iupFontDlgGetClass(void);
Iclass* iupFileDlgGetClass(void);

Iclass* iupLabelGetClass(void);
Iclass* iupButtonGetClass(void);
Iclass* iupToggleGetClass(void);
Iclass* iupRadioGetClass(void);
Iclass* iupCanvasGetClass(void);
Iclass* iupFrameGetClass(void);
Iclass* iupProgressBarGetClass(void);
Iclass* iupTextGetClass(void);
Iclass* iupMultilineGetClass(void);
Iclass* iupValGetClass(void);
Iclass* iupTabsGetClass(void);
Iclass* iupSpinGetClass(void);
Iclass* iupSpinboxGetClass(void);
Iclass* iupListGetClass(void);
Iclass* iupTreeGetClass(void);

Iclass* iupMenuGetClass(void);
Iclass* iupItemGetClass(void);
Iclass* iupSeparatorGetClass(void);
Iclass* iupSubmenuGetClass(void);

Iclass* iupFillGetClass(void);
Iclass* iupHboxGetClass(void);
Iclass* iupVboxGetClass(void);
Iclass* iupZboxGetClass(void);
Iclass* iupCboxGetClass(void);
Iclass* iupSboxGetClass(void);
Iclass* iupNormalizerGetClass(void);

Iclass* iupTimerGetClass(void);
Iclass* iupImageGetClass(void);
Iclass* iupImageRGBGetClass(void);
Iclass* iupImageRGBAGetClass(void);
Iclass* iupUserGetClass(void);
Iclass* iupClipboardGetClass(void);

/*************************************************/

void iupdrvMessageDlgInitClass(Iclass* ic);
void iupdrvColorDlgInitClass(Iclass* ic);
void iupdrvFontDlgInitClass(Iclass* ic);
void iupdrvFileDlgInitClass(Iclass* ic);

/************************************************/

/* Common definition of the canvas class */
typedef struct _iupCanvas {
  int sb;    /* scrollbar configuration, valid only after map, use iupBaseGetScrollbar before map */
  float posx, posy;
} iupCanvas;

#ifdef __cplusplus
}
#endif

#endif
