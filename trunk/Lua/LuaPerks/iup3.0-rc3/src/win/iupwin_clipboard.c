/** \file
 * \brief Clipboard for the Windows Driver.
 *
 * See Copyright Notice in "iup.h"
 */

#include <windows.h>
 
#include <stdio.h>
#include <stdlib.h>

#include "iup.h"
#include "iupcbs.h"

#include "iup_object.h"
#include "iup_attrib.h"
#include "iup_str.h"
#include "iup_image.h"

#include "iupwin_drv.h"


static int winClipboardSetTextAttrib(Ihandle *ih, const char *value)
{
  HANDLE hHandle;
  void* clip_str;
  int size = strlen(value)+1;
  (void)ih;

  if (!OpenClipboard(NULL))
    return 0;

  hHandle = GlobalAlloc(GMEM_MOVEABLE, size); 
  if (!hHandle)
    return 0;

  clip_str = GlobalLock(hHandle);
  CopyMemory(clip_str, value, size);
  GlobalUnlock(hHandle);

  EmptyClipboard();
  SetClipboardData(CF_TEXT, hHandle);
  CloseClipboard();

  return 0;
}

static char* winClipboardGetTextAttrib(Ihandle *ih)
{
  HANDLE hHandle;
  char* str;
  (void)ih;

  if (!OpenClipboard(NULL))
    return NULL;

  hHandle = GetClipboardData(CF_TEXT);
  if (!hHandle)
  {
    CloseClipboard();
    return NULL;
  }
  
  str = iupStrGetMemoryCopy((char*)GlobalLock(hHandle));
 
  GlobalUnlock(hHandle);
  CloseClipboard();
  return str;
}

static int winClipboardSetImageAttrib(Ihandle *ih, const char *value)
{
  HBITMAP hBitmap;

  if (!OpenClipboard(NULL))
    return 0;

  hBitmap = (HBITMAP)iupImageGetImage(value, ih, 0);
  iupImageClearCache(ih, hBitmap);

  EmptyClipboard();
  SetClipboardData(CF_BITMAP, (HANDLE)hBitmap);
  CloseClipboard();

  return 0;
}

static int winClipboardSetNativeImageAttrib(Ihandle *ih, const char *value)
{
  if (!OpenClipboard(NULL))
    return 0;

  EmptyClipboard();
  SetClipboardData(CF_DIB, (HANDLE)value);
  CloseClipboard();

  (void)ih;
  return 0;
}

static HANDLE winCopyHandle(HANDLE hHandle)
{
  void *src_data, *dst_data;
  SIZE_T size = GlobalSize(hHandle);
  HANDLE hNewHandle = GlobalAlloc(GMEM_MOVEABLE, size); 
  if (!hNewHandle)
    return NULL;

  src_data = GlobalLock(hHandle);
  dst_data = GlobalLock(hNewHandle);
  CopyMemory(dst_data, src_data, size);
  GlobalUnlock(hHandle);
  GlobalUnlock(hNewHandle);

  return hNewHandle;
}

static char* winClipboardGetNativeImageAttrib(Ihandle *ih)
{
  HANDLE hHandle;

  if (!OpenClipboard(NULL))
    return 0;

  hHandle = GetClipboardData(CF_DIB);
  if (!hHandle)
  {
    CloseClipboard();
    return NULL;
  }

  hHandle = winCopyHandle(hHandle);   /* must duplicate because CloseClipboard will invalidate the handle */
  CloseClipboard();
  
  (void)ih;
  return (char*)hHandle;
}

static char* winClipboardGetTextAvailableAttrib(Ihandle *ih)
{
  int check;
  (void)ih;
  OpenClipboard(NULL);
  check = IsClipboardFormatAvailable(CF_TEXT);
  CloseClipboard();
  if (check)
    return "YES";
  else
    return "NO";
}

static char* winClipboardGetImageAvailableAttrib(Ihandle *ih)
{
  int check;
  (void)ih;
  OpenClipboard(NULL);
  check = IsClipboardFormatAvailable(CF_DIB);
  CloseClipboard();
  if (check)
    return "YES";
  else
    return "NO";
}

/******************************************************************************/

Ihandle* IupClipboard(void)
{
  return IupCreate("clipboard");
}

Iclass* iupClipboardGetClass(void)
{
  Iclass* ic = iupClassNew(NULL);

  ic->name = "clipboard";
  ic->format = NULL;  /* no parameters */
  ic->nativetype = IUP_TYPECONTROL;
  ic->childtype = IUP_CHILDNONE;
  ic->is_interactive = 0;

  /* Attribute functions */
  iupClassRegisterAttribute(ic, "TEXT", winClipboardGetTextAttrib, winClipboardSetTextAttrib, NULL, NULL, IUPAF_NOT_MAPPED|IUPAF_NO_INHERIT);
  iupClassRegisterAttribute(ic, "NATIVEIMAGE", winClipboardGetNativeImageAttrib, winClipboardSetNativeImageAttrib, NULL, NULL, IUPAF_NO_STRING|IUPAF_NOT_MAPPED|IUPAF_NO_INHERIT);
  iupClassRegisterAttribute(ic, "IMAGE", NULL, winClipboardSetImageAttrib, NULL, NULL, IUPAF_WRITEONLY|IUPAF_NOT_MAPPED|IUPAF_NO_INHERIT);
  iupClassRegisterAttribute(ic, "TEXTAVAILABLE", winClipboardGetTextAvailableAttrib, NULL, NULL, NULL, IUPAF_READONLY|IUPAF_NOT_MAPPED|IUPAF_NO_INHERIT);
  iupClassRegisterAttribute(ic, "IMAGEAVAILABLE", winClipboardGetImageAvailableAttrib, NULL, NULL, NULL, IUPAF_READONLY|IUPAF_NOT_MAPPED|IUPAF_NO_INHERIT);

  return ic;
}
