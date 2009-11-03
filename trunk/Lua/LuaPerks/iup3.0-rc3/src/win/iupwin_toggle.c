/** \file
 * \brief Toggle Control
 *
 * See Copyright Notice in "iup.h"
 */

#include <windows.h>
#include <commctrl.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <memory.h>
#include <stdarg.h>

#include "iup.h"
#include "iupcbs.h"

#include "iup_object.h"
#include "iup_attrib.h"
#include "iup_str.h"
#include "iup_toggle.h"
#include "iup_drv.h"
#include "iup_image.h"

#include "iupwin_drv.h"
#include "iupwin_handle.h"
#include "iupwin_draw.h"


#ifndef CDIS_SHOWKEYBOARDCUES
#define CDIS_SHOWKEYBOARDCUES   0x0200    /* it is defined only when _WIN32_WINNT >= 0x0501 */
#endif


void iupdrvToggleAddCheckBox(int *x, int *y)
{
  (*x) += 16+6;
  if ((*y) < 16) (*y) = 16; /* minimum height */
}

static int winToggleIsActive(Ihandle* ih)
{
  return iupAttribGetInt(ih, "_IUPWIN_ACTIVE");
}

static void winToggleSetBitmap(Ihandle* ih, const char* name, int make_inactive)
{
  if (name)
  {
    HBITMAP bitmap = iupImageGetImage(name, ih, make_inactive);
    SendMessage(ih->handle, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)bitmap);
  }
  else
    SendMessage(ih->handle, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)NULL);  /* if not defined */
}

static void winToggleUpdateImage(Ihandle* ih, int active, int check)
{
  /* called only when (ih->data->type == IUP_TOGGLE_IMAGE && !iupwin_comctl32ver6) */
  char* name;

  if (!active)
  {
    name = iupAttribGet(ih, "IMINACTIVE");
    if (name)
      winToggleSetBitmap(ih, name, 0);
    else
    {
      /* if not defined then automaticaly create one based on IMAGE */
      name = iupAttribGet(ih, "IMAGE");
      winToggleSetBitmap(ih, name, 1); /* make_inactive */
    }
  }
  else
  {
    /* must restore the normal image */
    if (check)
    {
      name = iupAttribGet(ih, "IMPRESS");
      if (name)
        winToggleSetBitmap(ih, name, 0);
      else
      {
        /* if not defined then automaticaly create one based on IMAGE */
        name = iupAttribGet(ih, "IMAGE");
        winToggleSetBitmap(ih, name, 0);
      }
    }
    else
    {
      name = iupAttribGet(ih, "IMAGE");
      if (name)
        winToggleSetBitmap(ih, name, 0);
    }
  }
}

static void winToggleGetAlignment(Ihandle* ih, int *horiz_alignment, int *vert_alignment)
{
  char value1[30]="", value2[30]="";

  iupStrToStrStr(iupAttribGetStr(ih, "ALIGNMENT"), value1, value2, ':');

  if (iupStrEqualNoCase(value1, "ARIGHT"))
    *horiz_alignment = IUP_ALIGN_ARIGHT;
  else if (iupStrEqualNoCase(value1, "ALEFT"))
    *horiz_alignment = IUP_ALIGN_ALEFT;
  else /* "ACENTER" */
    *horiz_alignment = IUP_ALIGN_ACENTER;

  if (iupStrEqualNoCase(value2, "ABOTTOM"))
    *vert_alignment = IUP_ALIGN_ABOTTOM;
  else if (iupStrEqualNoCase(value2, "ATOP"))
    *vert_alignment = IUP_ALIGN_ATOP;
  else /* "ACENTER" */
    *vert_alignment = IUP_ALIGN_ACENTER;
}

