/** \file
 * \brief Simulation that is independent of the Simulation Base Driver
 *
 * See Copyright Notice in cd.h
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <memory.h>

#include "cd.h"
#include "cd_private.h"


void cdSimMark(cdCanvas* canvas, int x, int y)
{
  int oldinteriorstyle = canvas->interior_style;
  int oldlinestyle = canvas->line_style;
  int oldlinewidth = canvas->line_width;
  int size = canvas->mark_size;
  int half_size = size/2;
  int bottom = y-half_size;
  int top    = y+half_size;
  int left   = x-half_size;
  int right  = x+half_size;

  if (canvas->interior_style != CD_SOLID &&
      (canvas->mark_type == CD_CIRCLE ||
       canvas->mark_type == CD_BOX ||
       canvas->mark_type == CD_DIAMOND))
    cdCanvasInteriorStyle(canvas, CD_SOLID);

  if (canvas->line_style != CD_CONTINUOUS &&
      (canvas->mark_type == CD_STAR ||
       canvas->mark_type == CD_PLUS ||
       canvas->mark_type == CD_X ||
       canvas->mark_type == CD_HOLLOW_BOX ||
       canvas->mark_type == CD_HOLLOW_CIRCLE ||
       canvas->mark_type == CD_HOLLOW_DIAMOND))
    cdCanvasLineStyle(canvas, CD_CONTINUOUS);

  if (canvas->line_width != 1 &&
      (canvas->mark_type == CD_STAR ||
       canvas->mark_type == CD_PLUS ||
       canvas->mark_type == CD_X ||
       canvas->mark_type == CD_HOLLOW_BOX ||
       canvas->mark_type == CD_HOLLOW_CIRCLE ||
       canvas->mark_type == CD_HOLLOW_DIAMOND))
    cdCanvasLineWidth(canvas, 1);

  switch (canvas->mark_type)
  {
  case CD_STAR:
    canvas->cxLine(canvas->ctxcanvas, left, bottom, right, top);
    canvas->cxLine(canvas->ctxcanvas, left, top, right, bottom);
    /* continue */
  case CD_PLUS:
    canvas->cxLine(canvas->ctxcanvas, left, y, right, y);
    canvas->cxLine(canvas->ctxcanvas, x, bottom, x, top);
    break;
  case CD_HOLLOW_CIRCLE:
    canvas->cxArc(canvas->ctxcanvas, x, y, size, size, 0, 360);
    break;
  case CD_HOLLOW_BOX:
    canvas->cxRect(canvas->ctxcanvas, left, right, bottom, top);
    break;
  case CD_HOLLOW_DIAMOND:
    canvas->cxLine(canvas->ctxcanvas, left, y, x, top);
    canvas->cxLine(canvas->ctxcanvas, x, top, right, y);
    canvas->cxLine(canvas->ctxcanvas, right, y, x, bottom);
    canvas->cxLine(canvas->ctxcanvas, x, bottom, left, y);
    break;
  case CD_X:
    canvas->cxLine(canvas->ctxcanvas, left, bottom, right, top);
    canvas->cxLine(canvas->ctxcanvas, left, top, right, bottom);
    break;
  case CD_CIRCLE:
    canvas->cxSector(canvas->ctxcanvas, x, y, size, size, 0, 360);
    break;
  case CD_BOX:
    canvas->cxBox(canvas->ctxcanvas, left, right, bottom, top);
    break;
  case CD_DIAMOND:
    {
      cdPoint poly[5];
      poly[0].x = left;
      poly[0].y = y;
      poly[1].x = x;
      poly[1].y = top;
      poly[2].x = right;
      poly[2].y = y;
      poly[3].x = x;
      poly[3].y = bottom;
      canvas->cxPoly(canvas->ctxcanvas, CD_FILL, poly, 4);
    }
    break;
  }

  if (canvas->interior_style != oldinteriorstyle &&
      (canvas->mark_type == CD_CIRCLE ||
       canvas->mark_type == CD_BOX ||
       canvas->mark_type == CD_DIAMOND))
    cdCanvasInteriorStyle(canvas, oldinteriorstyle);

  if (canvas->line_style != oldlinestyle &&
      (canvas->mark_type == CD_STAR ||
       canvas->mark_type == CD_PLUS ||
       canvas->mark_type == CD_X ||
       canvas->mark_type == CD_HOLLOW_BOX ||
       canvas->mark_type == CD_HOLLOW_CIRCLE ||
       canvas->mark_type == CD_HOLLOW_DIAMOND))
    cdCanvasLineStyle(canvas, oldlinestyle);

  if (canvas->line_width != oldlinewidth &&
      (canvas->mark_type == CD_STAR ||
       canvas->mark_type == CD_PLUS ||
       canvas->mark_type == CD_X ||
       canvas->mark_type == CD_HOLLOW_BOX ||
       canvas->mark_type == CD_HOLLOW_CIRCLE ||
       canvas->mark_type == CD_HOLLOW_DIAMOND))
    cdCanvasLineWidth(canvas, oldlinewidth);
}

