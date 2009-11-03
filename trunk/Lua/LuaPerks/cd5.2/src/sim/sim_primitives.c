/** \file
 * \brief Primitives of the Simulation Base Driver
 *
 * See Copyright Notice in cd.h
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <memory.h>

#include "cd.h"
#include "cd_private.h"
#include "cd_truetype.h"
#include "sim.h"

void cdlineSIM(cdCtxCanvas* ctxcanvas, int x1, int y1, int x2, int y2)
{
  cdCanvas* canvas = ((cdCtxCanvasBase*)ctxcanvas)->canvas;
  int old_use_matrix = canvas->use_matrix;

  if (canvas->use_matrix && !canvas->invert_yaxis)
  {
    cdMatrixTransformPoint(canvas->matrix, x1, y1, &x1, &y1);
    cdMatrixTransformPoint(canvas->matrix, x2, y2, &x2, &y2);
  }

  /* must disable transformation here, because line simulation use cxPixel */
  canvas->use_matrix = 0;

  if(canvas->line_width > 1)
    simLineThick(canvas, x1, y1, x2, y2);
  else 
    simLineThin(canvas, x1, y1, x2, y2);

  canvas->use_matrix = old_use_matrix;
}

void cdrectSIM(cdCtxCanvas* ctxcanvas, int xmin, int xmax, int ymin, int ymax)
{
  cdCanvas* canvas = ((cdCtxCanvasBase*)ctxcanvas)->canvas;
  cdPoint poly[5]; /* leave room of one more point */
  poly[0].x = xmin; poly[0].y = ymin;
  poly[1].x = xmin; poly[1].y = ymax;
  poly[2].x = xmax; poly[2].y = ymax;
  poly[3].x = xmax; poly[3].y = ymin;
  canvas->cxPoly(canvas->ctxcanvas, CD_CLOSED_LINES, poly, 4);
}

void cdboxSIM(cdCtxCanvas* ctxcanvas, int xmin, int xmax, int ymin, int ymax)
{
  cdCanvas* canvas = ((cdCtxCanvasBase*)ctxcanvas)->canvas;
  
  if (canvas->use_matrix)
  {
    cdPoint poly[5]; /* leave room of one more point */
    poly[0].x = xmin; poly[0].y = ymin;
    poly[1].x = xmin; poly[1].y = ymax;
    poly[2].x = xmax; poly[2].y = ymax;
    poly[3].x = xmax; poly[3].y = ymin;
    canvas->cxPoly(canvas->ctxcanvas, CD_FILL, poly, 4);
  }
  else
  {
    cdSimulation* simulation = canvas->simulation;
    int y;

    /* must set line attributes here, because fill simulation use cxLine and cxPixel */
    int old_line_style = cdCanvasLineStyle(canvas, CD_CONTINUOUS);
    int old_line_width = cdCanvasLineWidth(canvas, 1);

    for(y=ymin;y<=ymax;y++)
      simFillHorizLine(simulation, xmin, y, xmax);

    cdCanvasLineStyle(canvas, old_line_style);
    cdCanvasLineWidth(canvas, old_line_width);
  }
}

void cdfSimBox(cdCtxCanvas *ctxcanvas, double xmin, double xmax, double ymin, double ymax)
{
  cdCanvas* canvas = ((cdCtxCanvasBase*)ctxcanvas)->canvas;
  cdfPoint poly[5]; /* leave room of one more point */
  poly[0].x = xmin; poly[0].y = ymin;
  poly[1].x = xmin; poly[1].y = ymax;
  poly[2].x = xmax; poly[2].y = ymax;
  poly[3].x = xmax; poly[3].y = ymin;
  canvas->cxFPoly(canvas->ctxcanvas, CD_FILL, poly, 4);
}

void cdfSimRect(cdCtxCanvas *ctxcanvas, double xmin, double xmax, double ymin, double ymax)
{
  cdCanvas* canvas = ((cdCtxCanvasBase*)ctxcanvas)->canvas;
  cdfPoint poly[5]; /* leave room of one more point */
  poly[0].x = xmin; poly[0].y = ymin;
  poly[1].x = xmin; poly[1].y = ymax;
  poly[2].x = xmax; poly[2].y = ymax;
  poly[3].x = xmax; poly[3].y = ymin;
  canvas->cxFPoly(canvas->ctxcanvas, CD_CLOSED_LINES, poly, 4);
}

