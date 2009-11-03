/** \file
 * \brief GTK Base Functions
 *
 * See Copyright Notice in "iup.h"
 */

#include <stdio.h>              
#include <stdlib.h>
#include <string.h>             
#include <limits.h>             

#include <gtk/gtk.h>

#include "iup.h"
#include "iupcbs.h"
#include "iupkey.h"

#include "iup_object.h"
#include "iup_childtree.h"
#include "iup_key.h"
#include "iup_str.h"
#include "iup_class.h"
#include "iup_attrib.h"
#include "iup_focus.h"
#include "iup_key.h"
#include "iup_image.h"
#include "iup_drv.h"

#include "iupgtk_drv.h"


/* WARNING: in GTK there are many controls that are not native windows, 
   so "->window" will NOT return a native window exclusive of that control,
   in fact it can return a base native window shared by many controls.
   IupCanvas is a special case that uses an exclusive native window. */

/* GTK only has abssolute positioning using a GtkFixed container,
   so all elements returned by iupChildTreeGetNativeParentHandle should be a GtkFixed. 
   If not looks in the native parent. */
static GtkFixed* gtkGetFixedParent(Ihandle* ih)
{
  GtkWidget* widget = iupChildTreeGetNativeParentHandle(ih);
  while (widget && !GTK_IS_FIXED(widget))
    widget = gtk_widget_get_parent(widget);
  return (GtkFixed*)widget;
}

void iupgtkUpdateMnemonic(Ihandle* ih)
{
  GtkLabel* label = (GtkLabel*)iupAttribGet(ih, "_IUPGTK_LABELMNEMONIC");
  if (label) gtk_label_set_mnemonic_widget(label, ih->handle);
}

void iupdrvActivate(Ihandle* ih)
{
  gtk_widget_activate(ih->handle);
}

void iupdrvReparent(Ihandle* ih)
{
  GtkFixed* fixed = gtkGetFixedParent(ih);
  GtkWidget* widget = (GtkWidget*)iupAttribGet(ih, "_IUP_EXTRAPARENT");
  if (!widget) widget = ih->handle;
  gtk_widget_reparent(widget, (GtkWidget*)fixed);
}

void iupgtkBaseAddToParent(Ihandle* ih)
{
  GtkFixed* fixed = gtkGetFixedParent(ih);
  GtkWidget* widget = (GtkWidget*)iupAttribGet(ih, "_IUP_EXTRAPARENT");
  if (!widget) widget = ih->handle;

  gtk_fixed_put(fixed, widget, 0, 0);
}

void iupdrvBaseLayoutUpdateMethod(Ihandle *ih)
{
  GtkFixed* fixed = gtkGetFixedParent(ih);
  GtkWidget* widget = (GtkWidget*)iupAttribGet(ih, "_IUP_EXTRAPARENT");
  if (!widget) widget = ih->handle;

  gtk_fixed_move(fixed, widget, ih->x, ih->y);
  gtk_widget_set_size_request(widget, ih->currentwidth, ih->currentheight);
}

void iupdrvBaseUnMapMethod(Ihandle* ih)
{
  GtkWidget* widget = (GtkWidget*)iupAttribGet(ih, "_IUP_EXTRAPARENT");
  if (!widget) widget = ih->handle;
  gtk_widget_unrealize(widget);
  gtk_widget_destroy(widget);   /* To match the call to gtk_*****_new     */
}

void iupdrvDisplayUpdate(Ihandle *ih)
{
  /* Post a REDRAW */
  gtk_widget_queue_draw(ih->handle);
}

void iupdrvDisplayRedraw(Ihandle *ih)
{
  GdkWindow* window = ih->handle->window;
  /* Post a REDRAW */
  gtk_widget_queue_draw(ih->handle);
  /* Force a REDRAW */
  if (window)
    gdk_window_process_updates(window, FALSE);
}

void iupdrvScreenToClient(Ihandle* ih, int *x, int *y)
{
  gint win_x = 0, win_y = 0;
  GdkWindow* window = ih->handle->window;
  if (window)
    gdk_window_get_origin(window, &win_x, &win_y);
  *x = *x - win_x;
  *y = *y - win_y;
}