static void winToggleDrawImage(Ihandle* ih, HDC hDC, int rect_width, int rect_height, int border, UINT itemState)
{
  int xpad = ih->data->horiz_padding + border, 
      ypad = ih->data->vert_padding + border;
  int horiz_alignment, vert_alignment;
  int x, y, width, height, bpp, shift = 1;
  HBITMAP hBitmap, hMask = NULL;
  char *name;
  int make_inactive = 0;

  if (itemState & ODS_DISABLED)
  {
    name = iupAttribGet(ih, "IMINACTIVE");
    if (!name)
    {
      name = iupAttribGet(ih, "IMAGE");
      make_inactive = 1;
    }
  }
  else
  {
    name = iupAttribGet(ih, "IMPRESS");
    if (itemState & ODS_SELECTED && name)
      shift = 0;
    else
      name = iupAttribGet(ih, "IMAGE");
  }

  hBitmap = iupImageGetImage(name, ih, make_inactive);
  if (!hBitmap)
    return;

  /* must use this info, since image can be a driver image loaded from resources */
  iupdrvImageGetInfo(hBitmap, &width, &height, &bpp);

  winToggleGetAlignment(ih, &horiz_alignment, &vert_alignment);
  if (horiz_alignment == IUP_ALIGN_ARIGHT)
    x = rect_width - (width + 2*xpad);
  else if (horiz_alignment == IUP_ALIGN_ACENTER)
    x = (rect_width - (width + 2*xpad))/2;
  else  /* ALEFT */
    x = 0;

  if (vert_alignment == IUP_ALIGN_ABOTTOM)
    y = rect_height - (height + 2*ypad);
  else if (vert_alignment == IUP_ALIGN_ATOP)
    y = 0;
  else  /* ACENTER */
    y = (rect_height - (height + 2*ypad))/2;

  x += xpad;
  y += ypad;

  if (itemState & ODS_SELECTED && !iupwin_comctl32ver6 && shift)
  {
    x++;
    y++;
  }

  if (bpp == 8)
    hMask = iupdrvImageCreateMask(IupGetHandle(name));

  iupwinDrawBitmap(hDC, hBitmap, hMask, x, y, width, height, bpp);

  if (hMask)
    DeleteObject(hMask);
}

static void winToggleDrawItem(Ihandle* ih, DRAWITEMSTRUCT *drawitem)
{ 
  int width, height, border = 4, check;
  HDC hDC;
  iupwinBitmapDC bmpDC;

  width = drawitem->rcItem.right - drawitem->rcItem.left;
  height = drawitem->rcItem.bottom - drawitem->rcItem.top;

  hDC = iupwinDrawCreateBitmapDC(&bmpDC, drawitem->hDC, width, height);

  iupwinDrawParentBackground(ih, hDC, &drawitem->rcItem);

  check = SendMessage(ih->handle, BM_GETCHECK, 0, 0L);
  if (check)
    drawitem->itemState |= ODS_SELECTED;
  else
    drawitem->itemState |= ODS_DEFAULT;  /* use default mark for NOT checked */

  iupwinDrawButtonBorder(ih->handle, hDC, &drawitem->rcItem, drawitem->itemState);

  winToggleDrawImage(ih, hDC, width, height, border, drawitem->itemState);

  if (drawitem->itemState & ODS_FOCUS)
  {
    border--;
    iupdrvDrawFocusRect(ih, hDC, border, border, width-2*border, height-2*border);
  }

  iupwinDrawDestroyBitmapDC(&bmpDC);
}


/***********************************************************************************************/


static int winToggleSetImageAttrib(Ihandle* ih, const char* value)
{
  if (ih->data->type == IUP_TOGGLE_IMAGE)
  {
    if (value != iupAttribGet(ih, "IMAGE"))
      iupAttribSetStr(ih, "IMAGE", (char*)value);

    if (iupwin_comctl32ver6)
      iupdrvDisplayRedraw(ih);
    else
    {
      int check = SendMessage(ih->handle, BM_GETCHECK, 0L, 0L);
      winToggleUpdateImage(ih, winToggleIsActive(ih), check);
    }
    return 1;
  }
  else
    return 0;
}

static int winToggleSetImInactiveAttrib(Ihandle* ih, const char* value)
{
  if (ih->data->type == IUP_TOGGLE_IMAGE)
  {
    if (value != iupAttribGet(ih, "IMINACTIVE"))
      iupAttribSetStr(ih, "IMINACTIVE", (char*)value);

    if (iupwin_comctl32ver6)
      iupdrvDisplayRedraw(ih);
    else
    {
      int check = SendMessage(ih->handle, BM_GETCHECK, 0L, 0L);
      winToggleUpdateImage(ih, winToggleIsActive(ih), check);
    }
    return 1;
  }
  else
    return 0;
}

