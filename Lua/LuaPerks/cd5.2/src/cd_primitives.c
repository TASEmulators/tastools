/** \file
 * \brief External API - Primitives
 *
 * See Copyright Notice in cd.h
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <assert.h>
#include <memory.h>
#include <math.h>


#include "cd.h"
#include "cd_private.h"

#define _CD_POLY_BLOCK 100

void cdCanvasPixel(cdCanvas* canvas, int x, int y, long color)
{
  assert(canvas);
  if (!_cdCheckCanvas(canvas)) return;

  if (canvas->use_origin)
  {
    x += canvas->origin.x;
    y += canvas->origin.y;
  }

  if (canvas->invert_yaxis)
    y = _cdInvertYAxis(canvas, y);

  canvas->cxPixel(canvas->ctxcanvas, x, y, color);
}

void cdCanvasMark(cdCanvas* canvas, int x, int y)
{
  assert(canvas);
  if (!_cdCheckCanvas(canvas)) return;

  if (canvas->mark_size == 1)
  {
    cdCanvasPixel(canvas, x, y, canvas->foreground);
    return;
  }

  if (canvas->use_origin)
  {
    x += canvas->origin.x;
    y += canvas->origin.y;
  }

  if (canvas->invert_yaxis)
    y = _cdInvertYAxis(canvas, y);

  cdSimMark(canvas, x, y);
}

void cdCanvasLine(cdCanvas* canvas, int x1, int y1, int x2, int y2)
{
  assert(canvas);
  if (!_cdCheckCanvas(canvas)) return;

  if (x1 == x2 && y1 == y2)
  {
    cdCanvasPixel(canvas, x1, y1, canvas->foreground);
    return;
  }
    
  if (canvas->use_origin)
  {
    x1 += canvas->origin.x;
    y1 += canvas->origin.y;
    x2 += canvas->origin.x;
    y2 += canvas->origin.y;
  }

  if (canvas->invert_yaxis)
  {
    y1 = _cdInvertYAxis(canvas, y1);
    y2 = _cdInvertYAxis(canvas, y2);
  }

  canvas->cxLine(canvas->ctxcanvas, x1, y1, x2, y2);
}

void cdfCanvasLine(cdCanvas* canvas, double x1, double y1, double x2, double y2)
{
  assert(canvas);
  if (!_cdCheckCanvas(canvas)) return;

  if (x1 == x2 && y1 == y2)
  {
    cdCanvasPixel(canvas, _cdRound(x1), _cdRound(y1), canvas->foreground);
    return;
  }
    
  if (canvas->use_origin)
  {
    x1 += canvas->forigin.x;
    y1 += canvas->forigin.y;
    x2 += canvas->forigin.x;
    y2 += canvas->forigin.y;
  }

  if (canvas->invert_yaxis)
  {
    y1 = _cdInvertYAxis(canvas, y1);
    y2 = _cdInvertYAxis(canvas, y2);
  }

  if (canvas->cxFLine)
    canvas->cxFLine(canvas->ctxcanvas, x1, y1, x2, y2);
  else
    canvas->cxLine(canvas->ctxcanvas, _cdRound(x1), _cdRound(y1), _cdRound(x2), _cdRound(y2));
}

void cdCanvasBegin(cdCanvas* canvas, int mode)
{
  assert(canvas);
  assert(mode>=CD_FILL);
  if (!_cdCheckCanvas(canvas)) return;

  if (mode == CD_REGION)
  {
    if (!canvas->cxNewRegion) return;

    canvas->new_region = 1;
    canvas->poly_n = 0;
    canvas->cxNewRegion(canvas->ctxcanvas);
    return;
  }

  canvas->sim_poly = 0;

  if (canvas->interior_style == CD_HOLLOW && mode == CD_FILL)
    mode = CD_CLOSED_LINES;

  /* simulacao de linhas */
  if ((mode == CD_CLOSED_LINES || mode == CD_OPEN_LINES || mode == CD_BEZIER) && 
      canvas->sim_mode & CD_SIM_POLYLINE)
    canvas->sim_poly = 1;

  /* simulacao de poligonos preenchidos */
  if (mode == CD_FILL && canvas->sim_mode & CD_SIM_POLYGON)
    canvas->sim_poly = 1;

  canvas->use_fpoly = -1;
  canvas->poly_n = 0;
  canvas->poly_mode = mode;
}

