/** \file
 * \brief Motif Base Functions
 *
 * See Copyright Notice in "iup.h"
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <Xm/Xm.h>
#include <Xm/ScrollBar.h>
#include <X11/cursorfont.h>

#include "iup.h"
#include "iupkey.h"
#include "iupcbs.h"

#include "iup_object.h"
#include "iup_childtree.h"
#include "iup_key.h"
#include "iup_str.h"
#include "iup_class.h"
#include "iup_attrib.h"
#include "iup_focus.h"
#include "iup_key.h"
#include "iup_drv.h"
#include "iup_image.h"
#include "iup_drv.h"

#include "iupmot_color.h"
#include "iupmot_drv.h"



void iupdrvActivate(Ihandle* ih)
{
  if (iupStrEqual(ih->iclass->name, "text") || iupStrEqual(ih->iclass->name, "multiline"))
    XmProcessTraversal(ih->handle, XmTRAVERSE_CURRENT);
  else
    XtCallActionProc(ih->handle, "ArmAndActivate", 0, 0, 0 );
}

static int motActivateMnemonic(Ihandle *dialog, int c)
{
  Ihandle *ih;
  char attrib[19] = "_IUPMOT_MNEMONIC_ ";
  attrib[17] = (char)c;
  ih = (Ihandle*)iupAttribGet(dialog, attrib);
  if (iupObjectCheck(ih))
  {
    iupdrvActivate(ih);
    return IUP_IGNORE;
  }
  return IUP_CONTINUE;
}

void iupmotSetMnemonicTitle(Ihandle *ih, const char* value)
{
  char c;
  char* str;

  if (!value) 
    value = "";

  str = iupStrProcessMnemonic(value, &c, -1);  /* remove & and return in c */
  if (str != value)
  {
    KeySym keysym = iupmotKeyCharToKeySym(c);
    XtVaSetValues(ih->handle, XmNmnemonic, keysym, NULL);   /* works only for menus, but underlines the letter */

    if (ih->iclass->nativetype != IUP_TYPEMENU)
    {
      Ihandle* dialog = IupGetDialog(ih);
      char attrib[22] = "_IUPMOT_MNEMONIC_ \0CB";
      attrib[17] = (char)toupper(c);

      /* used by motActivateMnemonic */
      if (iupStrEqual(ih->iclass->name, "label"))
        iupAttribSetStr(dialog, attrib, (char*)iupFocusNextInteractive(ih));
      else
        iupAttribSetStr(dialog, attrib, (char*)ih);

      /* used by iupmotKeyPressEvent */
      attrib[18] = '_';
      IupSetCallback(dialog, attrib, (Icallback)motActivateMnemonic);
    }

    iupmotSetString(ih->handle, XmNlabelString, str);
    free(str);
  }
  else
  {
    XtVaSetValues (ih->handle, XmNmnemonic, NULL, NULL);
    iupmotSetString(ih->handle, XmNlabelString, str);
  }
}

void iupmotSetString(Widget w, const char *resource, const char* value)
{
  XmString xm_str = XmStringCreateLocalized((String)value);
  XtVaSetValues(w, resource, xm_str, NULL);
  XmStringFree(xm_str);
}

char* iupmotConvertString(XmString str)
{
  char* text = (char*)XmStringUnparse(str, NULL, XmCHARSET_TEXT, XmCHARSET_TEXT, NULL, 0, XmOUTPUT_ALL);
  char* buf = iupStrGetMemoryCopy(text);
  XtFree(text);
  return buf;
}

#ifdef OLD_CONVERT
char* iupmotConvertString(XmString str)
{
  XmStringContext context;
  char *text, *p, *buf;
  unsigned int length;
  XmStringComponentType type;

  if (!XmStringInitContext (&context, str))
    return NULL;

  buf = iupStrGetMemory(XmStringLength(str));  /* always greatter than strlen */

  /* p keeps a running pointer through buf as text is read */
  p = buf;
  while ((type = XmStringGetNextTriple(context, &length, (XtPointer)&text)) != XmSTRING_COMPONENT_END) 
  {
    switch (type) 
    {
    case XmSTRING_COMPONENT_TEXT:
      memcpy(p, text, length);
      p += length;
      *p = 0;
      break;
    case XmSTRING_COMPONENT_TAB:
      *p++ = '\t';
      *p = 0;
      break;
    case XmSTRING_COMPONENT_SEPARATOR:
      *p++ = '\n';
      *p = 0;
      break;
    }
    XtFree(text);
  }

  XmStringFreeContext(context);

  return buf;
}
#endif