int simCalcEllipseNumSegments(cdCanvas* canvas, int xc, int yc, int width, int height)
{
  int n, dx, dy, hd;
  int w2 = width/2;
  int h2 = height/2;
  int x1 = xc-w2, 
      y1 = yc-h2, 
      x2 = xc+w2, 
      y2 = yc+h2;

  if (canvas->use_matrix)
  {
    cdMatrixTransformPoint(canvas->matrix, x1, y1, &x1, &y1);
    cdMatrixTransformPoint(canvas->matrix, x2, y2, &x2, &y2);
  }

  dx = (x1-x2);
  dy = (y1-y2);
  hd = (int)(sqrt(dx*dx + dy*dy)/2);

  /*  Estimation Heuristic:
  use half diagonal to estimate the number of segments for 360 degrees.
  Use the difference of the half diagonal and its projection to calculate the minimum angle:
  cos(min_angle) = hd / (hd + 1)     or   min_angle = acos(hd / (hd + 1))
  The number of segments will be 360 / min_angle.
  */

  n = (int)((360.0*CD_DEG2RAD) / acos((double)hd / (hd + 1.0)) + 0.5); /* round up */

  /* multiple of 4 */
  n = ((n + 3)/4)*4;

  /* minimum number is 4 */
  if (n < 4) n = 4;

  return n;
}

void cdarcSIM(cdCtxCanvas* ctxcanvas, int xc, int yc, int width, int height, double angle1, double angle2)
{
  cdCanvas* canvas = ((cdCtxCanvasBase*)ctxcanvas)->canvas;
  double c, s, sx, sy, x, y, prev_x, prev_y;
  double da;
  int i, yc2 = 2*yc, p,
      last_xi_a = -65535, 
      last_yi_a = -65535, 
      last_xi_b = -65535, 
      last_yi_b = -65535;
  cdPoint* poly = NULL;

  /* number of segments of equivalent poligonal for a full ellipse */
  int n = simCalcEllipseNumSegments(canvas, xc, yc, width, height);

  /* Use special floating point anti-alias line draw when
     line_width==1, and NOT using cdlineSIM. */
  if (canvas->line_width > 1 || canvas->cxLine != cdlineSIM)
  {
    poly = (cdPoint*)malloc(sizeof(cdPoint)*(n+1));  /* n+1 points */
    if (!poly) return;
  }

  /* number of segments for the arc */
  n = cdRound((fabs(angle2-angle1)*n)/360);
  if (n < 1) n = 1;

  /* converts degrees into radians */
  angle1 *= CD_DEG2RAD;
  angle2 *= CD_DEG2RAD;

  /* generates arc points at origin with axis x and y */

  da = (angle2-angle1)/n;
  c  = cos(da);
  s  = sin(da);
  sx = -(width*s)/height;
  sy = (height*s)/width;

  x = (width/2.0f)*cos(angle1);
  y = (height/2.0f)*sin(angle1);
  prev_x = x;
  prev_y = y;
  if (poly)
  {
    poly[0].x = _cdRound(x)+xc;
    poly[0].y = _cdRound(y)+yc;

    if (canvas->invert_yaxis)  /* must invert because of the angle orientation */
      poly[0].y = yc2 - poly[0].y;

    p = 1;
  }
  else
    simLineStyleNoReset = 1;

  for (i = 1; i < n+1; i++)  /* n+1 points */
  {
    x =  c*prev_x + sx*prev_y;
    y = sy*prev_x +  c*prev_y;

    if (poly)
    {
      poly[p].x = _cdRound(x)+xc;
      poly[p].y = _cdRound(y)+yc;

      if (canvas->invert_yaxis)   /* must invert because of the angle orientation */
        poly[p].y = yc2 - poly[p].y;

      if (poly[p-1].x != poly[p].x || poly[p-1].y != poly[p].y)
        p++;
    }
    else
    {
      int old_use_matrix = canvas->use_matrix;
      double x1 = prev_x+xc,
             y1 = prev_y+yc,
             x2 = x+xc,
             y2 = y+yc;

      if (canvas->use_matrix && !canvas->invert_yaxis)
      {
        cdfMatrixTransformPoint(canvas->matrix, x1, y1, &x1, &y1);
        cdfMatrixTransformPoint(canvas->matrix, x2, y2, &x2, &y2);
      }

      /* must disable transformation here, because line simulation use cxPixel */
      canvas->use_matrix = 0;

      if (canvas->invert_yaxis)  /* must invert because of the angle orientation */
      {
        y1 = yc2 - y1;
        y2 = yc2 - y2;
      }

      simfLineThin(canvas, x1, y1, x2, y2, &last_xi_a, &last_yi_a, &last_xi_b, &last_yi_b);

      canvas->use_matrix = old_use_matrix;
    }

    prev_x = x;
    prev_y = y;
  }

  if (poly)
  {
    canvas->cxPoly(canvas->ctxcanvas, CD_OPEN_LINES, poly, p);
    free(poly);
  }
  else
    simLineStyleNoReset = 0;
}