void cdCanvasVertex(cdCanvas* canvas, int x, int y)
{
  assert(canvas);
  if (!_cdCheckCanvas(canvas)) return;
  if (canvas->use_fpoly == 1) return; /* integer vertex inside a real poligon */

  if (!canvas->poly)
  {
    canvas->poly = (cdPoint*)malloc(sizeof(cdPoint)*(_CD_POLY_BLOCK+1)); /* always add 1 so we add another point at the end if necessary */
    canvas->poly_size = _CD_POLY_BLOCK;
  }

  canvas->use_fpoly = 0;

  if (canvas->use_origin)
  {
    x += canvas->origin.x;
    y += canvas->origin.y;
  }

  if (canvas->invert_yaxis)
    y = _cdInvertYAxis(canvas, y);

  if (canvas->poly_n == canvas->poly_size)
  {
    canvas->poly_size += _CD_POLY_BLOCK;
    canvas->poly = (cdPoint*)realloc(canvas->poly, sizeof(cdPoint) * (canvas->poly_size+1));
  }

  if (canvas->poly_mode != CD_BEZIER &&
      canvas->poly_n > 0 && 
      canvas->poly[canvas->poly_n-1].x == x && 
      canvas->poly[canvas->poly_n-1].y == y)
    return;  /* avoid duplicate points, if not a bezier */

  canvas->poly[canvas->poly_n].x = x;
  canvas->poly[canvas->poly_n].y = y;
  canvas->poly_n++;
}

void cdfCanvasVertex(cdCanvas* canvas, double x, double y)
{
  assert(canvas);
  if (!_cdCheckCanvas(canvas)) return;

  if (!canvas->cxFPoly)
  {
    cdCanvasVertex(canvas, _cdRound(x), _cdRound(y));
    return;
  }

  if (canvas->use_fpoly == 0) return; /* real vertex inside a integer poligon */

  if (!canvas->fpoly)
  {
    canvas->fpoly = (cdfPoint*)malloc(sizeof(cdfPoint)*(_CD_POLY_BLOCK+1));
    canvas->fpoly_size = _CD_POLY_BLOCK;
  }

  canvas->use_fpoly = 1;

  if (canvas->use_origin)
  {
    x += canvas->forigin.x;
    y += canvas->forigin.y;
  }

  if (canvas->invert_yaxis)
    y = _cdInvertYAxis(canvas, y);

  if (canvas->poly_n == canvas->fpoly_size)
  {
    canvas->fpoly_size += _CD_POLY_BLOCK;
    canvas->fpoly = (cdfPoint*)realloc(canvas->fpoly, sizeof(cdfPoint) * (canvas->fpoly_size+1));
  }

  canvas->fpoly[canvas->poly_n].x = x;
  canvas->fpoly[canvas->poly_n].y = y;
  canvas->poly_n++;
}

void cdCanvasEnd(cdCanvas* canvas)
{
  assert(canvas);
  if (!_cdCheckCanvas(canvas)) return;

  if (canvas->new_region && canvas->poly_n == 0)
  {
    canvas->new_region = 0;
    if (canvas->clip_mode == CD_CLIPREGION) cdCanvasClip(canvas, CD_CLIPREGION);
    return;
  }

  if (canvas->poly_mode==CD_OPEN_LINES && canvas->poly_n < 2)
  {
    canvas->poly_n = 0;
    return;
  }

  if (canvas->poly_mode==CD_BEZIER && (canvas->poly_n < 4 || ((canvas->poly_n-4)%3 != 0)))
  {
    canvas->poly_n = 0;
    return;
  }

  if ((canvas->poly_mode == CD_CLOSED_LINES ||
       canvas->poly_mode == CD_FILL ||
       canvas->poly_mode == CD_CLIP) && canvas->poly_n < 3)
  {
    canvas->poly_n = 0;
    return;
  }

  if (canvas->sim_poly)
    cdpolySIM(canvas->ctxcanvas, canvas->poly_mode, canvas->poly, canvas->poly_n);
  else
  {
    if (canvas->use_fpoly)
      canvas->cxFPoly(canvas->ctxcanvas, canvas->poly_mode, canvas->fpoly, canvas->poly_n);
    else
      canvas->cxPoly(canvas->ctxcanvas, canvas->poly_mode, canvas->poly, canvas->poly_n);
  }

  if (canvas->poly_mode == CD_CLIP)
  {
     canvas->clip_poly_n = canvas->poly_n;

    if (canvas->clip_fpoly) 
    {
      free(canvas->clip_fpoly);
      canvas->clip_fpoly = NULL;
    }

    if (canvas->clip_poly) 
    {
      free(canvas->clip_poly);
      canvas->clip_poly = NULL;
    }

    if (canvas->use_fpoly)
    {
      canvas->clip_fpoly = (cdfPoint*)malloc((canvas->poly_n+1) * sizeof(cdfPoint));
      memcpy(canvas->clip_fpoly, canvas->fpoly, canvas->poly_n * sizeof(cdfPoint));
    }
    else
    {
      canvas->clip_poly = (cdPoint*)malloc((canvas->poly_n+1) * sizeof(cdPoint));
      memcpy(canvas->clip_poly, canvas->poly, canvas->poly_n * sizeof(cdPoint));
    }
  }

  canvas->poly_n = 0;
  canvas->use_fpoly = -1;
}