/* Setup Bezier coefficient array once for each control polygon. 
 */
static void BezierForm(const cdPoint* p, cdfPoint* c)
{
  int k; 
  static int choose[4] = {1, 3, 3, 1};
  for (k = 0; k < 4; k++) 
  {
    c[k].x = p[k].x * choose[k];
    c[k].y = p[k].y * choose[k];
  }
}

static void fBezierForm(const cdfPoint* p, cdfPoint* c)
{
  int k; 
  static int choose[4] = {1, 3, 3, 1};
  for (k = 0; k < 4; k++) 
  {
    c[k].x = p[k].x * choose[k];
    c[k].y = p[k].y * choose[k];
  }
}

/* Return Point pt(t), t <= 0 <= 1 from C. 
 * BezierForm must be called once for any given control polygon.
 */
static void BezierCurve(const cdfPoint* c, cdfPoint *pt, double t)
{                  
  int k;
  double t1, tt, u;
  cdfPoint b[4];

  u = t;

  b[0].x = c[0].x;
  b[0].y = c[0].y;
  for(k = 1; k < 4; k++) 
  {
    b[k].x = c[k].x * u;
    b[k].y = c[k].y * u;
    u =u*t;
  }

  pt->x = b[3].x;  
  pt->y = b[3].y;
  t1 = 1-t;          
  tt = t1;
  for(k = 2; k >= 0; k--) 
  {
    pt->x += b[k].x * tt;
    pt->y += b[k].y * tt;
    tt =tt*t1;
  }
}

static int BezierNumSegments(cdCanvas* canvas, const cdPoint* p)
{
  int i, K, dx, dy, d,
    xmax = p[0].x, 
    ymax = p[0].y, 
    xmin = p[0].x, 
    ymin = p[0].y;

  for (i = 1; i < 4; i++)
  {
    if (p[i].x > xmax)
      xmax = p[i].x;
    if (p[i].y > ymax)
      ymax = p[i].y;
    if (p[i].x < xmin)
      xmin = p[i].x;
    if (p[i].y < ymin)
      ymin = p[i].y;
  }

  if (canvas->use_matrix)
  {
    cdMatrixTransformPoint(canvas->matrix, xmin, ymin, &xmin, &ymin);
    cdMatrixTransformPoint(canvas->matrix, xmax, ymax, &xmax, &ymax);
  }

  /* diagonal of the bouding box */
  dx = (xmax-xmin);
  dy = (ymax-ymin);
  d = (int)(sqrt(dx*dx + dy*dy));
  K = d / 8;
  if (K < 8) K = 8;
  return K;
}

static int fBezierNumSegments(cdCanvas* canvas, const cdfPoint* p)
{
  int i, K, d;
  double dx, dy,
    xmax = p[0].x, 
    ymax = p[0].y, 
    xmin = p[0].x, 
    ymin = p[0].y;

  for (i = 1; i < 4; i++)
  {
    if (p[i].x > xmax)
      xmax = p[i].x;
    if (p[i].y > ymax)
      ymax = p[i].y;
    if (p[i].x < xmin)
      xmin = p[i].x;
    if (p[i].y < ymin)
      ymin = p[i].y;
  }

  /* diagonal of the bouding box */
  dx = (xmax-xmin);
  dy = (ymax-ymin);
  d = (int)(sqrt(dx*dx + dy*dy));
  K = d / 8;
  if (K < 8) K = 8;
  return K;
}

/* from sim.h */
void simfLineThin(cdCanvas* canvas, double x1, double y1, double x2, double y2, int *last_xi_a, int *last_yi_a, int *last_xi_b, int *last_yi_b);

/* Quick and Simple Bezier Curve Drawing --- Robert D. Miller
 * Graphics GEMS V  */