gboolean iupgtkShowHelp(GtkWidget *widget, GtkWidgetHelpType *arg1, Ihandle *ih)
{
  Icallback cb;
  (void)widget;
  (void)arg1;

  cb = IupGetCallback(ih, "HELP_CB");
  if (cb && cb(ih) == IUP_CLOSE) 
    IupExitLoop();

  return FALSE;
}

gboolean iupgtkEnterLeaveEvent(GtkWidget *widget, GdkEventCrossing *evt, Ihandle *ih)
{
  Icallback cb = NULL;
  (void)widget;

  if (evt->type == GDK_ENTER_NOTIFY)
    cb = IupGetCallback(ih, "ENTERWINDOW_CB");
  else  if (evt->type == GDK_LEAVE_NOTIFY)
    cb = IupGetCallback(ih, "LEAVEWINDOW_CB");

  if (cb) 
    cb(ih);

  return FALSE;
}

int iupgtkSetMnemonicTitle(Ihandle* ih, GtkLabel* label, const char* value)
{
  char c = '_';
  char* str;

  if (!value) 
    value = "";

  str = iupStrProcessMnemonic(value, &c, 1);  /* replace & by c, the returned value of c is ignored in GTK */
  if (str != value)
  {
    gtk_label_set_text_with_mnemonic(label, iupgtkStrConvertToUTF8(str));
    free(str);
    return 1;
  }
  else
  {
    if (iupAttribGetBoolean(ih, "MARKUP"))
      gtk_label_set_markup(label, iupgtkStrConvertToUTF8(str));
    else
      gtk_label_set_text(label, iupgtkStrConvertToUTF8(str));
  }
  return 0;
}

int iupdrvBaseSetZorderAttrib(Ihandle* ih, const char* value)
{
  if (iupdrvIsVisible(ih))
  {
    GdkWindow* window = ih->handle->window;
    if (iupStrEqualNoCase(value, "TOP"))
      gdk_window_raise(window);
    else
      gdk_window_lower(window);
  }

  return 0;
}

void iupdrvSetVisible(Ihandle* ih, int visible)
{
  GtkWidget* container = (GtkWidget*)iupAttribGet(ih, "_IUP_EXTRAPARENT");
  if (visible)
  {
    if (container) gtk_widget_show(container);
    gtk_widget_show(ih->handle);
  }
  else
  {
    if (container) gtk_widget_hide(container);
    gtk_widget_hide(ih->handle);
  }
}

int iupdrvIsVisible(Ihandle* ih)
{
  if (GTK_WIDGET_VISIBLE(ih->handle))
  {
    /* if marked as visible, since we use gtk_widget_hide and NOT gtk_widget_hide_all
       must check its parents. */
    Ihandle* parent = ih->parent;
    while (parent)
    {
      if (parent->iclass->nativetype != IUP_TYPEVOID)
      {
        if (!GTK_WIDGET_VISIBLE(parent->handle))
          return 0;
      }

      parent = parent->parent;
    }
    return 1;
  }
  else
    return 0;
}

int iupdrvIsActive(Ihandle *ih)
{
  return (GTK_WIDGET_IS_SENSITIVE(ih->handle));
}

void iupdrvSetActive(Ihandle* ih, int enable)
{
  GtkWidget* container = (GtkWidget*)iupAttribGet(ih, "_IUP_EXTRAPARENT");
  if (container) gtk_widget_set_sensitive(container, enable);
  gtk_widget_set_sensitive(ih->handle, enable);
}

char* iupdrvBaseGetXAttrib(Ihandle *ih)
{
  GdkWindow* window = ih->handle->window;
  GtkWidget* container = (GtkWidget*)iupAttribGet(ih, "_IUP_EXTRAPARENT");
  if (container) window = container->window;

  if (window)
  {
    char* str = iupStrGetMemory(20);
    int x, y;
    gdk_window_get_origin(window, &x, &y);
    x += ih->handle->allocation.x;
    sprintf(str, "%d", x);
    return str;
  }
  else
    return NULL;
}