void iupdrvReparent(Ihandle* ih)
{
  Widget native_parent = iupChildTreeGetNativeParentHandle(ih);
  Widget widget = (Widget)iupAttribGet(ih, "_IUP_EXTRAPARENT");
  if (!widget) widget = ih->handle;
  XReparentWindow(iupmot_display, XtWindow(widget), XtWindow(native_parent), 0, 0);
}

void iupdrvBaseLayoutUpdateMethod(Ihandle *ih)
{
  Widget widget = (Widget)iupAttribGet(ih, "_IUP_EXTRAPARENT");
  if (!widget) widget = ih->handle;

  XtVaSetValues(widget,
    XmNx, (XtArgVal)ih->x,
    XmNy, (XtArgVal)ih->y,
    XmNwidth, (XtArgVal)ih->currentwidth,
    XmNheight, (XtArgVal)ih->currentheight,
    NULL);
}

void iupdrvBaseUnMapMethod(Ihandle* ih)
{
  Widget widget = (Widget)iupAttribGet(ih, "_IUP_EXTRAPARENT");
  if (!widget) widget = ih->handle;

  XtUnrealizeWidget(widget); /* To match the call to XtRealizeWidget */
  XtDestroyWidget(widget);   /* To match the call to XtCreateManagedWidget */
}

void iupdrvDisplayUpdate(Ihandle *ih)
{
  XExposeEvent evt;
  Dimension w, h;

  XtVaGetValues(ih->handle, XmNwidth, &w, 
                            XmNheight, &h, 
                            NULL);

  evt.type = Expose;
  evt.display = iupmot_display;
  evt.send_event = True;
  evt.window = XtWindow(ih->handle);

  evt.x = 0;
  evt.y = 0;
  evt.width = w;
  evt.height = h;

  evt.count = 0;

  /* POST a Redraw */
  XSendEvent(iupmot_display, XtWindow(ih->handle), False, ExposureMask, (XEvent*)&evt);
}

void iupdrvDisplayRedraw(Ihandle *ih)
{
  Widget w;

  /* POST a Redraw */
  iupdrvDisplayUpdate(ih);

  /* if this element has an inner native parent (like IupTabs), 
     then redraw that native parent if different from the element. */
  w = (Widget)iupClassObjectGetInnerNativeContainerHandle(ih, (Ihandle*)IupGetAttribute(ih, "VALUE_HANDLE"));
  if (w && w != ih->handle)
  {
    Widget handle = ih->handle;
    ih->handle = w;
    iupdrvDisplayUpdate(ih);
    ih->handle = handle;
  }

  /* flush exposure events. */
  XmUpdateDisplay(ih->handle);
}

void iupdrvScreenToClient(Ihandle* ih, int *x, int *y)
{
  Window child;
  XTranslateCoordinates(iupmot_display, RootWindow(iupmot_display, iupmot_screen),
                                        XtWindow(ih->handle),
                                        *x, *y, x, y, &child);
}

void iupmotHelpCallback(Widget w, Ihandle *ih, XtPointer call_data)
{
  Icallback cb = IupGetCallback(ih, "HELP_CB");
  if (cb && cb(ih) == IUP_CLOSE)
    IupExitLoop();

  (void)call_data;
  (void)w;
}

void iupmotEnterLeaveWindowEvent(Widget w, Ihandle *ih, XEvent *evt, Boolean *cont)
{
  Icallback cb = NULL;
  (void)cont;
  (void)w;

  if (evt->type == EnterNotify)
  {
    iupmotTipEnterNotify(ih);

    cb = IupGetCallback(ih, "ENTERWINDOW_CB");
  }
  else  if (evt->type == LeaveNotify)
  {
    iupmotTipLeaveNotify();

    cb = IupGetCallback(ih, "LEAVEWINDOW_CB");
  }

  if (cb)
    cb(ih);
}