void cdCanvasRect(cdCanvas* canvas, int xmin, int xmax, int ymin, int ymax)
{
  assert(canvas);
  if (!_cdCheckCanvas(canvas)) return;

  if (!cdCheckBoxSize(&xmin, &xmax, &ymin, &ymax))
    return;

  if (canvas->use_origin)
  {
    xmin += canvas->origin.x;
    xmax += canvas->origin.x;
    ymin += canvas->origin.y;
    ymax += canvas->origin.y;
  }

  if (canvas->invert_yaxis)
  {
    ymin = _cdInvertYAxis(canvas, ymin);
    ymax = _cdInvertYAxis(canvas, ymax);
    _cdSwapInt(ymin, ymax);
  }

  canvas->cxRect(canvas->ctxcanvas, xmin, xmax, ymin, ymax);
}

void cdfCanvasRect(cdCanvas* canvas, double xmin, double xmax, double ymin, double ymax)
{
  assert(canvas);
  if (!_cdCheckCanvas(canvas)) return;

  if (!cdfCheckBoxSize(&xmin, &xmax, &ymin, &ymax))
    return;

  if (canvas->use_origin)
  {
    xmin += canvas->origin.x;
    xmax += canvas->origin.x;
    ymin += canvas->origin.y;
    ymax += canvas->origin.y;
  }

  if (canvas->invert_yaxis)
  {
    ymin = _cdInvertYAxis(canvas, ymin);
    ymax = _cdInvertYAxis(canvas, ymax);
    _cdSwapDouble(ymin, ymax);
  }

  if (canvas->cxFRect)
    canvas->cxFRect(canvas->ctxcanvas, xmin, xmax, ymin, ymax);
  else
    canvas->cxRect(canvas->ctxcanvas, _cdRound(xmin), _cdRound(xmax), _cdRound(ymin), _cdRound(ymax));
}

void cdCanvasBox(cdCanvas* canvas, int xmin, int xmax, int ymin, int ymax)
{
  assert(canvas);
  if (!_cdCheckCanvas(canvas)) return;

  if (canvas->interior_style == CD_HOLLOW)
  {
    cdCanvasRect(canvas, xmin, xmax, ymin, ymax);
    return;
  }

  if (!cdCheckBoxSize(&xmin, &xmax, &ymin, &ymax))
    return;

  if (canvas->use_origin)
  {
    xmin += canvas->origin.x;
    xmax += canvas->origin.x;
    ymin += canvas->origin.y;
    ymax += canvas->origin.y;
  }

  if (canvas->invert_yaxis)
  {
    ymin = _cdInvertYAxis(canvas, ymin);
    ymax = _cdInvertYAxis(canvas, ymax);
    _cdSwapInt(ymin, ymax);
  }

  canvas->cxBox(canvas->ctxcanvas, xmin, xmax, ymin, ymax);
}

void cdfCanvasBox(cdCanvas* canvas, double xmin, double xmax, double ymin, double ymax)
{
  assert(canvas);
  if (!_cdCheckCanvas(canvas)) return;

  if (canvas->interior_style == CD_HOLLOW)
  {
    cdfCanvasRect(canvas, xmin, xmax, ymin, ymax);
    return;
  }

  if (!cdfCheckBoxSize(&xmin, &xmax, &ymin, &ymax))
    return;

  if (canvas->use_origin)
  {
    xmin += canvas->forigin.x;
    xmax += canvas->forigin.x;
    ymin += canvas->forigin.y;
    ymax += canvas->forigin.y;
  }

  if (canvas->invert_yaxis)
  {
    ymin = _cdInvertYAxis(canvas, ymin);
    ymax = _cdInvertYAxis(canvas, ymax);
    _cdSwapDouble(ymin, ymax);
  }

  if (canvas->cxFBox)
    canvas->cxFBox(canvas->ctxcanvas, xmin, xmax, ymin, ymax);
  else
    canvas->cxBox(canvas->ctxcanvas, _cdRound(xmin), _cdRound(xmax), _cdRound(ymin), _cdRound(ymax));
}