char* iupdrvBaseGetYAttrib(Ihandle *ih)
{
  GdkWindow* window = ih->handle->window;
  GtkWidget* container = (GtkWidget*)iupAttribGet(ih, "_IUP_EXTRAPARENT");
  if (container) window = container->window;

  if (window)
  {
    char* str = iupStrGetMemory(20);
    int x, y;
    gdk_window_get_origin(window, &x, &y);
    y += ih->handle->allocation.y;
    sprintf(str, "%d", y);
    return str;
  }
  else
    return NULL;
}

char* iupdrvBaseGetClientSizeAttrib(Ihandle *ih)
{
  char* str = iupStrGetMemory(20);
  int w, h;
  GdkWindow* window = ih->handle->window;

  if (window)
    gdk_drawable_get_size(window, &w, &h);
  else
    return NULL;

  sprintf(str, "%dx%d", w, h);
  return str;
}

static GdkColor gtkDarkerColor(GdkColor *color)
{
  GdkColor dark_color = {0L,0,0,0};

  dark_color.red = (color->red*9)/10;
  dark_color.green = (color->green*9)/10;
  dark_color.blue = (color->blue*9)/10;

  return dark_color;
}

static guint16 gtkCROP16(int x)
{
  if (x > 65535) return 65535;
  return (guint16)x;
}

static GdkColor gtkLighterColor(GdkColor *color)
{
  GdkColor light_color = {0L,0,0,0};

  light_color.red = gtkCROP16(((int)color->red*11)/10);
  light_color.green = gtkCROP16(((int)color->green*11)/10);
  light_color.blue = gtkCROP16(((int)color->blue*11)/10);

  return light_color;
}

void iupgtkBaseSetBgColor(InativeHandle* handle, unsigned char r, unsigned char g, unsigned char b)
{
  GtkRcStyle *rc_style;  
  GdkColor color;

  iupgdkColorSet(&color, r, g, b);

  rc_style = gtk_widget_get_modifier_style(handle);
  rc_style->base[GTK_STATE_NORMAL] = rc_style->bg[GTK_STATE_NORMAL]   = rc_style->bg[GTK_STATE_INSENSITIVE] = color;
  rc_style->bg[GTK_STATE_ACTIVE]   = rc_style->base[GTK_STATE_ACTIVE] = gtkDarkerColor(&color);
  rc_style->base[GTK_STATE_PRELIGHT] = rc_style->bg[GTK_STATE_PRELIGHT] = rc_style->base[GTK_STATE_INSENSITIVE] = gtkLighterColor(&color);

  rc_style->color_flags[GTK_STATE_NORMAL] |= GTK_RC_BASE | GTK_RC_BG;
  rc_style->color_flags[GTK_STATE_ACTIVE] |= GTK_RC_BASE | GTK_RC_BG;
  rc_style->color_flags[GTK_STATE_PRELIGHT] |= GTK_RC_BASE | GTK_RC_BG;
  rc_style->color_flags[GTK_STATE_INSENSITIVE] |= GTK_RC_BASE | GTK_RC_BG;

  gtk_widget_modify_style(handle, rc_style);
}

int iupdrvBaseSetBgColorAttrib(Ihandle* ih, const char* value)
{
  unsigned char r, g, b;
  if (!iupStrToRGB(value, &r, &g, &b))
    return 0;

  iupgtkBaseSetBgColor(ih->handle, r, g, b);

  /* DO NOT NEED TO UPDATE GTK IMAGES SINCE THEY DO NOT DEPEND ON BGCOLOR */

  return 1;
}