int iupdrvBaseSetZorderAttrib(Ihandle* ih, const char* value)
{
  if (iupdrvIsVisible(ih))
  {
    if (iupStrEqualNoCase(value, "TOP"))
      XRaiseWindow(iupmot_display, XtWindow(ih->handle));
    else
      XLowerWindow(iupmot_display, XtWindow(ih->handle));
  }

  return 0;
}

void iupdrvSetVisible(Ihandle* ih, int visible)
{
  Widget widget = (Widget)iupAttribGet(ih, "_IUP_EXTRAPARENT");
  if (!widget) widget = ih->handle;

  if (visible)
    XtMapWidget(widget);
  else
    XtUnmapWidget(widget);
}

int iupdrvIsVisible(Ihandle* ih)
{
  XWindowAttributes wa;
  XGetWindowAttributes(iupmot_display, XtWindow(ih->handle), &wa);
  return (wa.map_state == IsViewable);
}

int iupdrvIsActive(Ihandle* ih)
{
  return XtIsSensitive(ih->handle);
}

void iupdrvSetActive(Ihandle* ih, int enable)
{
  Widget widget = (Widget)iupAttribGet(ih, "_IUP_EXTRAPARENT");
  if (!widget) widget = ih->handle;

  XtSetSensitive(widget, enable);
}

char* iupmotGetXWindowAttrib(Ihandle *ih)
{
  return (char*)XtWindow(ih->handle);
}

char* iupdrvBaseGetXAttrib(Ihandle *ih)
{
  int x, y;
  Window child;
  char* str = iupStrGetMemory(20);
  Widget widget = (Widget)iupAttribGet(ih, "_IUP_EXTRAPARENT");
  if (!widget) widget = ih->handle;

  /* Translating to absolute screen coordinates */
                     /* source            destination */
  XTranslateCoordinates(iupmot_display, 
                        XtWindow(widget), RootWindow(iupmot_display, iupmot_screen), 
                        0, 0,             &x, &y, 
                        &child);

  sprintf(str, "%d", x);
  return str;
}

char* iupdrvBaseGetYAttrib(Ihandle *ih)
{
  int x, y;
  Window child;
  char* str = iupStrGetMemory(20);
  Widget widget = (Widget)iupAttribGet(ih, "_IUP_EXTRAPARENT");
  if (!widget) widget = ih->handle;

  /* Translating to absolute screen coordinates */
                     /* source            destination */
  XTranslateCoordinates(iupmot_display, 
                        XtWindow(widget), RootWindow(iupmot_display, iupmot_screen), 
                        0, 0,             &x, &y, 
                        &child);

  sprintf(str, "%d", y);
  return str;
}

void iupmotSetBgColor(Widget w, Pixel color)
{
  Pixel fgcolor;
  XtVaGetValues(w, XmNforeground, &fgcolor, NULL);
  XmChangeColor(w, color);
  /* XmChangeColor also sets the XmNforeground color, so we must reset to the previous one. */
  XtVaSetValues(w, XmNforeground, fgcolor, NULL);
  XtVaSetValues(w, XmNbackgroundPixmap, XmUNSPECIFIED_PIXMAP, NULL);
}

int iupdrvBaseSetBgColorAttrib(Ihandle* ih, const char* value)
{
  Pixel color = iupmotColorGetPixelStr(value);
  if (color != (Pixel)-1)
  {
    iupmotSetBgColor(ih->handle, color);

    /* update internal image cache for controls that have the IMAGE attribute */
    iupImageUpdateParent(ih);
  }
  return 1;
}

char* iupmotGetBgColorAttrib(Ihandle* ih)
{
  unsigned char r, g, b;
  Pixel color;
  char* str = iupStrGetMemory(20);
  XtVaGetValues(ih->handle, XmNbackground, &color, NULL); 
  iupmotColorGetRGB(color, &r, &g, &b);
  sprintf(str, "%d %d %d", (int)r, (int)g, (int)b);
  return str;
}

