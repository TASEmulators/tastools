/** \file
 * \brief Windows Driver TIPS management
 *
 * See Copyright Notice in "iup.h"
 */

#include <stdio.h>

#include <windows.h>
#include <commctrl.h>

#include "iup.h" 

#include "iup_object.h" 
#include "iup_attrib.h" 
#include "iup_str.h" 

#include "iupwin_drv.h"
#include "iupwin_handle.h"


#ifndef TTM_POPUP   /* it is defined only when _WIN32_WINNT >= 0x0501 */
#define TTM_POPUP  (WM_USER + 34)
#endif

static HWND winTipsCreate(HWND hParent)
{
  RECT rect = {1,1,1,1};
  HWND tips_hwnd = CreateWindowEx(WS_EX_TOPMOST, TOOLTIPS_CLASS, (LPSTR) NULL, TTS_ALWAYSTIP, 
                                  CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 
                                  hParent, (HMENU)NULL, iupwin_hinstance, NULL); 
  SendMessage(tips_hwnd, TTM_SETMAXTIPWIDTH, 0, (LPARAM)(INT)3000); 
  SendMessage(tips_hwnd, TTM_SETMARGIN, (WPARAM)0, (LPARAM)&rect);
  return tips_hwnd;
}

static int winTipsSendMessage(Ihandle* ih, HWND tips_hwnd, UINT msg)
{
  TOOLINFO ti;
  ZeroMemory(&ti, sizeof(TOOLINFO));
  ti.cbSize = sizeof(TOOLINFO); 
  ti.uFlags = TTF_SUBCLASS;
  ti.hinst = iupwin_hinstance; 
  ti.uId = 0; 
  ti.hwnd = ih->handle; 
  ti.lpszText = LPSTR_TEXTCALLBACK;
  ti.rect.right = 3000;
  ti.rect.bottom = 3000;

  return SendMessage(tips_hwnd, msg, 0, (LPARAM)&ti);
}

int iupdrvBaseSetTipAttrib(Ihandle* ih, const char* value)
{
  HWND tips_hwnd = (HWND)iupAttribGet(ih, "_IUPWIN_TIPSWIN");
  if (!tips_hwnd)
  {
    tips_hwnd = winTipsCreate(ih->handle);
    iupAttribSetStr(ih, "_IUPWIN_TIPSWIN", (char*)tips_hwnd);
    iupwinHandleAdd(ih, tips_hwnd);
  }

  if (value)
    winTipsSendMessage(ih, tips_hwnd, TTM_ADDTOOL);
  else
    winTipsSendMessage(ih, tips_hwnd, TTM_DELTOOL);

  return 1;
}

int iupdrvBaseSetTipVisibleAttrib(Ihandle* ih, const char* value)
{
  HWND tips_hwnd = (HWND)iupAttribGet(ih, "_IUPWIN_TIPSWIN");
  if (!tips_hwnd)
    return 0;

  /* must use IupGetAttribute to use inheritance */
  if (!IupGetAttribute(ih, "TIP"))
    return 0;

  if (iupStrBoolean(value))
    SendMessage(tips_hwnd, TTM_POPUP, 0, 0);  /* XP Only */
  else
    SendMessage(tips_hwnd, TTM_POP, 0, 0);

  return 0;
}

void iupwinTipsGetDispInfo(LPARAM lp)
{
  COLORREF color, tip_color;
  NMTTDISPINFO* tips_info;
  Ihandle* ih;
  HWND tips_hwnd;
  char* value;

  if (!lp) return;

  tips_info = (NMTTDISPINFO*)lp;
  ih = iupwinHandleGet(tips_info->hdr.hwndFrom);  /* hwndFrom is the tooltip window */
  if (!ih) return;

  tips_hwnd = (HWND)iupAttribGet(ih, "_IUPWIN_TIPSWIN");
  if (tips_hwnd != tips_info->hdr.hwndFrom) return;

  tips_info->hinst = NULL;
  tips_info->lpszText = IupGetAttribute(ih, "TIP");  /* must use IupGetAttribute to use inheritance */

  {
    HFONT hfont;
    value = iupAttribGetStr(ih, "TIPFONT");
    if (value)
    {
      if (iupStrEqualNoCase(value, "SYSTEM"))
        hfont = NULL;
      else
        hfont = iupwinGetHFont(value);
    }
    else
      hfont = (HFONT)iupwinGetHFontAttrib(ih);

    if (hfont)
    {
      HFONT tip_hfont = (HFONT)SendMessage(tips_hwnd, WM_GETFONT, 0, 0);
      if (tip_hfont != hfont)
        SendMessage(tips_hwnd, WM_SETFONT, (WPARAM)hfont, MAKELPARAM(TRUE,0));
    }
  }

  iupwinGetColorRef(ih, "TIPBGCOLOR", &color);
  tip_color = (COLORREF)SendMessage(tips_hwnd, TTM_GETTIPBKCOLOR, 0, 0);
  if (color != tip_color)
    SendMessage(tips_hwnd, TTM_SETTIPBKCOLOR, (WPARAM)color, 0);

  iupwinGetColorRef(ih, "TIPFGCOLOR", &color);
  tip_color = (COLORREF)SendMessage(tips_hwnd, TTM_GETTIPTEXTCOLOR, 0, 0);
  if (color != tip_color)
    SendMessage(tips_hwnd, TTM_SETTIPTEXTCOLOR, (WPARAM)color, 0);

  {
    int ballon = IupGetInt(ih, "TIPBALLON");  /* must use IupGetInt to use inheritance */
    DWORD style = GetWindowLong(tips_hwnd, GWL_STYLE);
    int tip_ballon = style & TTS_BALLOON? 1: 0; 
    if (tip_ballon != ballon)
    {
      if (ballon)
        style |= TTS_BALLOON;
      else
        style &= ~TTS_BALLOON;
      SetWindowLong(tips_hwnd, GWL_STYLE, style);
    }

    if (ballon)
    {
      char* ballon_title = IupGetAttribute(ih, "TIPBALLONTITLE"); /* must use IupGetAttribute to use inheritance */
      int ballon_icon = IupGetInt(ih, "TIPBALLONTITLEICON");  /* must use IupGetInt to use inheritance */
      SendMessage(tips_hwnd, TTM_SETTITLEA, ballon_icon, (LPARAM)ballon_title);
    }
    else
      SendMessage(tips_hwnd, TTM_SETTITLEA, 0, 0);
  }

  {
    int delay = IupGetInt(ih, "TIPDELAY"); /* must use IupGetInt to use inheritance */
    int tip_delay = SendMessage(tips_hwnd, TTM_GETDELAYTIME, TTDT_AUTOPOP, 0);
    if (delay != tip_delay)
      SendMessage(tips_hwnd, TTM_SETDELAYTIME, TTDT_AUTOPOP, (LPARAM)MAKELONG(delay, 0));
  }

  {
    TOOLINFO ti;

    ZeroMemory(&ti, sizeof(TOOLINFO));
    ti.cbSize = sizeof(TOOLINFO); 
    ti.uId = 0; 
    ti.hwnd = ih->handle;

    value = iupAttribGet(ih, "TIPRECT");
    if (value)
    {
      int x1, x2, y1, y2;
      sscanf(value, "%d %d %d %d", &x1, &y1, &x2, &y2);
      ti.rect.left = x1; ti.rect.right = x2;
      ti.rect.top = y1; ti.rect.bottom = y2;
    }
    else
      GetClientRect(ih->handle, &ti.rect);

    SendMessage(tips_hwnd, TTM_NEWTOOLRECT, 0, (LPARAM)&ti);
  }
}