void iupgtkBaseSetFgGdkColor(InativeHandle* handle, GdkColor *color)
{
  GtkRcStyle *rc_style;  

  rc_style = gtk_widget_get_modifier_style(handle);  
  rc_style->fg[GTK_STATE_ACTIVE] = rc_style->fg[GTK_STATE_NORMAL] = rc_style->fg[GTK_STATE_PRELIGHT] = *color;
  rc_style->text[GTK_STATE_ACTIVE] = rc_style->text[GTK_STATE_NORMAL] = rc_style->text[GTK_STATE_PRELIGHT] = *color;
  rc_style->color_flags[GTK_STATE_NORMAL] |= GTK_RC_TEXT | GTK_RC_FG;
  rc_style->color_flags[GTK_STATE_ACTIVE] |= GTK_RC_TEXT | GTK_RC_FG;
  rc_style->color_flags[GTK_STATE_PRELIGHT] |= GTK_RC_TEXT | GTK_RC_FG;

  /* do not set at CHILD_CONTAINER */
  gtk_widget_modify_style(handle, rc_style);
}

void iupgtkBaseSetFgColor(InativeHandle* handle, unsigned char r, unsigned char g, unsigned char b)
{
  GdkColor color;
  iupgdkColorSet(&color, r, g, b);
  iupgtkBaseSetFgGdkColor(handle, &color);
}

int iupdrvBaseSetFgColorAttrib(Ihandle* ih, const char* value)
{
  unsigned char r, g, b;
  if (!iupStrToRGB(value, &r, &g, &b))
    return 0;

  iupgtkBaseSetFgColor(ih->handle, r, g, b);

  return 1;
}

static GdkCursor* gtkEmptyCursor(Ihandle* ih)
{
  /* creates an empty cursor */
  GdkColor cursor_color = {0L,0,0,0};
  char bitsnull[1] = {0x00};

  GdkWindow* window = ih->handle->window;
  GdkPixmap* pixmapnull = gdk_bitmap_create_from_data(
    (GdkDrawable*)window,
    bitsnull,
    1,1);
  GdkCursor* cur = gdk_cursor_new_from_pixmap(
    pixmapnull,
    pixmapnull,
    &cursor_color,
    &cursor_color,
    0,0);

  g_object_unref(pixmapnull);

  return cur;
}

static GdkCursor* gtkGetCursor(Ihandle* ih, const char* name)
{
  static struct {
    const char* iupname;
    int         sysname;
  } table[] = {
    { "NONE",      0}, 
    { "NULL",      0}, 
    { "ARROW",     GDK_LEFT_PTR},
    { "BUSY",      GDK_WATCH},
    { "CROSS",     GDK_CROSSHAIR},
    { "HAND",      GDK_HAND2},
    { "HELP",      GDK_QUESTION_ARROW},
    { "IUP",       GDK_QUESTION_ARROW},
    { "MOVE",      GDK_FLEUR},
    { "PEN",       GDK_PENCIL},
    { "RESIZE_N",  GDK_TOP_SIDE},
    { "RESIZE_S",  GDK_BOTTOM_SIDE},
    { "RESIZE_NS", GDK_SB_V_DOUBLE_ARROW},
    { "RESIZE_W",  GDK_LEFT_SIDE},
    { "RESIZE_E",  GDK_RIGHT_SIDE},
    { "RESIZE_WE", GDK_SB_H_DOUBLE_ARROW},
    { "RESIZE_NE", GDK_TOP_RIGHT_CORNER},
    { "RESIZE_SE", GDK_BOTTOM_RIGHT_CORNER},
    { "RESIZE_NW", GDK_TOP_LEFT_CORNER},
    { "RESIZE_SW", GDK_BOTTOM_LEFT_CORNER},
    { "TEXT",      GDK_XTERM}, 
    { "UPARROW",   GDK_CENTER_PTR} 
  };

  GdkCursor* cur;
  char str[50];
  int i, count = sizeof(table)/sizeof(table[0]);

  /* check the cursor cache first (per control)*/
  sprintf(str, "_IUPGTK_CURSOR_%s", name);
  cur = (GdkCursor*)iupAttribGet(ih, str);
  if (cur)
    return cur;

  /* check the pre-defined IUP names first */
  for (i = 0; i < count; i++)
  {
    if (iupStrEqualNoCase(name, table[i].iupname)) 
    {
      if (table[i].sysname)
        cur = gdk_cursor_new(table[i].sysname);
      else
        cur = gtkEmptyCursor(ih);

      break;
    }
  }

  if (i == count)
  {
    /* check for a name defined cursor */
    cur = iupImageGetCursor(name);
  }

  /* save the cursor in cache */
  iupAttribSetStr(ih, str, (char*)cur);

  return cur;
}