int iupdrvBaseSetFgColorAttrib(Ihandle* ih, const char* value)
{
  Pixel color = iupmotColorGetPixelStr(value);
  if (color != (Pixel)-1)
    XtVaSetValues(ih->handle, XmNforeground, color, NULL);
  return 1;
}

void iupmotGetWindowSize(Ihandle *ih, int *width, int *height)
{
  Dimension w, h;
  XtVaGetValues(ih->handle, XmNwidth, &w, 
                            XmNheight, &h, 
                            NULL);
  *width = w;
  *height = h;
}

static Cursor motEmptyCursor(Ihandle* ih)
{
  /* creates an empty cursor */
  XColor cursor_color = {0L,0,0,0,0,0};
  char bitsnull[1] = {0x00};
  Pixmap pixmapnull;
  Cursor cur;

  pixmapnull = XCreateBitmapFromData(iupmot_display,
    XtWindow(ih->handle),
    bitsnull,
    1,1);

  cur = XCreatePixmapCursor(iupmot_display,
    pixmapnull,
    pixmapnull,
    &cursor_color,
    &cursor_color,
    0,0);

  XFreePixmap(iupmot_display, pixmapnull);

  return cur;
}

static Cursor motGetCursor(Ihandle* ih, const char* name)
{
  static struct {
    const char* iupname;
    int         sysname;
  } table[] = {
    { "NONE",      0}, 
    { "NULL",      0}, 
    { "ARROW",     XC_left_ptr},
    { "BUSY",      XC_watch},
    { "CROSS",     XC_crosshair},
    { "HAND",      XC_hand2},
    { "HELP",      XC_question_arrow},
    { "IUP",       XC_question_arrow},
    { "MOVE",      XC_fleur},
    { "PEN",       XC_pencil},
    { "RESIZE_N",  XC_top_side},
    { "RESIZE_S",  XC_bottom_side},
    { "RESIZE_NS", XC_sb_v_double_arrow},
    { "RESIZE_W",  XC_left_side},
    { "RESIZE_E",  XC_right_side},
    { "RESIZE_WE", XC_sb_h_double_arrow},
    { "RESIZE_NE", XC_top_right_corner},
    { "RESIZE_SE", XC_bottom_right_corner},
    { "RESIZE_NW", XC_top_left_corner},
    { "RESIZE_SW", XC_bottom_left_corner},
    { "TEXT",      XC_xterm}, 
    { "UPARROW",   XC_center_ptr} 
  };

  Cursor cur;
  char str[50];
  int i, count = sizeof(table)/sizeof(table[0]);

  /* check the cursor cache first (per control)*/
  sprintf(str, "_IUPMOT_CURSOR_%s", name);
  cur = (Cursor)iupAttribGet(ih, str);
  if (cur)
    return cur;

  /* check the pre-defined IUP names first */
  for (i = 0; i < count; i++)
  {
    if (iupStrEqualNoCase(name, table[i].iupname)) 
    {
      if (table[i].sysname)
        cur = XCreateFontCursor(iupmot_display, table[i].sysname);
      else
        cur = motEmptyCursor(ih);

      break;
    }
  }

  if (i == count)
  {
    /* check for a name defined cursor */
    cur = (Cursor)iupImageGetCursor(name);
  }

  iupAttribSetStr(ih, str, (char*)cur);
  return cur;
}

int iupdrvBaseSetCursorAttrib(Ihandle* ih, const char* value)
{
  Cursor cur = motGetCursor(ih, value);
  if (cur)
  {
    XDefineCursor(iupmot_display, XtWindow(ih->handle), cur);
    return 1;
  }
  return 0;
}

#include <Xm/XmP.h>
#include <Xm/DrawP.h>

void iupdrvDrawFocusRect(Ihandle* ih, void* _gc, int x, int y, int w, int h)
{
  Drawable wnd = (Drawable)IupGetAttribute(ih, "XWINDOW");  /* Use IupGetAttribute to consult the native implemetation */
  GC gc = (GC)_gc;
  XmeDrawHighlight(iupmot_display, wnd, gc, x, y, w, h, 1);
}

