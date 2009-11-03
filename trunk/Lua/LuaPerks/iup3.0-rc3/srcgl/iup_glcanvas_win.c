/** \file
 * \brief iupgl control for Windows
 *
 * See Copyright Notice in "iup.h"
 */

#include <windows.h>
#include <GL/gl.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <memory.h>

#include "iup.h"
#include "iupcbs.h"
#include "iupgl.h"

#include "iup_object.h"
#include "iup_attrib.h"
#include "iup_str.h"
#include "iup_stdcontrols.h"
#include "iup_assert.h"
#include "iup_register.h"


struct _IcontrolData 
{
  iupCanvas canvas;  /* from IupCanvas (must reserve it) */
  HWND window;
  HDC device;
  HGLRC context;
  HPALETTE palette;
  int is_owned_dc;
};

static int wGLCanvasDefaultResize_CB(Ihandle *ih, int width, int height)
{
  IupGLMakeCurrent(ih);
  glViewport(0,0,width,height);
  return IUP_DEFAULT;
}

static int wGLCanvasCreateMethod(Ihandle* ih, void** params)
{
  (void)params;
  free(ih->data); /* allocated by the iCanvasCreateMethod of IupCanvas */
  ih->data = iupALLOCCTRLDATA();
  IupSetCallback(ih, "RESIZE_CB", (Icallback)wGLCanvasDefaultResize_CB);
  return IUP_NOERROR;
}

static int wGLCreateContext(Ihandle* ih)
{
  Ihandle* ih_shared;
  int number;
  int isIndex = 0;
  int pixelFormat;
  PIXELFORMATDESCRIPTOR test_pfd;
  PIXELFORMATDESCRIPTOR pfd = { 
    sizeof(PIXELFORMATDESCRIPTOR),  /*  size of this pfd   */
      1,                     /* version number             */
      PFD_DRAW_TO_WINDOW |   /* support window             */
      PFD_SUPPORT_OPENGL,    /* support OpenGL             */
      PFD_TYPE_RGBA,         /* RGBA type                  */
      24,                    /* 24-bit color depth         */
      0, 0, 0, 0, 0, 0,      /* color bits ignored         */
      0,                     /* no alpha buffer            */
      0,                     /* shift bit ignored          */
      0,                     /* no accumulation buffer     */
      0, 0, 0, 0,            /* accum bits ignored         */
      16,                    /* 32-bit z-buffer             */
      0,                     /* no stencil buffer          */
      0,                     /* no auxiliary buffer        */
      PFD_MAIN_PLANE,        /* main layer                 */
      0,                     /* reserved                   */
      0, 0, 0                /* layer masks ignored        */
  };

  /* the IupCanvas is already mapped, just initialize the OpenGL context */

  /* double or single buffer */
  if (iupStrEqualNoCase(iupAttribGetStr(ih,"BUFFER"), "DOUBLE"))
    pfd.dwFlags |= PFD_DOUBLEBUFFER;

  /* stereo */
  if (iupAttribGetBoolean(ih,"STEREO"))
    pfd.dwFlags |= PFD_STEREO;

  /* rgba or index */ 
  if (iupStrEqualNoCase(iupAttribGetStr(ih,"COLOR"), "INDEX"))
  {
    isIndex = 1;
    pfd.iPixelType = PFD_TYPE_COLORINDEX;
    pfd.cColorBits = 8;  /* assume 8 bits when indexed */
    number = iupAttribGetInt(ih,"BUFFER_SIZE");
    if (number > 0) pfd.cColorBits = (BYTE)number;
  }

  /* red, green, blue bits */
  number = iupAttribGetInt(ih,"RED_SIZE");
  if (number > 0) pfd.cRedBits = (BYTE)number;
  pfd.cRedShift = 0;

  number = iupAttribGetInt(ih,"GREEN_SIZE");
  if (number > 0) pfd.cGreenBits = (BYTE)number;
  pfd.cGreenShift = pfd.cRedBits;

  number = iupAttribGetInt(ih,"BLUE_SIZE");
  if (number > 0) pfd.cBlueBits = (BYTE)number;
  pfd.cBlueShift = pfd.cRedBits + pfd.cGreenBits;

  number = iupAttribGetInt(ih,"ALPHA_SIZE");
  if (number > 0) pfd.cAlphaBits = (BYTE)number;
  pfd.cAlphaShift = pfd.cRedBits + pfd.cGreenBits + pfd.cBlueBits;

  /* depth and stencil size */
  number = iupAttribGetInt(ih,"DEPTH_SIZE");
  if (number > 0) pfd.cDepthBits = (BYTE)number;

  /* stencil */
  number = iupAttribGetInt(ih,"STENCIL_SIZE");
  if (number > 0) pfd.cStencilBits = (BYTE)number;

  /* red, green, blue accumulation bits */
  number = iupAttribGetInt(ih,"ACCUM_RED_SIZE");
  if (number > 0) pfd.cAccumRedBits = (BYTE)number;

  number = iupAttribGetInt(ih,"ACCUM_GREEN_SIZE");
  if (number > 0) pfd.cAccumGreenBits = (BYTE)number;

  number = iupAttribGetInt(ih,"ACCUM_BLUE_SIZE");
  if (number > 0) pfd.cAccumBlueBits = (BYTE)number;

  number = iupAttribGetInt(ih,"ACCUM_ALPHA_SIZE");
  if (number > 0) pfd.cAccumAlphaBits = (BYTE)number;

  pfd.cAccumBits = pfd.cAccumRedBits + pfd.cAccumGreenBits + pfd.cAccumBlueBits + pfd.cAccumAlphaBits;

  /* get a device context */
  {
    LONG style = GetClassLong(ih->data->window, GCL_STYLE);
    ih->data->is_owned_dc = (int) ((style & CS_OWNDC) || (style & CS_CLASSDC));
  }

  ih->data->device = GetDC(ih->data->window);
  iupAttribSetStr(ih, "VISUAL", (char*)ih->data->device);

  /* choose pixel format */
  pixelFormat = ChoosePixelFormat(ih->data->device, &pfd);
  if (pixelFormat == 0)
  {
    iupAttribSetStr(ih, "ERROR", "No appropriate pixel format.");
    return IUP_NOERROR;
  } 
  SetPixelFormat(ih->data->device,pixelFormat,&pfd);

  /* create rendering context */
  ih->data->context = wglCreateContext(ih->data->device);
  if (!ih->data->context)
  {
    iupAttribSetStr(ih, "ERROR", "Could not create a rendering context.");
    return IUP_NOERROR;
  }
  iupAttribSetStr(ih, "CONTEXT", (char*)ih->data->context);

  ih_shared = IupGetAttributeHandle(ih, "SHAREDCONTEXT");
  if (ih_shared && iupStrEqual(ih_shared->iclass->name, "glcanvas"))  /* must be an IupGLCanvas */
    wglShareLists(ih_shared->data->context, ih->data->context);

  DescribePixelFormat(ih->data->device, pixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &test_pfd);
  if ((pfd.dwFlags & PFD_STEREO) && !(test_pfd.dwFlags & PFD_STEREO))
  {
    iupAttribSetStr(ih, "ERROR", "Stereo not available.");
    return IUP_NOERROR;
  }

  /* create colormap for index mode */
  if (isIndex)
  {
    if (!ih->data->palette)
    {
      LOGPALETTE lp = {0x300,1,{255,255,255,PC_NOCOLLAPSE}};  /* set first color as white */
      ih->data->palette = CreatePalette(&lp);
      ResizePalette(ih->data->palette,1<<pfd.cColorBits);
      iupAttribSetStr(ih, "COLORMAP", (char*)ih->data->palette);
    }

    SelectPalette(ih->data->device,ih->data->palette,FALSE);
    RealizePalette(ih->data->device);
  }

  return IUP_NOERROR;
}