static int winToggleSetImPressAttrib(Ihandle* ih, const char* value)
{
  if (ih->data->type == IUP_TOGGLE_IMAGE)
  {
    if (value != iupAttribGet(ih, "IMPRESS"))
      iupAttribSetStr(ih, "IMPRESS", (char*)value);

    if (iupwin_comctl32ver6)
      iupdrvDisplayRedraw(ih);
    else
    {
      int check = SendMessage(ih->handle, BM_GETCHECK, 0L, 0L);
      winToggleUpdateImage(ih, winToggleIsActive(ih), check);
    }
    return 1;
  }
  else
    return 0;
}

static int winToggleSetValueAttrib(Ihandle* ih, const char* value)
{
  Ihandle *radio;
  int check;

  if (iupStrEqualNoCase(value,"NOTDEF"))
    check = BST_INDETERMINATE;
  else if (iupStrBoolean(value))
    check = BST_CHECKED;
  else
    check = BST_UNCHECKED;

  /* This is necessary because Windows does not handle the radio state 
     when a toggle is programatically changed. */
  radio = iupRadioFindToggleParent(ih);
  if (radio)
  {
    int oldcheck = (int)SendMessage(ih->handle, BM_GETCHECK, 0, 0L);

    Ihandle* last_tg = (Ihandle*)iupAttribGet(radio, "_IUPWIN_LASTTOGGLE");
    if (check)
    {
      if (iupObjectCheck(last_tg) && last_tg != ih)
          SendMessage(last_tg->handle, BM_SETCHECK, BST_UNCHECKED, 0L);
      iupAttribSetStr(radio, "_IUPWIN_LASTTOGGLE", (char*)ih);
    }

    if (last_tg != ih && oldcheck != check)
      SendMessage(ih->handle, BM_SETCHECK, check, 0L);
  }
  else
    SendMessage(ih->handle, BM_SETCHECK, check, 0L);

  if (ih->data->type == IUP_TOGGLE_IMAGE && !iupwin_comctl32ver6)
    winToggleUpdateImage(ih, winToggleIsActive(ih), check);

  return 0;
}

static char* winToggleGetValueAttrib(Ihandle* ih)
{
  int check = (int)SendMessage(ih->handle, BM_GETCHECK, 0, 0L);
  if (check == BST_INDETERMINATE)
    return "NOTDEF";
  else if (check == BST_CHECKED)
    return "ON";
  else
    return "OFF";
}

static int winToggleSetActiveAttrib(Ihandle* ih, const char* value)
{
  /* update the inactive image if necessary */
  if (ih->data->type == IUP_TOGGLE_IMAGE)
  {
    if (iupwin_comctl32ver6)
    {
      iupBaseSetActiveAttrib(ih, value);
      iupdrvDisplayRedraw(ih);
      return 0;
    }
    else
    {
      int active = iupStrBoolean(value);
      int check = SendMessage(ih->handle, BM_GETCHECK, 0, 0L);
      if (active)
        iupAttribSetStr(ih, "_IUPWIN_ACTIVE", "YES");
      else
        iupAttribSetStr(ih, "_IUPWIN_ACTIVE", "NO");
      winToggleUpdateImage(ih, active, check);
      return 0;
    }
  }

  return iupBaseSetActiveAttrib(ih, value);
}

static char* winToggleGetActiveAttrib(Ihandle* ih)
{
  if (ih->data->type == IUP_TOGGLE_IMAGE && !iupwin_comctl32ver6)
    return iupAttribGet(ih, "_IUPWIN_ACTIVE");
  else
    return iupBaseGetActiveAttrib(ih);
}

static int winToggleSetTitleAttrib(Ihandle* ih, const char* value)
{
  if (ih->data->type == IUP_TOGGLE_TEXT)
  {
    if (!value)
      value = "";
    SetWindowText(ih->handle, value);
  }
  return 0;
}

