/** \file
 * \brief initializes iupdial, iupgauge, iuptabs, iupcb, iupgc and iupval controls.
 *
 * See Copyright Notice in iup.h
 */

#include <stdlib.h>

#include "iup.h"

#include "iup_str.h"
#include "iup_image.h"

/* GTK and Win32 share the same library in Windows */
/* GTK and Motif share the same library in UNIX */

#ifndef IUP_IMGLIB_DUMMY
#ifdef WIN32
#include "iup_imglib_bitmaps.h"   /* Used only by the Win32 driver in Windows */
#endif
#ifndef WIN32
#include "iup_imglib_bitmaps8.h"  /* Used only by the Motif driver in UNIX */
#endif
#include "iup_imglib_bitmapsgtk.h"  /* Used only by the GTK driver in UNIX or Windows */
#ifndef WIN32
#include "iup_imglib_icons8.h"  /* Used only by the Motif driver in UNIX */
#include "iup_imglib_logos8.h"  /* Used only by the Motif driver in UNIX */
#endif
#include "iup_imglib_icons.h"
#include "iup_imglib_logos.h"
#endif

void IupImageLibOpen(void)
{
#ifndef IUP_IMGLIB_DUMMY
  int motif=0, win32=0, gtk=0;
  char* driver;

  if (IupGetGlobal("_IUP_IMAGELIB_OPEN"))
    return;

  driver = IupGetGlobal("DRIVER");
  if (iupStrEqualNoCase(driver, "GTK"))
    gtk = 1;
  else if (iupStrEqualNoCase(driver, "Motif"))
    motif = 1;
  else if (iupStrEqualNoCase(driver, "Win32"))
    win32 = 1;

  IupSetGlobal("_IUP_IMAGELIB_OPEN", "1");

  /**************** Bitmaps *****************/

#ifndef WIN32
  if (motif)
  {
    iupImageStockSet("IUP_ActionCancel", load_image_ActionCancel8, 0);
    iupImageStockSet("IUP_ActionOk", load_image_ActionOk8, 0);
    iupImageStockSet("IUP_ArrowDown", load_image_ArrowDown8, 0);
    iupImageStockSet("IUP_ArrowLeft", load_image_ArrowLeft8, 0);
    iupImageStockSet("IUP_ArrowRight", load_image_ArrowRight8, 0);
    iupImageStockSet("IUP_ArrowUp", load_image_ArrowUp8, 0);
    iupImageStockSet("IUP_EditCopy", load_image_EditCopy8, 0);
    iupImageStockSet("IUP_EditCut", load_image_EditCut8, 0);
    iupImageStockSet("IUP_EditErase", load_image_EditErase8, 0);
    iupImageStockSet("IUP_EditFind", load_image_EditFind8, 0);
    iupImageStockSet("IUP_EditPaste", load_image_EditPaste8, 0);
    iupImageStockSet("IUP_EditRedo", load_image_EditRedo8, 0);
    iupImageStockSet("IUP_EditUndo", load_image_EditUndo8, 0);
    iupImageStockSet("IUP_FileClose", load_image_FileClose8, 0);
    iupImageStockSet("IUP_FileCloseAll", load_image_FileCloseAll8, 0);
    iupImageStockSet("IUP_FileNew", load_image_FileNew8, 0);
    iupImageStockSet("IUP_FileOpen", load_image_FileOpen8, 0);
    iupImageStockSet("IUP_FileProperties", load_image_FileProperties8, 0);
    iupImageStockSet("IUP_FileSave", load_image_FileSave8, 0);
    iupImageStockSet("IUP_FileSaveAll", load_image_FileSaveAll8, 0);
    iupImageStockSet("IUP_FileText", load_image_FileText8, 0);
    iupImageStockSet("IUP_FontBold", load_image_FontBold8, 0);
    iupImageStockSet("IUP_FontDialog", load_image_FontDialog8, 0);
    iupImageStockSet("IUP_FontItalic", load_image_FontItalic8, 0);
    iupImageStockSet("IUP_MediaForward", load_image_MediaForward8, 0);
    iupImageStockSet("IUP_MediaGotoBegin", load_image_MediaGoToBegin8, 0);
    iupImageStockSet("IUP_MediaGoToEnd", load_image_MediaGoToEnd8, 0);
    iupImageStockSet("IUP_MediaPause", load_image_MediaPause8, 0);
    iupImageStockSet("IUP_MediaPlay", load_image_MediaPlay8, 0);
    iupImageStockSet("IUP_MediaRecord", load_image_MediaRecord8, 0);
    iupImageStockSet("IUP_MediaReverse", load_image_MediaReverse8, 0);
    iupImageStockSet("IUP_MediaRewind", load_image_MediaRewind8, 0);
    iupImageStockSet("IUP_MediaStop", load_image_MediaStop8, 0);
    iupImageStockSet("IUP_MessageError", load_image_MessageError8, 0);
    iupImageStockSet("IUP_MessageHelp", load_image_MessageHelp8, 0);
    iupImageStockSet("IUP_MessageInfo", load_image_MessageInfo8, 0);
    iupImageStockSet("IUP_NavigateHome", load_image_NavigateHome8, 0);
    iupImageStockSet("IUP_NavigateRefresh", load_image_NavigateRefresh8, 0);
    iupImageStockSet("IUP_Print", load_image_Print8, 0);
    iupImageStockSet("IUP_PrintPreview", load_image_PrintPreview8, 0);
    iupImageStockSet("IUP_ToolsColor", load_image_ToolsColor8, 0);
    iupImageStockSet("IUP_ToolsSettings", load_image_ToolsSettings8, 0);
    iupImageStockSet("IUP_ToolsSortAscend", load_image_ToolsSortAscend8, 0);
    iupImageStockSet("IUP_ToolsSortDescend", load_image_ToolsSortDescend8, 0);
    iupImageStockSet("IUP_ViewFullScreen", load_image_ViewFullScreen8, 0);
    iupImageStockSet("IUP_WindowsCascade", load_image_WindowsCascade8, 0);
    iupImageStockSet("IUP_WindowsTile", load_image_WindowsTile8, 0);
    iupImageStockSet("IUP_Zoom", load_image_Zoom8, 0);
    iupImageStockSet("IUP_ZoomActualSize", load_image_ZoomActualSize8, 0);
    iupImageStockSet("IUP_ZoomIn", load_image_ZoomIn8, 0);
    iupImageStockSet("IUP_ZoomOut", load_image_ZoomOut8, 0);
    iupImageStockSet("IUP_ZoomSelection", load_image_ZoomSelection8, 0);
  }
#endif

#ifdef WIN32
  if (win32)
  {
    iupImageStockSet("IUP_ActionCancel", load_image_ActionCancel, 0);
    iupImageStockSet("IUP_ActionOk", load_image_ActionOk, 0);
    iupImageStockSet("IUP_ArrowDown", load_image_ArrowDown, 0);
    iupImageStockSet("IUP_ArrowLeft", load_image_ArrowLeft, 0);
    iupImageStockSet("IUP_ArrowRight", load_image_ArrowRight, 0);
    iupImageStockSet("IUP_ArrowUp", load_image_ArrowUp, 0);
    iupImageStockSet("IUP_EditCopy", load_image_EditCopy, 0);
    iupImageStockSet("IUP_EditCut", load_image_EditCut, 0);
    iupImageStockSet("IUP_EditErase", load_image_EditErase, 0);
    iupImageStockSet("IUP_EditFind", load_image_EditFind, 0);
    iupImageStockSet("IUP_EditPaste", load_image_EditPaste, 0);
    iupImageStockSet("IUP_EditRedo", load_image_EditRedo, 0);
    iupImageStockSet("IUP_EditUndo", load_image_EditUndo, 0);
    iupImageStockSet("IUP_FileClose", load_image_FileClose, 0);
    iupImageStockSet("IUP_FileCloseAll", load_image_FileCloseAll, 0);
    iupImageStockSet("IUP_FileNew", load_image_FileNew, 0);
    iupImageStockSet("IUP_FileOpen", load_image_FileOpen, 0);
    iupImageStockSet("IUP_FileProperties", load_image_FileProperties, 0);
    iupImageStockSet("IUP_FileSave", load_image_FileSave, 0);
    iupImageStockSet("IUP_FileSaveAll", load_image_FileSaveAll, 0);
    iupImageStockSet("IUP_FileText", load_image_FileText, 0);
    iupImageStockSet("IUP_FontBold", load_image_FontBold, 0);
    iupImageStockSet("IUP_FontDialog", load_image_FontDialog, 0);
    iupImageStockSet("IUP_FontItalic", load_image_FontItalic, 0);
    iupImageStockSet("IUP_MediaForward", load_image_MediaForward, 0);
    iupImageStockSet("IUP_MediaGotoBegin", load_image_MediaGoToBegin, 0);
    iupImageStockSet("IUP_MediaGoToEnd", load_image_MediaGoToEnd, 0);
    iupImageStockSet("IUP_MediaPause", load_image_MediaPause, 0);
    iupImageStockSet("IUP_MediaPlay", load_image_MediaPlay, 0);
    iupImageStockSet("IUP_MediaRecord", load_image_MediaRecord, 0);
    iupImageStockSet("IUP_MediaReverse", load_image_MediaReverse, 0);
    iupImageStockSet("IUP_MediaRewind", load_image_MediaRewind, 0);
    iupImageStockSet("IUP_MediaStop", load_image_MediaStop, 0);
    iupImageStockSet("IUP_MessageError", load_image_MessageError, 0);
    iupImageStockSet("IUP_MessageHelp", load_image_MessageHelp, 0);
    iupImageStockSet("IUP_MessageInfo", load_image_MessageInfo, 0);
    iupImageStockSet("IUP_NavigateHome", load_image_NavigateHome, 0);
    iupImageStockSet("IUP_NavigateRefresh", load_image_NavigateRefresh, 0);
    iupImageStockSet("IUP_Print", load_image_Print, 0);
    iupImageStockSet("IUP_PrintPreview", load_image_PrintPreview, 0);
    iupImageStockSet("IUP_ToolsColor", load_image_ToolsColor, 0);
    iupImageStockSet("IUP_ToolsSettings", load_image_ToolsSettings, 0);
    iupImageStockSet("IUP_ToolsSortAscend", load_image_ToolsSortAscend, 0);
    iupImageStockSet("IUP_ToolsSortDescend", load_image_ToolsSortDescend, 0);
    iupImageStockSet("IUP_ViewFullScreen", load_image_ViewFullScreen, 0);
    iupImageStockSet("IUP_WindowsCascade", load_image_WindowsCascade, 0);
    iupImageStockSet("IUP_WindowsTile", load_image_WindowsTile, 0);
    iupImageStockSet("IUP_Zoom", load_image_Zoom, 0);
    iupImageStockSet("IUP_ZoomActualSize", load_image_ZoomActualSize, 0);
    iupImageStockSet("IUP_ZoomIn", load_image_ZoomIn, 0);
    iupImageStockSet("IUP_ZoomOut", load_image_ZoomOut, 0);
    iupImageStockSet("IUP_ZoomSelection", load_image_ZoomSelection, 0);
  }
#endif  

  if (gtk)
  {
    iupImageStockSet("IUP_ActionCancel", 0, "gtk-cancel");
    iupImageStockSet("IUP_ActionOk", 0, "gtk-apply");
    iupImageStockSet("IUP_ArrowDown", 0, "gtk-go-down");
    iupImageStockSet("IUP_ArrowLeft", 0, "gtk-go-back-ltr");
    iupImageStockSet("IUP_ArrowRight", 0, "gtk-go-forward-ltr");  
    iupImageStockSet("IUP_ArrowUp", 0, "gtk-go-up");       
    iupImageStockSet("IUP_EditCopy", 0, "gtk-copy");
    iupImageStockSet("IUP_EditCut", 0, "gtk-cut");
    iupImageStockSet("IUP_EditErase", 0, "gtk-close");
    iupImageStockSet("IUP_EditFind", 0, "gtk-find");
    iupImageStockSet("IUP_EditPaste", 0, "gtk-paste");
    iupImageStockSet("IUP_EditRedo", 0, "gtk-redo-ltr");
    iupImageStockSet("IUP_EditUndo", 0, "gtk-undo-ltr");
    iupImageStockSet("IUP_FileClose", load_image_iupgtk_close, 0);
    iupImageStockSet("IUP_FileCloseAll", load_image_iupgtk_close_all, 0);
    iupImageStockSet("IUP_FileNew", 0, "gtk-new");
    iupImageStockSet("IUP_FileOpen", 0, "gtk-open");
    iupImageStockSet("IUP_FileProperties", 0, "gtk-properties");
    iupImageStockSet("IUP_FileSave", 0, "gtk-save");
    iupImageStockSet("IUP_FileSaveAll", load_image_iupgtk_save_all, 0);
    iupImageStockSet("IUP_FileText", load_image_iupgtk_text, 0);
    iupImageStockSet("IUP_FontBold", 0, "gtk-bold");
    iupImageStockSet("IUP_FontDialog", 0, "gtk-font");
    iupImageStockSet("IUP_FontItalic", 0, "gtk-italic");
    iupImageStockSet("IUP_MediaForward", 0, "gtk-media-forward-ltr");
    iupImageStockSet("IUP_MediaGotoBegin", 0, "gtk-media-previous-ltr");
    iupImageStockSet("IUP_MediaGoToEnd", 0, "gtk-media-next-ltr");
    iupImageStockSet("IUP_MediaPause", 0, "gtk-media-pause");
    iupImageStockSet("IUP_MediaPlay", 0, "gtk-media-play-ltr");
    iupImageStockSet("IUP_MediaRecord", 0, "gtk-media-record");
    iupImageStockSet("IUP_MediaReverse", 0, "gtk-media-play-rtl");
    iupImageStockSet("IUP_MediaRewind", 0, "gtk-media-rewind-ltr");
    iupImageStockSet("IUP_MediaStop", 0, "gtk-media-stop");
    iupImageStockSet("IUP_MessageError", 0, "gtk-stop");
    iupImageStockSet("IUP_MessageHelp", 0, "gtk-help");
    iupImageStockSet("IUP_MessageInfo", 0, "gtk-info");
    iupImageStockSet("IUP_NavigateHome", 0, "gtk-home");
    iupImageStockSet("IUP_NavigateRefresh", 0, "gtk-refresh");
    iupImageStockSet("IUP_Print", 0, "gtk-print");
    iupImageStockSet("IUP_PrintPreview", 0, "gtk-print-preview");
    iupImageStockSet("IUP_ToolsColor", 0, "gtk-select-color");
    iupImageStockSet("IUP_ToolsSettings", 0, "gtk-preferences");
    iupImageStockSet("IUP_ToolsSortAscend", 0, "gtk-sort-ascending");
    iupImageStockSet("IUP_ToolsSortDescend", 0, "gtk-sort-descending");
    iupImageStockSet("IUP_ViewFullScreen", 0, "gtk-fullscreen");
    iupImageStockSet("IUP_WindowsCascade", load_image_iupgtk_cascade, 0);
    iupImageStockSet("IUP_WindowsTile", load_image_iupgtk_tile, 0);
    iupImageStockSet("IUP_Zoom", load_image_iupgtk_zoom, 0);
    iupImageStockSet("IUP_ZoomActualSize", 0, "gtk-zoom-100");
    iupImageStockSet("IUP_ZoomIn", 0, "gtk-zoom-in");
    iupImageStockSet("IUP_ZoomOut", 0, "gtk-zoom-out");
    iupImageStockSet("IUP_ZoomSelection", 0, "gtk-zoom-fit");
  }

  /***************** Icons *****************/

#ifndef WIN32
  if (motif)
  {
    iupImageStockSet("IUP_Tecgraf", load_image_Tecgraf8, 0);
    iupImageStockSet("IUP_PUC-Rio", load_image_PUC_Rio8, 0);
    iupImageStockSet("IUP_BR", load_image_BR8, 0);
    iupImageStockSet("IUP_Lua", load_image_Lua8, 0);
    iupImageStockSet("IUP_TecgrafPUC-Rio", load_image_TecgrafPUC_Rio8, 0);
    iupImageStockSet("IUP_Petrobras", load_image_Petrobras8, 0);
  }
  else
#endif
  {
    iupImageStockSet("IUP_Tecgraf", load_image_Tecgraf, 0);
    iupImageStockSet("IUP_PUC-Rio", load_image_PUC_Rio, 0);
    iupImageStockSet("IUP_BR", load_image_BR, 0);
    iupImageStockSet("IUP_Lua", load_image_Lua, 0);
    iupImageStockSet("IUP_TecgrafPUC-Rio", load_image_TecgrafPUC_Rio, 0);
    iupImageStockSet("IUP_Petrobras", load_image_Petrobras, 0);
  }

  /***************** Logos *****************/

#ifndef WIN32
  if (motif)
  {
    iupImageStockSet("IUP_LogoTecgraf", load_image_LogoTecgraf8, 0);
    iupImageStockSet("IUP_LogoPUC-Rio", load_image_LogoPUC_Rio8, 0);
    iupImageStockSet("IUP_LogoBR", load_image_LogoBR8, 0);
    iupImageStockSet("IUP_LogoLua", load_image_LogoLua8, 0);
    iupImageStockSet("IUP_LogoTecgrafPUC-Rio", load_image_LogoTecgrafPUC_Rio8, 0);
    iupImageStockSet("IUP_LogoPetrobras", load_image_LogoPetrobras8, 0);
  }
  else
#endif
  {
    iupImageStockSet("IUP_LogoTecgraf", load_image_LogoTecgraf, 0);
    iupImageStockSet("IUP_LogoPUC-Rio", load_image_LogoPUC_Rio, 0);
    iupImageStockSet("IUP_LogoBR", load_image_LogoBR, 0);
    iupImageStockSet("IUP_LogoLua", load_image_LogoLua, 0);
    iupImageStockSet("IUP_LogoTecgrafPUC-Rio", load_image_LogoTecgrafPUC_Rio, 0);
    iupImageStockSet("IUP_LogoPetrobras", load_image_LogoPetrobras, 0);
  }

#ifdef WIN32
  if (win32)
  {
    iupImageStockSet("IUP_DeviceCamera", load_image_DeviceCamera, 0);
    iupImageStockSet("IUP_DeviceCD", load_image_DeviceCD, 0);
    iupImageStockSet("IUP_DeviceCellPhone", load_image_DeviceCellPhone, 0);
    iupImageStockSet("IUP_DeviceComputer", load_image_DeviceComputer, 0);
    iupImageStockSet("IUP_DeviceFax", load_image_DeviceFax, 0);
    iupImageStockSet("IUP_DeviceMP3", load_image_DeviceMP3, 0);
    iupImageStockSet("IUP_DeviceNetwork", load_image_DeviceNetwork, 0);
    iupImageStockSet("IUP_DevicePDA", load_image_DevicePDA, 0);
    iupImageStockSet("IUP_DevicePrinter", load_image_DevicePrinter, 0);
    iupImageStockSet("IUP_DeviceScanner", load_image_DeviceScanner, 0);
    iupImageStockSet("IUP_DeviceSound", load_image_DeviceSound, 0);
    iupImageStockSet("IUP_DeviceVideo", load_image_DeviceVideo, 0);

    iupImageStockSet("IUP_LogoMessageError", load_image_LogoMessageError, 0);
    iupImageStockSet("IUP_LogoMessageHelp", load_image_LogoMessageHelp, 0);
    iupImageStockSet("IUP_LogoMessageInfo", load_image_LogoMessageInfo, 0);
    iupImageStockSet("IUP_LogoMessageSecurity", load_image_LogoMessageSecurity, 0);
    iupImageStockSet("IUP_LogoMessageWarning", load_image_LogoMessageWarning, 0);
  }
#endif  

  if (gtk)
  {
    iupImageStockSet("IUP_LogoMessageError", 0, "gtk-dialog-error");
    iupImageStockSet("IUP_LogoMessageHelp", 0, "gtk-dialog-question");
    iupImageStockSet("IUP_LogoMessageInfo", 0, "gtk-dialog-info");
    iupImageStockSet("IUP_LogoMessageSecurity", 0, "gtk-dialog-authentication");
    iupImageStockSet("IUP_LogoMessageWarning", 0, "gtk-dialog-warning");
  }
#endif  
}
 