int iupdrvBaseSetCursorAttrib(Ihandle* ih, const char* value)
{
  GdkCursor* cur = gtkGetCursor(ih, value);
  if (cur)
  {
    GdkWindow* window = ih->handle->window;
    if (window)
      gdk_window_set_cursor(window, cur);
    return 1;
  }
  return 0;
}

void iupgdkColorSet(GdkColor* color, unsigned char r, unsigned char g, unsigned char b)
{
  color->red = iupCOLOR8TO16(r);
  color->green = iupCOLOR8TO16(g);
  color->blue = iupCOLOR8TO16(b);
  color->pixel = 0;
}

static void gtkDragDataReceived(GtkWidget* w, GdkDragContext* context, int x, int y,
                                      GtkSelectionData* seldata, guint info, guint time, Ihandle* ih)
{
  gchar **uris = NULL;
  int i, count;

  IFnsiii cb = (IFnsiii)IupGetCallback(ih, "DROPFILES_CB");
  if (!cb) return; 

#if GTK_CHECK_VERSION(2, 6, 0)
  uris = g_uri_list_extract_uris((char*)seldata->data);
#endif

  if (!uris)
    return;

  count = 0;
  while (uris[count])
    count++;

  for (i=0; i<count; i++)
  {
    char* filename = uris[i];
    if (iupStrEqualPartial(filename, "file://"))
    {
      filename += strlen("file://");
      if (filename[2] == ':') /* in Windows there is an extra '/' at the begining. */
        filename++;
    }
    if (cb(ih, filename, count-i-1, x, y) == IUP_IGNORE)
      break;
  }

  g_strfreev (uris);
  (void)time;
  (void)info;
  (void)w;
  (void)context;
}

int iupgtkSetDragDropAttrib(Ihandle* ih, const char* value)
{
  if (iupStrBoolean(value))
  {
    GtkTargetEntry dragtypes[] = { { "text/uri-list", 0, 0 } };
    gtk_drag_dest_set(ih->handle, GTK_DEST_DEFAULT_ALL, dragtypes,
                      sizeof(dragtypes) / sizeof(dragtypes[0]), GDK_ACTION_COPY);
    g_signal_connect(G_OBJECT(ih->handle), "drag_data_received", G_CALLBACK(gtkDragDataReceived), ih);
  }
  else
  {
    gtk_drag_dest_unset(ih->handle);
  }
  return 1;
}

int iupdrvGetScrollbarSize(void)
{
  static int size = 0;

  if (size == 0)
  {
    GtkRequisition requisition;
    GtkWidget* win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    GtkWidget* sb = gtk_vscrollbar_new(NULL);
    gtk_container_add((GtkContainer*)win, sb);
    gtk_widget_realize(win);
    gtk_widget_size_request(sb, &requisition);
    size = requisition.width;
    gtk_widget_destroy(win);
  }

  return size;
}

void iupdrvDrawFocusRect(Ihandle* ih, void* _gc, int x, int y, int w, int h)
{
  GdkWindow* window = ih->handle->window;
  GtkStyle *style = gtk_widget_get_style(ih->handle);
  (void)_gc;

  gtk_paint_focus(style, window, GTK_WIDGET_STATE(ih->handle), NULL, ih->handle, NULL, x, y, w, h);
}

void iupdrvBaseRegisterCommonAttrib(Iclass* ic)
{
#ifdef WIN32                                 
  iupClassRegisterAttribute(ic, "HFONT", iupgtkGetFontIdAttrib, NULL, NULL, NULL, IUPAF_NOT_MAPPED|IUPAF_NO_INHERIT|IUPAF_NO_STRING);
#else
  iupClassRegisterAttribute(ic, "XFONTID", iupgtkGetFontIdAttrib, NULL, NULL, NULL, IUPAF_NOT_MAPPED|IUPAF_NO_INHERIT|IUPAF_NO_STRING);
#endif
  iupClassRegisterAttribute(ic, "PANGOFONTDESC", iupgtkGetPangoFontDescAttrib, NULL, NULL, NULL, IUPAF_NOT_MAPPED|IUPAF_NO_INHERIT|IUPAF_NO_STRING);
}