static void normAngles(double *angle1, double *angle2)
{
  *angle1 = fmod(*angle1,360);
  *angle2 = fmod(*angle2,360);
  if (*angle2 <= *angle1) *angle2 += 360;
}

void cdCanvasArc(cdCanvas* canvas, int xc, int yc, int w, int h, double angle1, double angle2)
{
  assert(canvas);
  if (!_cdCheckCanvas(canvas)) return;

  if (angle1 == angle2 || w == 0 || h == 0)
    return;

  normAngles(&angle1, &angle2);

  if (canvas->use_origin)
  {
    xc += canvas->origin.x;
    yc += canvas->origin.y;
  }

  if (canvas->invert_yaxis)
    yc = _cdInvertYAxis(canvas, yc);

  canvas->cxArc(canvas->ctxcanvas, xc, yc, w, h, angle1, angle2);
}

void cdfCanvasArc(cdCanvas* canvas, double xc, double yc, double w, double h, double angle1, double angle2)
{
  assert(canvas);
  if (!_cdCheckCanvas(canvas)) return;

  if (angle1 == angle2 || w == 0 || h == 0)
    return;

  normAngles(&angle1, &angle2);

  if (canvas->use_origin)
  {
    xc += canvas->forigin.x;
    yc += canvas->forigin.y;
  }

  if (canvas->invert_yaxis)
    yc = _cdInvertYAxis(canvas, yc);


  if (canvas->cxFArc)
    canvas->cxFArc(canvas->ctxcanvas, xc, yc, w, h, angle1, angle2);
  else
    canvas->cxArc(canvas->ctxcanvas, _cdRound(xc), _cdRound(yc), _cdRound(w), _cdRound(h), angle1, angle2);
}

void cdCanvasSector(cdCanvas* canvas, int xc, int yc, int w, int h, double angle1, double angle2)
{
  assert(canvas);
  if (!_cdCheckCanvas(canvas)) return;

  if (angle1 == angle2 || w == 0 || h == 0)
    return;

  normAngles(&angle1, &angle2);

  if (canvas->interior_style == CD_HOLLOW)
  {
    cdCanvasArc(canvas, xc, yc, w, h, angle1, angle2);

    if (fabs(angle2-angle1) < 360)
    {
      int xi,yi,xf,yf;
      
      xi = xc + cdRound(w*cos(CD_DEG2RAD*angle1)/2.0);
      yi = yc + cdRound(h*sin(CD_DEG2RAD*angle1)/2.0);
      
      xf = xc + cdRound(w*cos(CD_DEG2RAD*angle2)/2.0);
      yf = yc + cdRound(h*sin(CD_DEG2RAD*angle2)/2.0);

      cdCanvasLine(canvas, xi, yi, xc, yc);
      cdCanvasLine(canvas, xc, yc, xf, yf);
    }

    return;
  }

  if (canvas->use_origin)
  {
    xc += canvas->origin.x;
    yc += canvas->origin.y;
  }

  if (canvas->invert_yaxis)
    yc = _cdInvertYAxis(canvas, yc);

  canvas->cxSector(canvas->ctxcanvas, xc, yc, w, h, angle1, angle2);
}

void cdfCanvasSector(cdCanvas* canvas, double xc, double yc, double w, double h, double angle1, double angle2)
{
  assert(canvas);
  if (!_cdCheckCanvas(canvas)) return;

  if (angle1 == angle2 || w == 0 || h == 0)
    return;

  normAngles(&angle1, &angle2);

  if (canvas->interior_style == CD_HOLLOW)
  {
    cdfCanvasArc(canvas, xc, yc, w, h, angle1, angle2);

    if (fabs(angle2-angle1) < 360)
    {
      double xi,yi,xf,yf;
      
      xi = xc + w*cos(CD_DEG2RAD*angle1)/2.0;
      yi = yc + h*sin(CD_DEG2RAD*angle1)/2.0;
      
      xf = xc + w*cos(CD_DEG2RAD*angle2)/2.0;
      yf = yc + h*sin(CD_DEG2RAD*angle2)/2.0;

      cdfCanvasLine(canvas, xi, yi, xc, yc);
      cdfCanvasLine(canvas, xc, yc, xf, yf);
    }

    return;
  }

  if (canvas->use_origin)
  {
    xc += canvas->forigin.x;
    yc += canvas->forigin.y;
  }

  if (canvas->invert_yaxis)
    yc = _cdInvertYAxis(canvas, yc);

  if (canvas->cxFSector)
    canvas->cxFSector(canvas->ctxcanvas, xc, yc, w, h, angle1, angle2);
  else
    canvas->cxSector(canvas->ctxcanvas, _cdRound(xc), _cdRound(yc), _cdRound(w), _cdRound(h), angle1, angle2);
}

