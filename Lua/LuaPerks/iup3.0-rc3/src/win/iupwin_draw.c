/** \file
 * \brief Draw Functions
 *
 * See Copyright Notice in "iup.h"
 */

#undef _WIN32_WINNT
#define _WIN32_WINNT 0x0501
#include <windows.h>
#include <uxtheme.h>
#include <tmschema.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <memory.h>

#include "iup.h"

#include "iup_attrib.h"
#include "iup_class.h"
#include "iup_str.h"

#include "iupwin_drv.h"
#include "iupwin_info.h"
#include "iupwin_draw.h"


#ifndef TABP_AEROWIZARDBODY
#define TABP_AEROWIZARDBODY  11  /* manually added definition */
#endif

#ifndef TMT_FILLCOLORHINT
#define TMT_FILLCOLORHINT 3821
#endif
#ifndef TMT_TEXTCOLOR
#define TMT_TEXTCOLOR 3823
#endif


typedef HTHEME  (STDAPICALLTYPE *_winThemeOpenData)(HWND hwnd, LPCWSTR pszClassList);
typedef HRESULT (STDAPICALLTYPE *_winThemeCloseData)(HTHEME hTheme);
typedef HRESULT (STDAPICALLTYPE *_winThemeDrawBackground)(HTHEME hTheme, HDC hDC, int iPartId, int iStateId, const RECT *pRect, const RECT *pClipRect);
typedef HRESULT (STDAPICALLTYPE *_winThemeGetColor)(HTHEME hTheme, int iPartId, int iStateId, int iPropId, COLORREF *pColor);

static _winThemeOpenData winThemeOpenData = NULL;
static _winThemeCloseData winThemeCloseData = NULL;
static _winThemeDrawBackground winThemeDrawBackground = NULL;
static _winThemeGetColor winThemeGetColor = NULL;

typedef BOOL (CALLBACK* _winAlphaBlendFunc)( HDC hdcDest, 
                             int xoriginDest, int yoriginDest, 
                             int wDest, int hDest, HDC hdcSrc, 
                             int xoriginSrc, int yoriginSrc, 
                             int wSrc, int hSrc, 
                             BLENDFUNCTION ftn);
static _winAlphaBlendFunc winAlphaBlend = NULL;

static int winDrawThemeEnabled(void)
{
  return winThemeOpenData? 1: 0;
}

void iupwinDrawText(HDC hDC, const char* text, int x, int y, int width, int height, HFONT hFont, COLORREF fgcolor, int style)
{
  COLORREF oldcolor;
  RECT rect;
  HFONT hOldFont = SelectObject(hDC, hFont);

  rect.left = x;
  rect.top = y;
  rect.right = x+width;
  rect.bottom = y+height;

  SetTextAlign(hDC, TA_TOP|TA_LEFT);
  SetBkMode(hDC, TRANSPARENT);
  oldcolor = SetTextColor(hDC, fgcolor);

  DrawText(hDC, text, -1, &rect, style|DT_NOCLIP);

  SelectObject(hDC, hOldFont);
  SetTextColor(hDC, oldcolor);
  SetBkMode(hDC, OPAQUE);
}

void iupwinDrawBitmap(HDC hDC, HBITMAP hBitmap, HBITMAP hMask, int x, int y, int width, int height, int bpp)
{
  HDC hMemDC = CreateCompatibleDC(hDC);
  SelectObject(hMemDC, hBitmap);

  if (bpp == 32 && winAlphaBlend)
  {
    BLENDFUNCTION blendfunc;
    blendfunc.BlendOp = AC_SRC_OVER;
    blendfunc.BlendFlags = 0;
    blendfunc.SourceConstantAlpha = 0xFF;
    blendfunc.AlphaFormat = AC_SRC_ALPHA;

    winAlphaBlend(hDC, x, y, width, height, 
                  hMemDC, 0, 0, width, height, 
                  blendfunc);
  }
  else if (bpp == 8 && hMask)
    MaskBlt(hDC, x, y, width, height, 
            hMemDC, 0, 0, 
            hMask, 0, 0, MAKEROP4(SRCCOPY, 0xAA0000));
  else
    BitBlt(hDC, x, y, width, height, 
           hMemDC, 0, 0, 
           SRCCOPY);


  DeleteDC(hMemDC);
}