static int wGLCanvasMapMethod(Ihandle* ih)
{
  /* get a device context */
  ih->data->window = (HWND)iupAttribGet(ih, "HWND"); /* check first in the hash table, can be defined by the IupFileDlg */
  if (!ih->data->window)
    ih->data->window = (HWND)IupGetAttribute(ih, "HWND");  /* works for Win32 and GTK, only after mapping the IupCanvas */
  if (!ih->data->window)
    return IUP_NOERROR;

  {
    LONG style = GetClassLong(ih->data->window, GCL_STYLE);
    ih->data->is_owned_dc = (int) ((style & CS_OWNDC) || (style & CS_CLASSDC));
  }

  return wGLCreateContext(ih);
}

static void wGLCanvasUnMapMethod(Ihandle* ih)
{
  if (ih->data->context)
  {
    if (ih->data->context == wglGetCurrentContext())
      wglMakeCurrent(NULL, NULL);

    wglDeleteContext(ih->data->context);
  }

  if (ih->data->palette)
    DeleteObject((HGDIOBJ)ih->data->palette);

  if (ih->data->device)
    ReleaseDC(ih->data->window, ih->data->device);
}

static int wGLCanvasSetRefreshContextAttrib(Ihandle* ih, const char* value)
{
  if (!ih->data->is_owned_dc)
  {
    if (ih->data->context)
    {
      if (ih->data->context == wglGetCurrentContext())
        wglMakeCurrent(NULL, NULL);

      wglDeleteContext(ih->data->context);
    }

    if (ih->data->device)
      ReleaseDC(ih->data->window, ih->data->device);

    wGLCreateContext(ih);
  }

  (void)value;
  return 0;
}