void cdfSimArc(cdCtxCanvas *ctxcanvas, double xc, double yc, double width, double height, double angle1, double angle2)
{
  cdCanvas* canvas = ((cdCtxCanvasBase*)ctxcanvas)->canvas;
  double c, s, sx, sy, x, y, prev_x, prev_y, da;
  int i, p;
  cdfPoint* poly = NULL;

  /* number of segments of equivalent poligonal for a full ellipse */
  int n = simCalcEllipseNumSegments(canvas, (int)xc, (int)yc, (int)width, (int)height);

  poly = (cdfPoint*)malloc(sizeof(cdfPoint)*(n+1));  /* n+1 points */
  if (!poly) return;

  /* number of segments for the arc */
  n = cdRound((fabs(angle2-angle1)*n)/360);
  if (n < 1) n = 1;

  /* converts degrees into radians */
  angle1 *= CD_DEG2RAD;
  angle2 *= CD_DEG2RAD;

  /* generates arc points at origin with axis x and y */

  da = (angle2-angle1)/n;
  c  = cos(da);
  s  = sin(da);
  sx = -(width*s)/height;
  sy = (height*s)/width;

  x = (width/2.0f)*cos(angle1);
  y = (height/2.0f)*sin(angle1);
  prev_x = x;
  prev_y = y;
  poly[0].x = x+xc;
  poly[0].y = y+yc;

  p = 1;

  for (i = 1; i < n+1; i++)  /* n+1 points */
  {
    x =  c*prev_x + sx*prev_y;
    y = sy*prev_x +  c*prev_y;

    poly[p].x = x+xc;
    poly[p].y = y+yc;

    if (poly[p-1].x != poly[p].x || 
        poly[p-1].y != poly[p].y)
      p++;

    prev_x = x;
    prev_y = y;
  }

  canvas->cxFPoly(canvas->ctxcanvas, CD_OPEN_LINES, poly, p);
  free(poly);
}

void cdfSimElipse(cdCtxCanvas* ctxcanvas, double xc, double yc, double width, double height, double angle1, double angle2, int sector)
{
  cdCanvas* canvas = ((cdCtxCanvasBase*)ctxcanvas)->canvas;
  double c, s, sx, sy, x, y, prev_x, prev_y, da;
  int i, p;
  cdfPoint* poly;

  /* number of segments of equivalent poligonal for a full ellipse */
  int n = simCalcEllipseNumSegments(canvas, (int)xc, (int)yc, (int)width, (int)height);

  /* number of segments for the arc */
  n = cdRound(((angle2-angle1)*n)/360);
  if (n < 1) n = 1;

  poly = (cdfPoint*)malloc(sizeof(cdfPoint)*(n+2+1));  /* n+1 points +1 center */

  /* converts degrees into radians */
  angle1 *= CD_DEG2RAD;
  angle2 *= CD_DEG2RAD;

  /* generates arc points at origin with axis x and y */

  da = (angle2-angle1)/n;
  c  = cos(da);
  s  = sin(da);
  sx = -(width*s)/height;
  sy = (height*s)/width;

  x = xc +  (width/2.0)*cos(angle1);
  y = yc + (height/2.0)*sin(angle1);
  prev_x = x;
  prev_y = y;

  poly[0].x = x;
  poly[0].y = y;
  p = 1;

  for (i = 1; i < n+1; i++) /* n+1 points */
  {
    x = xc +  c*(prev_x-xc) + sx*(prev_y-yc);
    y = yc + sy*(prev_x-xc) +  c*(prev_y-yc);

    poly[p].x = x;
    poly[p].y = y;

    if (poly[p-1].x != poly[p].x || poly[p-1].y != poly[p].y)
      p++;

    prev_x = x;
    prev_y = y;
  }

  if (poly[p-1].x != poly[0].x || poly[p-1].y != poly[0].y)
  {
    if (sector)  /* cdSector */
    {
      /* add center */
      poly[p].x = xc;
      poly[p].y = yc;
    }
    else         /* cdChord */
    {
      /* add initial point */
      poly[p].x = poly[0].x;
      poly[p].y = poly[0].y;
    }
    p++;
  }

  canvas->cxFPoly(canvas->ctxcanvas, CD_FILL, poly, p);
  free(poly);
}

