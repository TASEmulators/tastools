/** \file
 * \brief Driver Interface. Each driver must export the symbols defined here.
 *
 * See Copyright Notice in "iup.h"
 */
 
#ifndef __IUP_DRV_H 
#define __IUP_DRV_H

#ifdef __cplusplus
extern "C" {
#endif

  
/** \defgroup drv Driver Interface 
 * \par
 * Each driver must export the symbols defined here.
 * \par
 * See \ref iup_drv.h 
 */


/** Sets a global environment attribute. Called from IupSetGlobal and IupStoreGlobal.
 * Must return 1 is process the atribute, or 0 is not. 
 * \ingroup drv */
int iupdrvSetGlobal(const char* name, const char* value);

/** Returns a global environment attribute. Called from IupGetGlobal.
 * \ingroup drv */
char* iupdrvGetGlobal(const char* name);

/** Changes the idle callback. Called from IupSetFunction.
 * \ingroup drv */
void iupdrvSetIdleFunction(Icallback func);

/** Convert the coordinates from screen relative to client area releative.
 * \ingroup drv */
void iupdrvScreenToClient(Ihandle* ih, int *x, int *y);

/** Returns true if the element is visible.
 * \ingroup drv */
int iupdrvIsVisible(Ihandle* ih);

/** Returns true if the element is active.
 * \ingroup drv */
int iupdrvIsActive(Ihandle* ih);

/** Actually changes the focus to the given element.
 * \ingroup drv */
void iupdrvSetFocus(Ihandle* ih);

/** Changes the visible state of an element. 
 * Not used for dialogs. 
 * \ingroup drv */
void iupdrvSetVisible(Ihandle* ih, int enable);

/** Changes the active state of an element. 
 * \ingroup drv */
void iupdrvSetActive(Ihandle* ih, int enable);

/** Post a redraw of a control.
 * \ingroup drv */
void iupdrvDisplayUpdate(Ihandle *ih);

/** Force a redraw of a control.
 * \ingroup drv */
void iupdrvDisplayRedraw(Ihandle *ih);

/** Reparent the native control.
 * \ingroup drv */
void iupdrvReparent(Ihandle* ih);

/** Draws a focus rectangle
 * \ingroup drv */
void iupdrvDrawFocusRect(Ihandle* ih, void* gc, int x, int y, int w, int h);

/** Size of the scroolbar.
 * \ingroup drv */
int iupdrvGetScrollbarSize(void);

/** Activates a control.
 * \ingroup drv */
void iupdrvActivate(Ihandle* ih);

/** Returns the height of a menu bar.
 * \ingroup drv */
int iupdrvMenuGetMenuBarSize(Ihandle* ih);


/* Called only from IupOpen/IupClose. */
int iupdrvOpen(int *argc, char ***argv);
void iupdrvClose(void);


#ifdef __cplusplus
}
#endif

#endif