void cdSimPolyBezier(cdCanvas* canvas, const cdPoint* points, int n)
{
  int i = 0, k, K, poly_max = 0;
  cdfPoint pt, prev_pt;
  cdfPoint bezier_control[4];
  cdPoint* poly = NULL;
  int use_poly = 0,
      last_xi_a = -65535, 
      last_yi_a = -65535, 
      last_xi_b = -65535, 
      last_yi_b = -65535;

  /* Use special floating point anti-alias line draw when
     line_width==1, and NOT using cdlineSIM. */
  if (canvas->line_width > 1 || canvas->cxLine != cdlineSIM)
    use_poly = 1;

  n--; /* first n is 4 */
  while (n >= 3)
  {
    BezierForm(points+i, bezier_control);
    K = BezierNumSegments(canvas, points+i);

    if (use_poly && poly_max < K+1)
    {
      poly = realloc(poly, sizeof(cdPoint)*(K+1)); /* K+1 points */
      if (!poly) return;
      poly_max = K+1;
    }

    /* first segment */
    BezierCurve(bezier_control, &pt, 0);
    if (use_poly)
    {
      poly[0].x = _cdRound(pt.x);
      poly[0].y = _cdRound(pt.y);
    }
    else
      prev_pt = pt;

    for(k = 1; k < K+1; k++) 
    {
      BezierCurve(bezier_control, &pt, (double)k/(double)K);

      if (use_poly)
      {
        poly[k].x = _cdRound(pt.x);
        poly[k].y = _cdRound(pt.y);
      }
      else
      {
        int old_use_matrix = canvas->use_matrix;
        double x1 = prev_pt.x,
               y1 = prev_pt.y,
               x2 = pt.x,
               y2 = pt.y;

        if (canvas->use_matrix && !canvas->invert_yaxis)
        {
          cdfMatrixTransformPoint(canvas->matrix, x1, y1, &x1, &y1);
          cdfMatrixTransformPoint(canvas->matrix, x2, y2, &x2, &y2);
        }

        /* must disable transformation here, because line simulation use cxPixel */
        canvas->use_matrix = 0;

        simfLineThin(canvas, x1, y1, x2, y2, &last_xi_a, &last_yi_a, &last_xi_b, &last_yi_b);

        canvas->use_matrix = old_use_matrix;
        prev_pt = pt;
      }
    }

    if (use_poly)
      canvas->cxPoly(canvas->ctxcanvas, CD_OPEN_LINES, poly, k);

    n -= 3; i += 3;
  }

  if (poly) free(poly);
}

void cdfSimPolyBezier(cdCanvas* canvas, const cdfPoint* points, int n)
{
  int i = 0, k, K, poly_max = 0;
  cdfPoint pt;
  cdfPoint bezier_control[4];
  cdfPoint* poly = NULL;

  n--; /* first n is 4 */
  while (n >= 3)
  {
    fBezierForm(points+i, bezier_control);
    K = fBezierNumSegments(canvas, points+i);

    if (poly_max < K+1)
    {
      poly = realloc(poly, sizeof(cdfPoint)*(K+1)); /* K+1 points */
      if (!poly) return;
      poly_max = K+1;
    }

    /* first segment */
    BezierCurve(bezier_control, &pt, 0);
    poly[0].x = _cdRound(pt.x);
    poly[0].y = _cdRound(pt.y);

    for(k = 1; k < K+1; k++) 
    {
      BezierCurve(bezier_control, &pt, (double)k/(double)K);

      poly[k].x = _cdRound(pt.x);
      poly[k].y = _cdRound(pt.y);
    }

    canvas->cxFPoly(canvas->ctxcanvas, CD_OPEN_LINES, poly, k);
    n -= 3; i += 3;
  }

  if (poly) free(poly);
}

void cdSimPutImageRectRGBA(cdCanvas* canvas, int iw, int ih, const unsigned char *r, const unsigned char *g, const unsigned char *b, const unsigned char *a, int x, int y, int w, int h, int xmin, int xmax, int ymin, int ymax)
{
  int size, i, j, dst, src, *fx, *fy, rw, rh;
  unsigned char *ar, *ag, *ab, al;

  size = w * h;
  ar = (unsigned char*)malloc(size*3);
  if (!ar) return;
  ag = ar + size;
  ab = ag + size;

  canvas->cxGetImageRGB(canvas->ctxcanvas, ar, ag, ab, x, y, w, h);

  rw = xmax-xmin+1;
  rh = ymax-ymin+1;

  fx = cdGetZoomTable(w, rw, xmin);
  fy = cdGetZoomTable(h, rh, ymin);

  for (j = 0; j < h; j++)
  {
    for (i = 0; i < w; i++)
    {
      dst = j * w + i;
      src = fy[j] * iw + fx[i];
      al = a[src];
      ar[dst] = CD_ALPHA_BLEND(r[src], ar[dst], al);
      ag[dst] = CD_ALPHA_BLEND(g[src], ag[dst], al);
      ab[dst] = CD_ALPHA_BLEND(b[src], ab[dst], al);
    }
  }

  canvas->cxPutImageRectRGB(canvas->ctxcanvas, w, h, ar, ag, ab, x, y, w, h, 0, 0, 0, 0);

  free(ar);

  free(fx);
  free(fy);
}