static int winToggleSetPaddingAttrib(Ihandle* ih, const char* value)
{
  iupStrToIntInt(value, &ih->data->horiz_padding, &ih->data->vert_padding, 'x');

  if (ih->handle && iupwin_comctl32ver6 && ih->data->type == IUP_TOGGLE_IMAGE)
    iupdrvDisplayRedraw(ih);

  return 0;
}

static int winToggleSetBgColorAttrib(Ihandle* ih, const char* value)
{
  (void)value;
  if (ih->data->type == IUP_TOGGLE_IMAGE)
  {
    /* update internal image cache for controls that have the IMAGE attribute */
    iupAttribSetStr(ih, "BGCOLOR", value);
    iupImageUpdateParent(ih);
    iupdrvDisplayRedraw(ih);
  }
  return 1;
}

static char* winToggleGetBgColorAttrib(Ihandle* ih)
{
  /* the most important use of this is to provide
     the correct background for images */
  if (iupwin_comctl32ver6 && ih->data->type == IUP_TOGGLE_IMAGE)
  {
    COLORREF cr;
    if (iupwinDrawGetThemeButtonBgColor(ih->handle, &cr))
    {
      char* str = iupStrGetMemory(20);
      sprintf(str, "%d %d %d", (int)GetRValue(cr), (int)GetGValue(cr), (int)GetBValue(cr));
      return str;
    }
  }

  if (ih->data->type == IUP_TOGGLE_TEXT)
    return iupBaseNativeParentGetBgColorAttrib(ih);
  else
    return IupGetGlobal("DLGBGCOLOR");
}


/****************************************************************************************/


static int winToggleCtlColor(Ihandle* ih, HDC hdc, LRESULT *result)
{
  COLORREF cr;

  SetBkMode(hdc, TRANSPARENT);

  if (iupwinGetColorRef(ih, "FGCOLOR", &cr))
    SetTextColor(hdc, cr);

  if (iupwinGetParentBgColor(ih, &cr))
  {
    SetDCBrushColor(hdc, cr);
    *result = (LRESULT)GetStockObject(DC_BRUSH);
    return 1;
  }
  return 0;
}

static int winToggleWmNotify(Ihandle* ih, NMHDR* msg_info, int *result)
{
  if (msg_info->code == NM_CUSTOMDRAW)
  {
    /* called only when iupwin_comctl32ver6 AND (ih->data->type==IUP_TOGGLE_IMAGE) */
    NMCUSTOMDRAW *customdraw = (NMCUSTOMDRAW*)msg_info;

    if (customdraw->dwDrawStage==CDDS_PREERASE)
    {
      DRAWITEMSTRUCT drawitem;
      drawitem.itemState = 0;

      if (customdraw->uItemState & CDIS_DISABLED)
        drawitem.itemState |= ODS_DISABLED;
      else if (customdraw->uItemState & CDIS_SELECTED)
        drawitem.itemState |= ODS_SELECTED;
      else if (customdraw->uItemState & CDIS_HOT)
        drawitem.itemState |= ODS_HOTLIGHT;
      else if (customdraw->uItemState & CDIS_DEFAULT)
        drawitem.itemState |= ODS_DEFAULT;

      if (customdraw->uItemState & CDIS_FOCUS && (customdraw->uItemState & CDIS_SHOWKEYBOARDCUES))
        drawitem.itemState |= ODS_FOCUS;

      drawitem.hDC = customdraw->hdc;
      drawitem.rcItem = customdraw->rc;

      winToggleDrawItem(ih, (void*)&drawitem);  /* Simulate a WM_DRAWITEM */

      *result = CDRF_SKIPDEFAULT;
      return 1;
    }
  }

  return 0; /* result not used */
}

static int winToggleProc(Ihandle* ih, UINT msg, WPARAM wp, LPARAM lp, LRESULT *result)
{
  (void)lp;
  (void)wp;

  switch (msg)
  {
    case WM_MOUSEACTIVATE:
      if (!winToggleIsActive(ih))
      {
        *result = MA_NOACTIVATEANDEAT;
        return 1;
      }
      break;
    case WM_LBUTTONDOWN:
    case WM_LBUTTONUP:
    case WM_ACTIVATE:
    case WM_SETFOCUS:
      if (!winToggleIsActive(ih)) 
      {
        *result = 0;
        return 1;
      }
      break;
  }

  if (msg == WM_LBUTTONDOWN)
    winToggleUpdateImage(ih, 1, 1);
  else if (msg == WM_LBUTTONUP)
    winToggleUpdateImage(ih, 1, 0);

  return iupwinBaseProc(ih, msg, wp, lp, result);
}