static int gtkStrIsAscii(const char* str)
{
  while(*str)
  {
    int c = *str;
    if (c < 0)
      return 0;
    str++;
  }
  return 1;
}

static char* gtkStrToUTF8(const char *str, const char* charset)
{
  return g_convert(str, -1, "UTF-8", charset, NULL, NULL, NULL);
}

static char* gtkStrFromUTF8(const char *str, const char* charset)
{
  return g_convert(str, -1, charset, "UTF-8", NULL, NULL, NULL);
}

static char* gktLastConvertUTF8 = NULL;

void iupgtkReleaseConvertUTF8(void)
{
  if (gktLastConvertUTF8)
    g_free(gktLastConvertUTF8);
}

char* iupgtkStrConvertToUTF8(const char* str)  /* From IUP to GTK */
{
  if (!str || *str == 0)
    return (char*)str;

  if (iupgtk_utf8autoconvert)  /* this means str is in current locale */
  {
    const char *charset = NULL;
    if (g_get_charset(&charset)==TRUE)  /* current locale is already UTF-8 */
    {
      if (g_utf8_validate(str, -1, NULL))
        return (char*)str;
      else
      {
        if (gktLastConvertUTF8)
          g_free(gktLastConvertUTF8);
        gktLastConvertUTF8 = gtkStrToUTF8(str, "ISO8859-1");   /* if string is not UTF-8, assume ISO8859-1 */
        if (!gktLastConvertUTF8) return (char*)str;
        return gktLastConvertUTF8;
      }
    }
    else
    {
      if (gtkStrIsAscii(str) || !charset)
        return (char*)str;
      else if (charset)
      {
        if (gktLastConvertUTF8)
          g_free(gktLastConvertUTF8);
        gktLastConvertUTF8 = gtkStrToUTF8(str, charset);
        if (!gktLastConvertUTF8) return (char*)str;
        return gktLastConvertUTF8;
      }
    }
  }
  return (char*)str;
}

char* iupgtkStrConvertFromUTF8(const char* str)  /* From GTK to IUP */
{
  if (!str || *str == 0)
    return (char*)str;

  if (iupgtk_utf8autoconvert)  /* this means str is in current locale */
  {
    const gchar *charset = NULL;
    if (g_get_charset(&charset)==TRUE)  /* current locale is already UTF-8 */
    {
      if (g_utf8_validate(str, -1, NULL))
        return (char*)str;
      else
      {
        if (gktLastConvertUTF8)
          g_free(gktLastConvertUTF8);
        gktLastConvertUTF8 = gtkStrFromUTF8(str, "ISO8859-1");  /* if string is not UTF-8, assume ISO8859-1 */
        if (!gktLastConvertUTF8) return (char*)str;
        return gktLastConvertUTF8;
      }
    }
    else
    {
      if (gtkStrIsAscii(str) || !charset)
        return (char*)str;
      else if (charset)
      {
        if (gktLastConvertUTF8)
          g_free(gktLastConvertUTF8);
        gktLastConvertUTF8 = gtkStrFromUTF8(str, charset);
        if (!gktLastConvertUTF8) return (char*)str;
        return gktLastConvertUTF8;
      }
    }
  }
  return (char*)str;
}

static gboolean gtkGetFilenameCharset(const gchar **filename_charset)
{
  const gchar **charsets = NULL;
  gboolean is_utf8 = FALSE;
  
#if GTK_CHECK_VERSION(2, 6, 0)
  is_utf8 = g_get_filename_charsets (&charsets);
#endif

  if (filename_charset && charsets)
    *filename_charset = charsets[0];
  
  return is_utf8;
}