void iupdrvBaseRegisterCommonAttrib(Iclass* ic)
{
  iupClassRegisterAttribute(ic, "XMFONTLIST", iupmotGetFontListAttrib, NULL, NULL, NULL, IUPAF_NOT_MAPPED|IUPAF_NO_INHERIT|IUPAF_NO_STRING);
  iupClassRegisterAttribute(ic, "XFONTSTRUCT", iupmotGetFontStructAttrib, NULL, NULL, NULL, IUPAF_NOT_MAPPED|IUPAF_NO_INHERIT|IUPAF_NO_STRING);
  iupClassRegisterAttribute(ic, "XFONTID", iupmotGetFontIdAttrib, NULL, NULL, NULL, IUPAF_NOT_MAPPED|IUPAF_NO_INHERIT|IUPAF_NO_STRING);
}

static void motDoNothing(Widget w, XEvent*  evt, String* params, Cardinal* num_params)
{
  (void)w;
  (void)evt;
  (void)params;
  (void)num_params;
}

void iupmotDisableDragSource(Widget w)
{
  char dragTranslations[] = "#override <Btn2Down>: iupDoNothing()";
  static int do_nothing_rec = 0;
  if (!do_nothing_rec)
  {
    XtActionsRec rec = {"iupDoNothing", (XtActionProc)motDoNothing};
    XtAppAddActions(iupmot_appcontext, &rec, 1);
    do_nothing_rec = 1;
  }
  XtOverrideTranslations(w, XtParseTranslationTable(dragTranslations));
}

int iupdrvGetScrollbarSize(void)
{
  return 15;
}

void iupmotSetPixmap(Ihandle* ih, const char* name, const char* prop, int make_inactive)
{
  if (name)
  {
    Pixmap old_pixmap;
    Pixmap pixmap = (Pixmap)iupImageGetImage(name, ih, make_inactive);
    if (!pixmap) 
      pixmap = XmUNSPECIFIED_PIXMAP;
    XtVaGetValues(ih->handle, prop, &old_pixmap, NULL);
    if (pixmap != old_pixmap)
      XtVaSetValues(ih->handle, prop, pixmap, NULL);
    return;
  }

  /* if not defined */
  XtVaSetValues(ih->handle, prop, XmUNSPECIFIED_PIXMAP, NULL);
}

void iupmotButtonPressReleaseEvent(Widget w, Ihandle* ih, XEvent* evt, Boolean* cont)
{
  unsigned long elapsed;
  static Time last = 0;
  char status[IUPKEY_STATUS_SIZE] = IUPKEY_STATUS_INIT;
  IFniiiis cb;

  XButtonEvent *but_evt = (XButtonEvent*)evt;
  if (but_evt->button!=Button1 &&
      but_evt->button!=Button2 &&
      but_evt->button!=Button3 &&
      but_evt->button!=Button4 &&
      but_evt->button!=Button5) 
    return;

  cb = (IFniiiis) IupGetCallback(ih,"BUTTON_CB");
  if (cb)
  {
    int ret, doubleclick = 0;
    int b = IUP_BUTTON1+(but_evt->button-1);

    /* Double/Single Click */
    if (but_evt->type==ButtonPress)
    {
      elapsed = but_evt->time - last;
      last = but_evt->time;
      if ((int)elapsed <= XtGetMultiClickTime(iupmot_display))
        doubleclick = 1;
    }

    iupmotButtonKeySetStatus(but_evt->state, but_evt->button, status, doubleclick);

    ret = cb(ih, b, (but_evt->type==ButtonPress), but_evt->x, but_evt->y, status);
    if (ret==IUP_CLOSE)
      IupExitLoop();
    else if (ret==IUP_IGNORE)
      *cont=False;
  }         

  (void)w;
}

void iupmotPointerMotionEvent(Widget w, Ihandle *ih, XEvent *evt, Boolean *cont)
{
  IFniis cb = (IFniis)IupGetCallback(ih,"MOTION_CB");
  if (cb)
  {
    XMotionEvent *motion_evt = (XMotionEvent*)evt;
    char status[IUPKEY_STATUS_SIZE] = IUPKEY_STATUS_INIT;
    iupmotButtonKeySetStatus(motion_evt->state, 0, status, 0);
    cb(ih, motion_evt->x, motion_evt->y, status);
  }

  (void)w;
  (void)cont;
}