void iupImageLibLoadAll(void)
{
#ifndef IUP_IMGLIB_DUMMY
  /* Bitmaps */
  iupImageStockLoad("IUP_ActionCancel");
  iupImageStockLoad("IUP_ActionOk");
  iupImageStockLoad("IUP_ArrowDown");
  iupImageStockLoad("IUP_ArrowLeft");
  iupImageStockLoad("IUP_ArrowRight");
  iupImageStockLoad("IUP_ArrowUp");
  iupImageStockLoad("IUP_EditCopy");
  iupImageStockLoad("IUP_EditCut");
  iupImageStockLoad("IUP_EditErase");
  iupImageStockLoad("IUP_EditFind");
  iupImageStockLoad("IUP_EditPaste");
  iupImageStockLoad("IUP_EditRedo");
  iupImageStockLoad("IUP_EditUndo");
  iupImageStockLoad("IUP_FileClose");
  iupImageStockLoad("IUP_FileCloseAll");
  iupImageStockLoad("IUP_FileNew");
  iupImageStockLoad("IUP_FileOpen");
  iupImageStockLoad("IUP_FileProperties");
  iupImageStockLoad("IUP_FileSave");
  iupImageStockLoad("IUP_FileSaveAll");
  iupImageStockLoad("IUP_FileText");
  iupImageStockLoad("IUP_FontBold");
  iupImageStockLoad("IUP_FontDialog");
  iupImageStockLoad("IUP_FontItalic");
  iupImageStockLoad("IUP_MediaForward");
  iupImageStockLoad("IUP_MediaGotoBegin");
  iupImageStockLoad("IUP_MediaGoToEnd");
  iupImageStockLoad("IUP_MediaPause");
  iupImageStockLoad("IUP_MediaPlay");
  iupImageStockLoad("IUP_MediaRecord");
  iupImageStockLoad("IUP_MediaReverse");
  iupImageStockLoad("IUP_MediaRewind");
  iupImageStockLoad("IUP_MediaStop");
  iupImageStockLoad("IUP_MessageError");
  iupImageStockLoad("IUP_MessageHelp");
  iupImageStockLoad("IUP_MessageInfo");
  iupImageStockLoad("IUP_NavigateHome");
  iupImageStockLoad("IUP_NavigateRefresh");
  iupImageStockLoad("IUP_Print");
  iupImageStockLoad("IUP_PrintPreview");
  iupImageStockLoad("IUP_ToolsColor");
  iupImageStockLoad("IUP_ToolsSettings");
  iupImageStockLoad("IUP_ToolsSortAscend");
  iupImageStockLoad("IUP_ToolsSortDescend");
  iupImageStockLoad("IUP_ViewFullScreen");
  iupImageStockLoad("IUP_WindowsCascade");
  iupImageStockLoad("IUP_WindowsTile");
  iupImageStockLoad("IUP_Zoom");
  iupImageStockLoad("IUP_ZoomActualSize");
  iupImageStockLoad("IUP_ZoomIn");
  iupImageStockLoad("IUP_ZoomOut");
  iupImageStockLoad("IUP_ZoomSelection");

  /* Icons */
  iupImageStockLoad("IUP_Tecgraf");
  iupImageStockLoad("IUP_PUC-Rio");
  iupImageStockLoad("IUP_BR");
  iupImageStockLoad("IUP_Lua");
  iupImageStockLoad("IUP_TecgrafPUC-Rio");
  iupImageStockLoad("IUP_Petrobras");

  /* Logos */
  iupImageStockLoad("IUP_LogoTecgraf");
  iupImageStockLoad("IUP_LogoPUC-Rio");
  iupImageStockLoad("IUP_LogoBR");
  iupImageStockLoad("IUP_LogoLua");
  iupImageStockLoad("IUP_LogoTecgrafPUC-Rio");
  iupImageStockLoad("IUP_LogoPetrobras");

#ifdef WIN32
  /* Logos - Win32 Only */
  iupImageStockLoad("IUP_DeviceCamera");
  iupImageStockLoad("IUP_DeviceCD");
  iupImageStockLoad("IUP_DeviceCellPhone");
  iupImageStockLoad("IUP_DeviceComputer");
  iupImageStockLoad("IUP_DeviceFax");
  iupImageStockLoad("IUP_DeviceMP3");
  iupImageStockLoad("IUP_DeviceNetwork");
  iupImageStockLoad("IUP_DevicePDA");
  iupImageStockLoad("IUP_DevicePrinter");
  iupImageStockLoad("IUP_DeviceScanner");
  iupImageStockLoad("IUP_DeviceSound");
  iupImageStockLoad("IUP_DeviceVideo");
#endif

  /* Logos - GTK and Win32 Only */
  iupImageStockLoad("IUP_LogoMessageError");
  iupImageStockLoad("IUP_LogoMessageHelp");
  iupImageStockLoad("IUP_LogoMessageInfo");
  iupImageStockLoad("IUP_LogoMessageSecurity");
  iupImageStockLoad("IUP_LogoMessageWarning");
#endif  
}