char* iupgtkStrConvertToFilename(const char* str)   /* From IUP to Filename */
{
  if (!str || *str == 0)
    return (char*)str;

  if (iupgtk_utf8autoconvert)  /* this means str is in current locale */
    return (char*)str;
  else
  {
    const gchar *charset = NULL;
    if (gtkGetFilenameCharset(&charset)==TRUE)  /* current locale is already UTF-8 */
    {
      if (g_utf8_validate(str, -1, NULL))
        return (char*)str;
      else
      {
        if (gktLastConvertUTF8)
          g_free(gktLastConvertUTF8);
        gktLastConvertUTF8 = gtkStrFromUTF8(str, "ISO8859-1");  /* if string is not UTF-8, assume ISO8859-1 */
        if (!gktLastConvertUTF8) return (char*)str;
        return gktLastConvertUTF8;
      }
    }
    else
    {
      if (gtkStrIsAscii(str) || !charset)
        return (char*)str;
      else if (charset)
      {
        if (gktLastConvertUTF8)
          g_free(gktLastConvertUTF8);
        gktLastConvertUTF8 = gtkStrFromUTF8(str, charset);
        if (!gktLastConvertUTF8) return (char*)str;
        return gktLastConvertUTF8;
      }
    }
  }
  return (char*)str;
}

char* iupgtkStrConvertFromFilename(const char* str)   /* From Filename to IUP */
{
  if (!str || *str == 0)
    return (char*)str;

  if (iupgtk_utf8autoconvert)  /* this means str is in current locale */
    return (char*)str;
  else
  {
    const char *charset = NULL;
    if (gtkGetFilenameCharset(&charset)==TRUE)  /* current locale is already UTF-8 */
    {
      if (g_utf8_validate(str, -1, NULL))
        return (char*)str;
      else
      {
        if (gktLastConvertUTF8)
          g_free(gktLastConvertUTF8);
        gktLastConvertUTF8 = gtkStrToUTF8(str, "ISO8859-1");   /* if string is not UTF-8, assume ISO8859-1 */
        if (!gktLastConvertUTF8) return (char*)str;
        return gktLastConvertUTF8;
      }
    }
    else
    {
      if (gtkStrIsAscii(str) || !charset)
        return (char*)str;
      else if (charset)
      {
        if (gktLastConvertUTF8)
          g_free(gktLastConvertUTF8);
        gktLastConvertUTF8 = gtkStrToUTF8(str, charset);
        if (!gktLastConvertUTF8) return (char*)str;
        return gktLastConvertUTF8;
      }
    }
  }
  return (char*)str;
}

gboolean iupgtkMotionNotifyEvent(GtkWidget *widget, GdkEventMotion *evt, Ihandle *ih)
{
  IFniis cb = (IFniis)IupGetCallback(ih,"MOTION_CB");
  if (cb)
  {
    char status[IUPKEY_STATUS_SIZE] = IUPKEY_STATUS_INIT;
    iupgtkButtonKeySetStatus(evt->state, 0, status, 0);
    cb(ih, (int)evt->x, (int)evt->y, status);
  }

  (void)widget;
  return FALSE;
}

gboolean iupgtkButtonEvent(GtkWidget *widget, GdkEventButton *evt, Ihandle *ih)
{
  IFniiiis cb = (IFniiiis)IupGetCallback(ih,"BUTTON_CB");
  if (cb)
  {
    int doubleclick = 0, ret, press = 1;
    int b = IUP_BUTTON1+(evt->button-1);
    char status[IUPKEY_STATUS_SIZE] = IUPKEY_STATUS_INIT;

    if (evt->type == GDK_BUTTON_RELEASE)
      press = 0;

    if (evt->type == GDK_2BUTTON_PRESS)
      doubleclick = 1;

    iupgtkButtonKeySetStatus(evt->state, evt->button, status, doubleclick);

    ret = cb(ih, b, press, (int)evt->x, (int)evt->y, status);
    if (ret==IUP_CLOSE)
      IupExitLoop();
    else if (ret==IUP_IGNORE)
      return TRUE;
  }

  (void)widget;
  return FALSE;
}