static int winToggleWmCommand(Ihandle* ih, WPARAM wp, LPARAM lp)
{
  (void)lp;

  switch (HIWORD(wp))
  {
  case BN_DOUBLECLICKED:
  case BN_CLICKED:
    {
      Ihandle *radio;
      IFni cb;
      int check = SendMessage(ih->handle, BM_GETCHECK, 0, 0L);

      if (ih->data->type == IUP_TOGGLE_IMAGE && !iupwin_comctl32ver6)
      {
        int active = winToggleIsActive(ih);
        winToggleUpdateImage(ih, active, check);
        if (!active)
          return 0;
      }

      radio = iupRadioFindToggleParent(ih);
      if (radio)
      {
        /* This is necessary because Windows does not send a message
           when a toggle is unchecked in a Radio. 
           Also if the toggle is already checked in a radio, 
           a click will call the callback again. */

        Ihandle* last_tg = (Ihandle*)iupAttribGet(radio, "_IUPWIN_LASTTOGGLE");
        if (iupObjectCheck(last_tg) && last_tg != ih)
        {
          /* uncheck last toggle */
          SendMessage(last_tg->handle, BM_SETCHECK, BST_UNCHECKED, 0L);

          cb = (IFni) IupGetCallback(last_tg, "ACTION");
          if (cb && cb(last_tg, 0) == IUP_CLOSE)
              IupExitLoop();

          iupBaseCallValueChangedCb(last_tg);
        }
        iupAttribSetStr(radio, "_IUPWIN_LASTTOGGLE", (char*)ih);

        if (last_tg != ih)
        {
          /* check new toggle */
          SendMessage(ih->handle, BM_SETCHECK, BST_CHECKED, 0L);

          cb = (IFni)IupGetCallback(ih, "ACTION");
          if (cb && cb (ih, 1) == IUP_CLOSE)
              IupExitLoop();

          iupBaseCallValueChangedCb(ih);
        }
      }
      else
      {
        if (check == BST_INDETERMINATE)
          check = -1;

        cb = (IFni)IupGetCallback(ih, "ACTION");
        if (cb && cb (ih, check) == IUP_CLOSE)
            IupExitLoop();

        iupBaseCallValueChangedCb(ih);
      }
    }
  }


  return 0; /* not used */
}

static int winToggleMapMethod(Ihandle* ih)
{
  Ihandle* radio = iupRadioFindToggleParent(ih);
  char* value;
  DWORD dwStyle = WS_CHILD | 
                  BS_NOTIFY; /* necessary because of the base messages */

  if (!ih->parent)
    return IUP_ERROR;

  if (radio)
    ih->data->radio = 1;

  value = iupAttribGet(ih, "IMAGE");
  if (value)
  {
    ih->data->type = IUP_TOGGLE_IMAGE;
    dwStyle |= BS_BITMAP|BS_PUSHLIKE;
  }
  else
  {
    ih->data->type = IUP_TOGGLE_TEXT;
    dwStyle |= BS_TEXT|BS_MULTILINE;

    if (iupAttribGetBoolean(ih, "RIGHTBUTTON"))
      dwStyle |= BS_RIGHTBUTTON;
  }

  if (iupAttribGetBoolean(ih, "CANFOCUS"))
    dwStyle |= WS_TABSTOP;

  if (radio)
  {
    dwStyle |= BS_RADIOBUTTON;

    if (!iupAttribGet(radio, "_IUPWIN_LASTTOGGLE"))
    {
      /* this is the first toggle in the radio, and the last toggle with VALUE=ON */
      iupAttribSetStr(ih, "VALUE","ON");
    }
  }
  else
  {
    if (ih->data->type == IUP_TOGGLE_TEXT && iupAttribGetBoolean(ih, "3STATE"))
      dwStyle |= BS_AUTO3STATE;
    else
      dwStyle |= BS_AUTOCHECKBOX;
  }

  if (!iupwinCreateWindowEx(ih, "BUTTON", 0, dwStyle))
    return IUP_ERROR;

  /* Process WM_COMMAND */
  IupSetCallback(ih, "_IUPWIN_COMMAND_CB", (Icallback)winToggleWmCommand);

  /* Process background color */
  IupSetCallback(ih, "_IUPWIN_CTLCOLOR_CB", (Icallback)winToggleCtlColor);

  if (ih->data->type == IUP_TOGGLE_IMAGE)
  {
    if (iupwin_comctl32ver6)
      IupSetCallback(ih, "_IUPWIN_NOTIFY_CB", (Icallback)winToggleWmNotify);  /* Process WM_NOTIFY */
    else
    {
      IupSetCallback(ih, "_IUPWIN_CTRLPROC_CB", (Icallback)winToggleProc);
      iupAttribSetStr(ih, "_IUPWIN_ACTIVE", "YES");
    }
  }

  return IUP_NOERROR;
}

