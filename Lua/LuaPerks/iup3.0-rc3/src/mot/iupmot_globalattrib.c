/** \file
 * \brief Motif Driver implementation of iupdrvSetGlobal
 *
 * See Copyright Notice in "iup.h"
 */

#include <Xm/Xm.h>

#include <stdio.h>

#include "iup.h"

#include "iup_str.h"
#include "iup_drv.h"
#include "iup_drvinfo.h"
#include "iup_strmessage.h"

#include "iupmot_drv.h"


static void motGlobalSendKey(int key, int press)
{
  Window focus;
	int revert_to;
  XKeyEvent evt;
  memset(&evt, 0, sizeof(XKeyEvent));
  evt.display = iupmot_display;
  evt.send_event = True;
  evt.root = DefaultRootWindow(iupmot_display);

	XGetInputFocus(iupmot_display, &focus, &revert_to);
  evt.window = focus;

  iupmotKeyEncode(key, &evt.keycode, &evt.state);
  if (!evt.keycode)
    return;

  if (press & 0x01)
  {
    evt.type = KeyPress;
    XSendEvent(iupmot_display, (Window)InputFocus, False, KeyPressMask, (XEvent*)&evt);
  }

  if (press & 0x02)
  {
    evt.type = KeyRelease;
    XSendEvent(iupmot_display, (Window)InputFocus, False, KeyReleaseMask, (XEvent*)&evt);
  }
}

int iupdrvSetGlobal(const char *name, const char *value)
{
  if (iupStrEqual(name, "LANGUAGE"))
  {
    iupStrMessageUpdateLanguage(value);
    return 1;
  }
  if (iupStrEqual(name, "AUTOREPEAT"))
  {
    XKeyboardControl values;
    if (iupStrBoolean(value))
      values.auto_repeat_mode = 1;
    else
      values.auto_repeat_mode = 0;
    XChangeKeyboardControl(iupmot_display, KBAutoRepeatMode, &values);
    return 0;
  }
  if (iupStrEqual(name, "CURSORPOS"))
  {
    int x, y;
    if (iupStrToIntInt(value, &x, &y, 'x') == 2)
      XWarpPointer(iupmot_display,None,RootWindow(iupmot_display, iupmot_screen),0,0,0,0,x,y);
    return 0;
  }
  if (iupStrEqual(name, "KEYPRESS"))
  {
    int key;
    if (iupStrToInt(value, &key))
      motGlobalSendKey(key, 0x01);
    return 0;
  }
  if (iupStrEqual(name, "KEYRELEASE"))
  {
    int key;
    if (iupStrToInt(value, &key))
      motGlobalSendKey(key, 0x02);
    return 0;
  }
  if (iupStrEqual(name, "KEY"))
  {
    int key;
    if (iupStrToInt(value, &key))
      motGlobalSendKey(key, 0x03);
    return 0;
  }
  return 1;
}

char* iupdrvGetGlobal(const char *name)
{
  if (iupStrEqual(name, "CURSORPOS"))
  {
    int x, y;
    char* str = iupStrGetMemory(50);
    iupdrvGetCursorPos(&x, &y);
    sprintf(str, "%dx%d", x, y);
    return str;
  }
  if (iupStrEqual(name, "SHIFTKEY"))
  {
    char key[5];
    iupdrvGetKeyState(key);
    if (key[0] == 'S')
      return "ON";
    return "OFF";
  }
  if (iupStrEqual(name, "CONTROLKEY"))
  {
    char key[5];
    iupdrvGetKeyState(key);
    if (key[1] == 'C')
      return "ON";
    return "OFF";
  }
  if (iupStrEqual(name, "MODKEYSTATE"))
  {
    char *str = iupStrGetMemory(5);
    iupdrvGetKeyState(str);
    return str;
  }
  if (iupStrEqual(name, "SCREENSIZE"))
  {
    char *str = iupStrGetMemory(50);
    int w, h;
    iupdrvGetScreenSize(&w, &h);
    sprintf(str, "%dx%d", w, h);
    return str;
  }
  if (iupStrEqual(name, "FULLSIZE"))
  {
    char *str = iupStrGetMemory(50);
    int w, h;
    iupdrvGetFullSize(&w, &h);
    sprintf(str, "%dx%d", w, h);
    return str;
  }
  if (iupStrEqual(name, "SCREENDEPTH"))
  {
    char *str = iupStrGetMemory(50);
    int bpp = iupdrvGetScreenDepth();
    sprintf(str, "%d", bpp);
    return str;
  }
  return NULL;
}
