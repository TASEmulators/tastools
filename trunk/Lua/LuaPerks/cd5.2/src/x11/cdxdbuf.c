/** \file
 * \brief X-Windows Double Buffer Driver
 *
 * See Copyright Notice in cd.h
 */

#include "cdx11.h"
#include "cddbuf.h"
#include <stdlib.h>
#include <stdio.h>


static void cdkillcanvas (cdCtxCanvas* ctxcanvas)
{
  cdKillImage(ctxcanvas->image_dbuffer);
  cdxKillCanvas(ctxcanvas);
}

static void cddeactivate(cdCtxCanvas* ctxcanvas)
{
  cdCanvas* canvas_dbuffer = ctxcanvas->canvas_dbuffer;
  /* this is done in the canvas_dbuffer context */
  cdCanvasDeactivate(canvas_dbuffer);
}

static void cdflush(cdCtxCanvas* ctxcanvas)
{
  int old_writemode;
  cdImage* image_dbuffer = ctxcanvas->image_dbuffer;
  cdCanvas* canvas_dbuffer = ctxcanvas->canvas_dbuffer;

  /* flush the writing in the image */
  XFlush(ctxcanvas->dpy);

  /* this is done in the canvas_dbuffer context */
  /* Flush can be affected by Origin and Clipping, but not WriteMode */
  old_writemode = cdCanvasWriteMode(canvas_dbuffer, CD_REPLACE);
  cdCanvasPutImageRect(canvas_dbuffer, image_dbuffer, 0, 0, 0, 0, 0, 0);
  cdCanvasWriteMode(canvas_dbuffer, old_writemode);
}

static void cdcreatecanvas(cdCanvas* canvas, cdCanvas* canvas_dbuffer)
{
  int w, h;
  cdCtxCanvas* ctxcanvas;
  cdImage* image_dbuffer;
  cdCtxImage* ctximage;

  cdCanvasActivate(canvas_dbuffer);
  w = canvas_dbuffer->w;
  h = canvas_dbuffer->h;
  if (w==0) w=1;
  if (h==0) h=1;

  /* this is done in the canvas_dbuffer context */
  image_dbuffer = cdCanvasCreateImage(canvas_dbuffer, w, h);
  if (!image_dbuffer) 
    return;

  ctximage = image_dbuffer->ctximage;

  /* Inicializa driver DBuffer */
  ctxcanvas = cdxCreateCanvas(canvas, ctximage->dpy, ctximage->scr, ctximage->img, ctximage->vis);
  if (!ctxcanvas)
    return;

  ctxcanvas->image_dbuffer = image_dbuffer;
  ctxcanvas->canvas_dbuffer = canvas_dbuffer;
}

static int cdactivate(cdCtxCanvas* ctxcanvas)
{
  int w, h;
  cdCanvas* canvas_dbuffer = ctxcanvas->canvas_dbuffer;

  /* this is done in the canvas_dbuffer context */
  /* this will update canvas size */
  cdCanvasActivate(canvas_dbuffer);
  w = canvas_dbuffer->w;
  h = canvas_dbuffer->h;
  if (w==0) w=1;
  if (h==0) h=1;

  /* check if the size changed */
  if (w != ctxcanvas->image_dbuffer->w ||
      h != ctxcanvas->image_dbuffer->h)
  {
    cdCanvas* canvas = ctxcanvas->canvas;
    /* save the current, if the rebuild fail */
    cdImage* old_image_dbuffer = ctxcanvas->image_dbuffer;
    cdCtxCanvas* old_ctxcanvas = ctxcanvas;

    /* if the image is rebuild, the canvas that uses the image must be also rebuild */

    /* rebuild the image and the canvas */
    canvas->ctxcanvas = NULL;
    canvas->context->cxCreateCanvas(canvas, canvas_dbuffer);
    if (!canvas->ctxcanvas)
    {
      canvas->ctxcanvas = old_ctxcanvas;
      return CD_ERROR;
    }

    /* remove the old image and canvas */
    cdKillImage(old_image_dbuffer);
    cdxKillCanvas(old_ctxcanvas);

    ctxcanvas = canvas->ctxcanvas;

    /* update canvas attributes */
    canvas->cxBackground(ctxcanvas, canvas->background);
    canvas->cxForeground(ctxcanvas, canvas->foreground);
    canvas->cxBackOpacity(ctxcanvas, canvas->back_opacity);
    canvas->cxWriteMode(ctxcanvas, canvas->write_mode);
    canvas->cxLineStyle(ctxcanvas, canvas->line_style);
    canvas->cxLineWidth(ctxcanvas, canvas->line_width);
    canvas->cxLineCap(ctxcanvas, canvas->line_cap);
    canvas->cxLineJoin(ctxcanvas, canvas->line_join);
    canvas->cxHatch(ctxcanvas, canvas->hatch_style);
    if (canvas->stipple) canvas->cxStipple(ctxcanvas, canvas->stipple_w, canvas->stipple_h, canvas->stipple);
    if (canvas->pattern) canvas->cxPattern(ctxcanvas, canvas->pattern_w, canvas->pattern_h, canvas->pattern);
    canvas->cxInteriorStyle(ctxcanvas, canvas->interior_style);
    if (canvas->native_font[0] == 0) canvas->cxFont(ctxcanvas, canvas->font_type_face, canvas->font_style, canvas->font_size);
    else canvas->cxNativeFont(ctxcanvas, canvas->native_font);
/*    canvas->cxTextAlignment(ctxcanvas, canvas->text_alignment);     */
/*    canvas->cxTextOrientation(ctxcanvas, canvas->text_orientation); */
    if (canvas->clip_mode == CD_CLIPAREA && canvas->cxClipArea) canvas->cxClipArea(ctxcanvas, canvas->clip_rect.xmin, canvas->clip_rect.xmax, canvas->clip_rect.ymin, canvas->clip_rect.ymax);
/*    if (canvas->clip_mode == CD_CLIPAREA && canvas->cxFClipArea) canvas->cxFClipArea(ctxcanvas, canvas->clip_frect.xmin, canvas->clip_frect.xmax, canvas->clip_frect.ymin, canvas->clip_frect.ymax); */
    if (canvas->clip_mode == CD_CLIPPOLYGON && canvas->clip_poly) canvas->cxPoly(ctxcanvas, CD_CLIP, canvas->clip_poly, canvas->clip_poly_n);
/*    if (canvas->clip_mode == CD_CLIPPOLYGON && canvas->clip_fpoly) canvas->cxFPoly(ctxcanvas, CD_CLIP, canvas->clip_fpoly, canvas->clip_poly_n); */
    if (canvas->clip_mode != CD_CLIPOFF) canvas->cxClip(ctxcanvas, canvas->clip_mode);
  }

  return CD_OK;
}

static void cdinittable(cdCanvas* canvas)
{
  cdxInitTable(canvas);

  canvas->cxActivate = cdactivate;
  canvas->cxDeactivate = cddeactivate;
  canvas->cxFlush = cdflush;
  canvas->cxKillCanvas = cdkillcanvas;
}

static cdContext cdDBufferContext =
{
  CD_CAP_ALL & ~(CD_CAP_PLAY | CD_CAP_YAXIS | 
                 CD_CAP_FPRIMTIVES ),
  0,
  cdcreatecanvas,  
  cdinittable,
  NULL,             
  NULL, 
};

cdContext* cdContextDBuffer(void)
{
  if (cdUseContextPlus(CD_QUERY))
  {
    cdContext* ctx = cdGetContextPlus(CD_CTX_DBUFFER);
    if (ctx != NULL)
      return ctx;
  }

  return &cdDBufferContext;
}