void iupdrvToggleInitClass(Iclass* ic)
{
  /* Driver Dependent Class functions */
  ic->Map = winToggleMapMethod;

  /* Driver Dependent Attribute functions */

  /* Overwrite Visual */
  iupClassRegisterAttribute(ic, "ACTIVE", winToggleGetActiveAttrib, winToggleSetActiveAttrib, IUPAF_SAMEASSYSTEM, "YES", IUPAF_DEFAULT);

  /* Visual */
  iupClassRegisterAttribute(ic, "BGCOLOR", winToggleGetBgColorAttrib, winToggleSetBgColorAttrib, IUPAF_SAMEASSYSTEM, "DLGBGCOLOR", IUPAF_DEFAULT);  

  /* Special */
  iupClassRegisterAttribute(ic, "FGCOLOR", NULL, NULL, "DLGFGCOLOR", NULL, IUPAF_NOT_MAPPED);  /* force the new default value */  
  iupClassRegisterAttribute(ic, "TITLE", iupdrvBaseGetTitleAttrib, winToggleSetTitleAttrib, NULL, NULL, IUPAF_NO_DEFAULTVALUE|IUPAF_NO_INHERIT);

  /* IupToggle only */
  iupClassRegisterAttribute(ic, "ALIGNMENT", NULL, NULL, IUPAF_SAMEASSYSTEM, "ACENTER:ACENTER", IUPAF_NO_INHERIT);
  iupClassRegisterAttribute(ic, "IMAGE", NULL, winToggleSetImageAttrib, NULL, NULL, IUPAF_NO_DEFAULTVALUE|IUPAF_NO_INHERIT);
  iupClassRegisterAttribute(ic, "IMINACTIVE", NULL, winToggleSetImInactiveAttrib, NULL, NULL, IUPAF_NO_DEFAULTVALUE|IUPAF_NO_INHERIT);
  iupClassRegisterAttribute(ic, "IMPRESS", NULL, winToggleSetImPressAttrib, NULL, NULL, IUPAF_NO_DEFAULTVALUE|IUPAF_NO_INHERIT);
  iupClassRegisterAttribute(ic, "VALUE", winToggleGetValueAttrib, winToggleSetValueAttrib, NULL, NULL, IUPAF_NO_DEFAULTVALUE|IUPAF_NO_INHERIT);
  iupClassRegisterAttribute(ic, "PADDING", iupToggleGetPaddingAttrib, winToggleSetPaddingAttrib, IUPAF_SAMEASSYSTEM, "0x0", IUPAF_NOT_MAPPED);

  /* IupToggle Windows only */
  iupClassRegisterAttribute(ic, "RIGHTBUTTON", NULL, NULL, NULL, NULL, IUPAF_NO_INHERIT);

  /* necessary because it uses an old HBITMAP solution when NOT using styles */
  if (!iupwin_comctl32ver6)  /* Used by iupdrvImageCreateImage */
    iupClassRegisterAttribute(ic, "FLAT_ALPHA", NULL, NULL, IUPAF_SAMEASSYSTEM, "YES", IUPAF_NOT_MAPPED|IUPAF_NO_INHERIT);
}