static Iclass* wGlCanvasGetClass(void)
{
  Iclass* ic = iupClassNew(iupCanvasGetClass());

  ic->name = "glcanvas";
  ic->format = "A"; /* one optional callback name */
  ic->nativetype = IUP_TYPECANVAS;
  ic->childtype = IUP_CHILDNONE;
  ic->is_interactive = 1;

  ic->Create = wGLCanvasCreateMethod;
  ic->Map = wGLCanvasMapMethod;
  ic->UnMap = wGLCanvasUnMapMethod;

  iupClassRegisterAttribute(ic, "BUFFER", NULL, NULL, IUPAF_SAMEASSYSTEM, "SINGLE", IUPAF_DEFAULT);
  iupClassRegisterAttribute(ic, "COLOR", NULL, NULL, IUPAF_SAMEASSYSTEM, "RGBA", IUPAF_DEFAULT);

  iupClassRegisterAttribute(ic, "CONTEXT", NULL, NULL, NULL, NULL, IUPAF_READONLY|IUPAF_NO_STRING);
  iupClassRegisterAttribute(ic, "VISUAL", NULL, NULL, NULL, NULL, IUPAF_READONLY|IUPAF_NO_STRING);
  iupClassRegisterAttribute(ic, "COLORMAP", NULL, NULL, NULL, NULL, IUPAF_READONLY|IUPAF_NO_STRING);

  iupClassRegisterAttribute(ic, "REFRESHCONTEXT", NULL, wGLCanvasSetRefreshContextAttrib, NULL, NULL, IUPAF_WRITEONLY|IUPAF_NO_INHERIT);

  return ic;
}

/******************************************* Exported functions */

void IupGLCanvasOpen(void)
{
  if (!IupGetGlobal("_IUP_GLCANVAS_OPEN"))
  {
    iupRegisterClass(wGlCanvasGetClass());
    IupSetGlobal("_IUP_GLCANVAS_OPEN", "1");
  }
}

Ihandle* IupGLCanvas(const char *action)
{
  void *params[2];
  params[0] = (void*)action;
  params[1] = NULL;
  return IupCreatev("glcanvas", params);
}

int IupGLIsCurrent(Ihandle* ih)
{
  iupASSERT(iupObjectCheck(ih));
  if (!iupObjectCheck(ih))
    return 0;

  /* must be an IupGLCanvas */
  if (!iupStrEqual(ih->iclass->name, "glcanvas"))
    return 0;

  /* must be mapped */
  if (!ih->data->window)
    return 0;

  if (ih->data->context == wglGetCurrentContext())
    return 1;

  return 0;
}

void IupGLMakeCurrent(Ihandle* ih)
{
  iupASSERT(iupObjectCheck(ih));
  if (!iupObjectCheck(ih))
    return;

  /* must be an IupGLCanvas */
  if (!iupStrEqual(ih->iclass->name, "glcanvas"))
    return;

  /* must be mapped */
  if (!ih->data->window)
    return;

  wglMakeCurrent(ih->data->device, ih->data->context);
}

void IupGLSwapBuffers(Ihandle* ih)
{
  iupASSERT(iupObjectCheck(ih));
  if (!iupObjectCheck(ih))
    return;

  /* must be an IupGLCanvas */
  if (!iupStrEqual(ih->iclass->name, "glcanvas"))
    return;

  /* must be mapped */
  if (!ih->data->window)
    return;

  SwapBuffers(ih->data->device);
}

void IupGLPalette(Ihandle* ih, int index, float r, float g, float b)
{
  iupASSERT(iupObjectCheck(ih));
  if (!iupObjectCheck(ih))
    return;

  /* must be an IupGLCanvas */
  if (!iupStrEqual(ih->iclass->name, "glcanvas"))
    return;

  /* must be mapped */
  if (!ih->data->window)
    return;

  /* must have a palette */
  if (ih->data->palette)
  {
    PALETTEENTRY entry;
    entry.peRed    = (BYTE)(r*255);
    entry.peGreen  = (BYTE)(g*255);
    entry.peBlue   = (BYTE)(b*255);
    entry.peFlags  = PC_NOCOLLAPSE;
    SetPaletteEntries(ih->data->palette,index,1,&entry);
    UnrealizeObject(ih->data->device);
    SelectPalette(ih->data->device,ih->data->palette,FALSE);
    RealizePalette(ih->data->device);
  }
}

void IupGLUseFont(Ihandle* ih, int first, int count, int list_base)
{
  HFONT old_font, font;

  iupASSERT(iupObjectCheck(ih));
  if (!iupObjectCheck(ih))
    return;

  /* must be an IupGLCanvas */
  if (!iupStrEqual(ih->iclass->name, "glcanvas"))
    return;

  /* must be mapped */
  if (!ih->data->window)
    return;

  font = (HFONT)IupGetAttribute(ih, "HFONT");
  if (font)
  {
    old_font = SelectObject(ih->data->device, font);
    wglUseFontBitmaps(ih->data->device, first, count, list_base);
    SelectObject(ih->data->device, old_font);
  }
}

void IupGLWait(int gl)
{
  if (gl)
    glFinish();
  else
    GdiFlush();
}