void iupwinDrawInit(void)
{
  if (!winAlphaBlend)
  {
    HINSTANCE lib = LoadLibrary("Msimg32");
    if (lib)
      winAlphaBlend = (_winAlphaBlendFunc)GetProcAddress(lib, "AlphaBlend");
  }

  if (!winThemeOpenData && iupwin_comctl32ver6)
  {
    HMODULE hinstDll = LoadLibrary("uxtheme.dll");
    if (hinstDll)
    {
      winThemeOpenData = (_winThemeOpenData)GetProcAddress(hinstDll, "OpenThemeData");
      winThemeCloseData = (_winThemeCloseData)GetProcAddress(hinstDll, "CloseThemeData");
      winThemeDrawBackground = (_winThemeDrawBackground)GetProcAddress(hinstDll, "DrawThemeBackground");
      winThemeGetColor = (_winThemeGetColor)GetProcAddress(hinstDll, "GetThemeColor");
    }
  }
}

static int winDrawGetThemeStateId(int itemState)
{
  if (itemState & ODS_DISABLED)
    return PBS_DISABLED; 
  else if (itemState & ODS_SELECTED)
    return PBS_PRESSED;  
  else if (itemState & ODS_HOTLIGHT)
    return PBS_HOT;      
  else if (itemState & ODS_DEFAULT)
    return PBS_DEFAULTED;
  else
    return PBS_NORMAL;       
}

static int winDrawThemeButtonBorder(HWND hWnd, HDC hDC, RECT *rect, UINT itemState)
{
  int iStateId;
  HTHEME hTheme;

  if (!winDrawThemeEnabled()) 
    return 0; 

  hTheme = winThemeOpenData(hWnd, L"BUTTON");
  if (!hTheme) 
    return 0;

  iStateId = winDrawGetThemeStateId(itemState);

  winThemeDrawBackground(hTheme, hDC, BP_PUSHBUTTON, iStateId, rect, NULL);

  winThemeCloseData(hTheme);
  return 1;
}

void iupwinDrawThemeFrameBorder(HWND hWnd, HDC hDC, RECT *rect, UINT itemState)
{
  int iStateId = GBS_NORMAL;
  HTHEME hTheme;

  if (!winDrawThemeEnabled()) 
    return; 

  hTheme = winThemeOpenData(hWnd, L"BUTTON");
  if (!hTheme) 
    return;

  if (itemState & ODS_DISABLED)
    iStateId = GBS_DISABLED;

  winThemeDrawBackground(hTheme, hDC, BP_GROUPBOX, iStateId, rect, NULL);

  winThemeCloseData(hTheme);
}

int iupwinDrawGetThemeTabsBgColor(HWND hWnd, COLORREF *color)
{
  HTHEME hTheme;
  HRESULT ret;

  if (!winDrawThemeEnabled()) 
    return 0; 

  hTheme = winThemeOpenData(hWnd, L"TAB");
  if (!hTheme) 
    return 0;

  if (iupwinIsVista())
    ret = winThemeGetColor(hTheme, TABP_AEROWIZARDBODY, TIS_NORMAL, TMT_FILLCOLORHINT, color);
  else
    ret = winThemeGetColor(hTheme, TABP_BODY, TIS_NORMAL, TMT_FILLCOLORHINT, color);

  winThemeCloseData(hTheme);
  return (ret == S_OK)? 1: 0;
}

int iupwinDrawGetThemeButtonBgColor(HWND hWnd, COLORREF *color)
{
  HTHEME hTheme;
  HRESULT ret;

  if (!winDrawThemeEnabled()) 
    return 0; 

  hTheme = winThemeOpenData(hWnd, L"BUTTON");
  if (!hTheme) 
    return 0;

  ret = winThemeGetColor(hTheme, BP_PUSHBUTTON, PBS_NORMAL, TMT_FILLCOLORHINT, color);

  winThemeCloseData(hTheme);
  return (ret == S_OK)? 1: 0;
}