void cdCanvasChord(cdCanvas* canvas, int xc, int yc, int w, int h, double angle1, double angle2)
{
  assert(canvas);
  if (!_cdCheckCanvas(canvas)) return;

  if (angle1 == angle2 || w == 0 || h == 0)
    return;

  normAngles(&angle1, &angle2);

  if (canvas->interior_style == CD_HOLLOW)
  {
    int xi,yi,xf,yf;
    
    xi = xc + cdRound(w*cos(CD_DEG2RAD*angle1)/2.0);
    yi = yc + cdRound(h*sin(CD_DEG2RAD*angle1)/2.0);
    
    xf = xc + cdRound(w*cos(CD_DEG2RAD*angle2)/2.0);
    yf = yc + cdRound(h*sin(CD_DEG2RAD*angle2)/2.0);

    cdCanvasArc(canvas, xc, yc, w, h, angle1, angle2);

    if (fabs(angle2-angle1) < 360)
      cdCanvasLine(canvas, xi, yi, xf, yf);

    return;
  }

  if (canvas->use_origin)
  {
    xc += canvas->origin.x;
    yc += canvas->origin.y;
  }

  if (canvas->invert_yaxis)
    yc = _cdInvertYAxis(canvas, yc);

  canvas->cxChord(canvas->ctxcanvas, xc, yc, w, h, angle1, angle2);
}

void cdfCanvasChord(cdCanvas* canvas, double xc, double yc, double w, double h, double angle1, double angle2)
{
  assert(canvas);
  if (!_cdCheckCanvas(canvas)) return;

  if (angle1 == angle2 || w == 0 || h == 0)
    return;

  normAngles(&angle1, &angle2);

  if (canvas->interior_style == CD_HOLLOW)
  {
    double xi,yi,xf,yf;
    
    xi = xc + w*cos(CD_DEG2RAD*angle1)/2.0;
    yi = yc + h*sin(CD_DEG2RAD*angle1)/2.0;
    
    xf = xc + w*cos(CD_DEG2RAD*angle2)/2.0;
    yf = yc + h*sin(CD_DEG2RAD*angle2)/2.0;

    cdfCanvasArc(canvas, xc, yc, w, h, angle1, angle2);

    if (fabs(angle2-angle1) < 360)
      cdfCanvasLine(canvas, xi, yi, xf, yf);

    return;
  }

  if (canvas->use_origin)
  {
    xc += canvas->forigin.x;
    yc += canvas->forigin.y;
  }

  if (canvas->invert_yaxis)
    yc = _cdInvertYAxis(canvas, yc);

  if (canvas->cxFChord)
    canvas->cxFChord(canvas->ctxcanvas, xc, yc, w, h, angle1, angle2);
  else
    canvas->cxChord(canvas->ctxcanvas, _cdRound(xc), _cdRound(yc), _cdRound(w), _cdRound(h), angle1, angle2);
}

void cdCanvasGetEllipseBox(int xc, int yc, int w, int h, double a1, double a2, int *xmin, int *xmax, int *ymin, int *ymax)
{
#define _BBOX()               \
  if (x > *xmax) *xmax = x;   \
  if (y > *ymax) *ymax = y;   \
  if (x < *xmin) *xmin = x;   \
  if (y < *ymin) *ymin = y;

  int x, y;

  *xmin = (int)(xc+w/2*cos(a1*CD_DEG2RAD));
  *ymin = (int)(yc+h/2*sin(a1*CD_DEG2RAD));
  *xmax = *xmin;
  *ymax = *ymin;

  x = (int)(xc+w/2*cos(a2*CD_DEG2RAD));
  y = (int)(yc+h/2*sin(a2*CD_DEG2RAD));
  _BBOX()

  if (a1 > a2)
  {
     x = xc+w/2;
     y = yc;
    _BBOX()
  }
  if ((a1<90 && 90<a2) || (a1>a2 && a2>90) || (a2<a1 && a1<90))
  {
    x = xc;
    y = yc+h/2;
    _BBOX()
  }
  if ((a1<180 && 180<a2) || (a1>a2 && a2>180) || (a2<a1 && a1<180))
  {
    x = xc-w/2;
    y = yc;
    _BBOX()
  }
  if ((a1<270 && 270<a2) || (a1>a2 && a2>270) || (a2<a1 && a1<270))
  {
    x = xc;
    y = yc-h/2;
    _BBOX()
  }
}