static void cdSimElipse(cdCtxCanvas* ctxcanvas, int xc, int yc, int width, int height, double angle1, double angle2, int sector)
{
  cdCanvas* canvas = ((cdCtxCanvasBase*)ctxcanvas)->canvas;
  float c, s, sx, sy, x, y, prev_x, prev_y;
  double da;
  int i, p, yc2 = 2*yc;
  cdPoint* poly;

  /* number of segments of equivalent poligonal for a full ellipse */
  int n = simCalcEllipseNumSegments(canvas, xc, yc, width, height);

  /* number of segments for the arc */
  n = cdRound(((angle2-angle1)*n)/360);
  if (n < 1) n = 1;

  poly = (cdPoint*)malloc(sizeof(cdPoint)*(n+2+1));  /* n+1 points +1 center */

  /* converts degrees into radians */
  angle1 *= CD_DEG2RAD;
  angle2 *= CD_DEG2RAD;

  /* generates arc points at origin with axis x and y */

  da = (angle2-angle1)/n;
  c  = (float)cos(da);
  s  = (float)sin(da);
  sx = -(width*s)/height;
  sy = (height*s)/width;

  x = xc +  (width/2.0f)*(float)cos(angle1);
  y = yc + (height/2.0f)*(float)sin(angle1);
  prev_x = x;
  prev_y = y;

  poly[0].x = _cdRound(x);
  poly[0].y = _cdRound(y);
  if (canvas->invert_yaxis)
    poly[0].y = yc2 - poly[0].y;
  p = 1;

  for (i = 1; i < n+1; i++) /* n+1 points */
  {
    x = xc +  c*(prev_x-xc) + sx*(prev_y-yc);
    y = yc + sy*(prev_x-xc) +  c*(prev_y-yc);

    poly[p].x = _cdRound(x);
    poly[p].y = _cdRound(y);

    if (canvas->invert_yaxis)
      poly[p].y = yc2 - poly[p].y;

    if (poly[p-1].x != poly[p].x || poly[p-1].y != poly[p].y)
      p++;

    prev_x = x;
    prev_y = y;
  }

  if (poly[p-1].x != poly[0].x || poly[p-1].y != poly[0].y)
  {
    if (sector)  /* cdSector */
    {
      /* add center */
      poly[p].x = xc;
      poly[p].y = yc;
    }
    else         /* cdChord */
    {
      /* add initial point */
      poly[p].x = poly[0].x;
      poly[p].y = poly[0].y;
    }
    p++;
  }

  canvas->cxPoly(canvas->ctxcanvas, CD_FILL, poly, p);

  free(poly);
}

void cdsectorSIM(cdCtxCanvas* ctxcanvas, int xc, int yc, int width, int height, double angle1, double angle2)
{
  cdSimElipse(ctxcanvas, xc, yc, width, height, angle1, angle2, 1);
}

void cdchordSIM(cdCtxCanvas* ctxcanvas, int xc, int yc, int width, int height, double angle1, double angle2)
{
  cdSimElipse(ctxcanvas, xc, yc, width, height, angle1, angle2, 0);
}

void cdpolySIM(cdCtxCanvas* ctxcanvas, int mode, cdPoint* poly, int n)
{
  cdCanvas* canvas = ((cdCtxCanvasBase*)ctxcanvas)->canvas;
  int i, reset = 1;

  switch(mode) 
  {
  case CD_CLOSED_LINES:
    poly[n] = poly[0];
    n++;
    /* continue */
  case CD_OPEN_LINES:
    if (simLineStyleNoReset)  /* Bezier simulation use several poly */
    {
      reset = 0;
      simLineStyleNoReset = 1;
    }
    for (i = 0; i< n - 1; i++)
      canvas->cxLine(canvas->ctxcanvas, poly[i].x, poly[i].y, poly[i+1].x, poly[i+1].y);
    if (reset) simLineStyleNoReset = 0;
    break;
  case CD_BEZIER:
    simLineStyleNoReset = 1;
    cdSimPolyBezier(canvas, poly, n);
    simLineStyleNoReset = 0;
    break;
  case CD_FILL:
    {
      /* must set line attributes here, because fill simulation use cxLine */
      int oldwidth = cdCanvasLineWidth(canvas, 1);
      int oldstyle = cdCanvasLineStyle(canvas, CD_CONTINUOUS);
      int old_use_matrix = canvas->use_matrix;

      if (canvas->use_matrix && !canvas->invert_yaxis)
      {
        for(i = 0; i < n; i++)
          cdMatrixTransformPoint(canvas->matrix, poly[i].x, poly[i].y, &poly[i].x, &poly[i].y);
      }

      /* must disable transformation here, because line simulation use cxPixel */
      canvas->use_matrix = 0;

      simPolyFill(canvas->simulation, poly, n);

      canvas->use_matrix = old_use_matrix;
      cdCanvasLineStyle(canvas, oldstyle);
      cdCanvasLineWidth(canvas, oldwidth);
    }
    break;
  }
}