int iupwinDrawGetThemeFrameFgColor(HWND hWnd, COLORREF *color)
{
  HTHEME hTheme;
  HRESULT ret;

  if (!winDrawThemeEnabled()) 
    return 0; 

  hTheme = winThemeOpenData(hWnd, L"BUTTON");
  if (!hTheme) 
    return 0;

  ret = winThemeGetColor(hTheme, BP_GROUPBOX, GBS_NORMAL, TMT_TEXTCOLOR, color);

  winThemeCloseData(hTheme);
  return (ret == S_OK)? 1: 0;
}

static int winDrawGetStateId(int itemState)
{
  if (itemState & ODS_DISABLED)
    return DFCS_INACTIVE; 
  else if (itemState & ODS_SELECTED)
    return DFCS_PUSHED;  
  else if (itemState & ODS_HOTLIGHT)
    return DFCS_HOT;      
  else
    return 0;   
}

void iupwinDrawButtonBorder(HWND hWnd, HDC hDC, RECT *rect, UINT itemState)
{
  if (!winDrawThemeButtonBorder(hWnd, hDC, rect, itemState))
  {
    DrawFrameControl(hDC, rect, DFC_BUTTON, DFCS_BUTTONPUSH | winDrawGetStateId(itemState));
    if (itemState & ODS_DEFAULT)  /* if a button has the focus, must draw the default button frame */
      FrameRect(hDC, rect, (HBRUSH)GetStockObject(BLACK_BRUSH));
  }
}

void iupdrvDrawFocusRect(Ihandle* ih, void* gc, int x, int y, int w, int h)
{
  HDC hDC = (HDC)gc;
  RECT rect;
  (void)ih;

  rect.left = x;  
  rect.top = y;  
  rect.right = x+w;  
  rect.bottom = y+h;

  DrawFocusRect(hDC, &rect);
}

void iupwinDrawRemoveTheme(HWND hwnd)
{
  typedef HRESULT (STDAPICALLTYPE *winSetWindowTheme)(HWND hwnd, LPCWSTR pszSubAppName, LPCWSTR pszSubIdList);
  static winSetWindowTheme mySetWindowTheme = NULL;
  if (!mySetWindowTheme)
  {
    HMODULE hinstDll = LoadLibrary("uxtheme.dll");
    if (hinstDll)
      mySetWindowTheme = (winSetWindowTheme)GetProcAddress(hinstDll, "SetWindowTheme");
  }

  if (mySetWindowTheme)
    mySetWindowTheme(hwnd, L"", L"");
}

void iupwinDrawParentBackground(Ihandle* ih, HDC hDC, RECT* rect)
{
  unsigned char r=0, g=0, b=0;
  char* color = iupBaseNativeParentGetBgColorAttrib(ih);
  iupStrToRGB(color, &r, &g, &b);
  SetDCBrushColor(hDC, RGB(r,g,b));
  FillRect(hDC, rect, (HBRUSH)GetStockObject(DC_BRUSH));
}

HDC iupwinDrawCreateBitmapDC(iupwinBitmapDC *bmpDC, HDC hDC, int w, int h)
{
  bmpDC->w = w;
  bmpDC->h = h;
  bmpDC->hDC = hDC;

  bmpDC->hBitmap = CreateCompatibleBitmap(bmpDC->hDC, w, h);
  bmpDC->hBitmapDC = CreateCompatibleDC(bmpDC->hDC);
  bmpDC->hOldBitmap = SelectObject(bmpDC->hBitmapDC, bmpDC->hBitmap);
  return bmpDC->hBitmapDC;
}

void iupwinDrawDestroyBitmapDC(iupwinBitmapDC *bmpDC)
{
  BitBlt(bmpDC->hDC, 0, 0, bmpDC->w, bmpDC->h, bmpDC->hBitmapDC, 0, 0, SRCCOPY);
  SelectObject(bmpDC->hBitmapDC, bmpDC->hOldBitmap);
  DeleteObject(bmpDC->hBitmap);
  DeleteDC(bmpDC->hBitmapDC);
}

