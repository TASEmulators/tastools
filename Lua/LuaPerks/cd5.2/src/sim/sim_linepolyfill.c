/** \file
 * \brief Primitives of the Simulation Base Driver
 *
 * See Copyright Notice in cd.h
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <memory.h>
#include <assert.h>

#include "cd.h"
#include "cd_private.h"
#include "cd_truetype.h"
#include "sim.h"


/* para estilos de linha usando rotacao de bits */
static const unsigned short int simLineStyleBitTable[5]=
{ 
  0xFFFF, /* CD_CONTINUOUS  */
  0xFF00, /* CD_DASHED      */
  0x1111, /* CD_DOTTED      */
  0xFE10, /* CD_DASH_DOT    */
  0xFF24, /* CD_DASH_DOT_DOT*/
};
int simLineStyleNoReset = 0;
static unsigned short int simLineStyleLastBits = 0;

#define simRotateLineStyle(_x) (((_x) & 0x8000)? ((_x) << 1)|(0x0001): ((_x) << 1))

#define INTENSITYSHIFT 8  /* # of bits by which to shift ErrorAcc to get intensity level */


/* Point in Polygon was obtained from:
   www.geometryalgorithms.com/Archive/algorithm_0103/algorithm_0103.htm

   Copyright 2001, softSurfer (www.softsurfer.com)
   This code may be freely used and modified for any purpose
   providing that this copyright notice is included with it.
   SoftSurfer makes no warranty for this code, and cannot be held
   liable for any real or imagined damage resulting from its use.
*/

#define isLeft( _P0, _P1, _x, _y )  ((_P1.x - _P0.x)*(_y - _P0.y) - (_x - _P0.x)*(_P1.y - _P0.y))

int simIsPointInPolyWind(cdPoint* poly, int n, int x, int y)
{
  int i, i1, 
      wn = 0;  /* the winding number counter  */

  for (i = 0; i < n; i++) 
  {   
    i1 = (i+1)%n; /* next point(i+1), next of last(n-1) is first(0) */

    if (poly[i].y <= y) 
    {         
      if (poly[i1].y > y)      /* an upward crossing */
        if (isLeft(poly[i], poly[i1], x, y) > 0)  /* P left of edge */
          ++wn;                /* have a valid up intersect */
    }
    else 
    {                       
      if (poly[i1].y <= y)     /* a downward crossing */
        if (isLeft(poly[i], poly[i1], x, y) < 0)  /* P right of edge */
          --wn;                /* have a valid down intersect */
    }
  }

  return wn;
}

static int compare_int(const int* xx1, const int* xx2)
{
  return *xx1 - *xx2;
}

void simAddSegment(simLineSegment* segment, int x1, int y1, int x2, int y2, int *y_max, int *y_min)
{
  /* Make sure p2.y > p1.y */
  if (y1 > y2) 
  {
    _cdSwapInt(y1, y2);
    _cdSwapInt(x1, x2);
  }

  segment->x1 = x1;
  segment->y1 = y1;
  segment->x2 = x2;
  segment->y2 = y2;

  segment->x = x2;  /* initial value */
  
  segment->DeltaY = y2 - y1;
  segment->DeltaX = x2 - x1;
  if (segment->DeltaX >= 0)
    segment->XDir = -1;  /* inverted from simLineThin since here is from p2 to p1 */
  else 
  {
    segment->XDir = 1;
    segment->DeltaX = -segment->DeltaX; /* make DeltaX positive */
  }

  segment->ErrorAcc = 0;  /* initialize the line error accumulator to 0 */

  /* Is this an X-major or Y-major line? */
  if (segment->DeltaY > segment->DeltaX) 
  {
    if (segment->DeltaY==0)  /* do not compute for horizontal segments */
      return;

    /* Y-major line; calculate 16-bit fixed-point fractional part of a
    pixel that X advances each time Y advances 1 pixel, truncating the
    result so that we won't overrun the endpoint along the X axis */
    segment->ErrorInc = (unsigned short)(((unsigned long)segment->DeltaX << 16) / (unsigned long)segment->DeltaY);
  }
  else
  {
    if (segment->DeltaX==0)  /* do not compute for vertical segments */
      return;

    /* It's an X-major line; calculate 16-bit fixed-point fractional part of a
    pixel that Y advances each time X advances 1 pixel, truncating the
    result to avoid overrunning the endpoint along the X axis */
    segment->ErrorInc = (unsigned short)(((unsigned long)segment->DeltaY << 16) / (unsigned long)segment->DeltaX);
  }

  /* also calculates y_max and y_min of the polygon */
  if (y2 > *y_max)
    *y_max = y2;
  if (y1 < *y_min)
    *y_min = y1;
}

int simSegmentInc(simLineSegment* segment, cdCanvas* canvas, int y)
{
  unsigned short ErrorAccTemp, Weighting;

  if (segment->DeltaY == 0) 
  {
    /* Horizontal line */
    while (segment->DeltaX-- != 0) 
      segment->x += segment->XDir;
    return segment->x;
  }

  if (segment->DeltaX == 0) 
  {
    /* Vertical line */
    segment->DeltaY--;
    return segment->x;
  }

  if (segment->DeltaX == segment->DeltaY) 
  {
    /* Perfect Diagonal line */
    segment->x += segment->XDir;
    segment->DeltaY--;
    return segment->x;
  }

  /* Is this an X-major or Y-major line? */
  if (segment->DeltaY > segment->DeltaX) 
  {
    /* Increment pixels other than the first and last */
    ErrorAccTemp = segment->ErrorAcc;   /* remember currrent accumulated error */
    segment->ErrorAcc += segment->ErrorInc;      /* calculate error for next pixel */
    if (segment->ErrorAcc <= ErrorAccTemp) 
    {
      /* The error accumulator turned over, so advance the X coord */
      segment->x += segment->XDir;
    }

    Weighting = segment->ErrorAcc >> INTENSITYSHIFT;

    if (Weighting < 128)
      return segment->x;
    else
      return segment->x + segment->XDir;
  }
  else
  {
    /* Increment all pixels other than the first and last */
    int hline_end = 0;
    while (!hline_end)
    {
      ErrorAccTemp = segment->ErrorAcc;   /* remember currrent accumulated error */
      segment->ErrorAcc += segment->ErrorInc;      /* calculate error for next pixel */
      if (segment->ErrorAcc <= ErrorAccTemp) 
      {
        /* The error accumulator turned over, so advance the Y coord */
        hline_end = 1;
      }

      segment->x += segment->XDir; /* X-major, so always advance X */
    }

    return segment->x;
  }
}

typedef struct _simIntervalList
{
  int* xx;
  int n, count;
} simIntervalList;

static int simFillCheckAAPixel(simIntervalList* line_int_list, int x)
{
  int i, *xx = line_int_list->xx;
  for (i = 0; i < line_int_list->n; i+=2)
  {
    if (xx[i] <= x && x <= xx[i+1])
      return 0; /* inside, already drawn, do not draw */
  }
  return 1;
}

static void simPolyAAPixels(cdCanvas *canvas, simIntervalList* line_int_list, int y_min, int y_max, int x1, int y1, int x2, int y2)
{
  unsigned short ErrorInc, ErrorAcc;
  unsigned short ErrorAccTemp, Weighting;
  int DeltaX, DeltaY, XDir;
  int no_antialias = !(canvas->simulation->antialias);

  /* Make sure p2.y > p1.y */
  if (y1 > y2) 
  {
    _cdSwapInt(y1, y2);
    _cdSwapInt(x1, x2);
  }

  DeltaX = x2 - x1;
  if (DeltaX >= 0)
    XDir = 1;
  else 
  {
    XDir = -1;
    DeltaX = -DeltaX; /* make DeltaX positive */
  }

  /* Special-case horizontal, vertical, and diagonal lines, which
  require no weighting because they go right through the center of
  every pixel */
  DeltaY = y2 - y1;
  if (DeltaY == 0 || DeltaX == 0 || DeltaX == DeltaY) return;

  /* Line is not horizontal, diagonal, or vertical */

  /* start and end pixels are not necessary 
     since they are always drawn in the previous step. */

  ErrorAcc = 0;  /* initialize the line error accumulator to 0 */

  /* Is this an X-major or Y-major line? */
  if (DeltaY > DeltaX) 
  {
    ErrorInc = (unsigned short)(((unsigned long)DeltaX << 16) / (unsigned long)DeltaY);

    /* Draw all pixels other than the first and last */
    while (--DeltaY) 
    {
      ErrorAccTemp = ErrorAcc;   /* remember currrent accumulated error */
      ErrorAcc += ErrorInc;      /* calculate error for next pixel */
      if (ErrorAcc <= ErrorAccTemp) 
        x1 += XDir;

      y1++; /* Y-major, so always advance Y */

      Weighting = ErrorAcc >> INTENSITYSHIFT;

      if (y1 < y_min || y1 > y_max) continue;

      if (no_antialias)
      {
        if (Weighting < 128)
        {
          if (simFillCheckAAPixel(line_int_list+(y1-y_min), x1))
            simFillDrawAAPixel(canvas, x1, y1, 255);
        }
        else
        {
          if (simFillCheckAAPixel(line_int_list+(y1-y_min), x1 + XDir))
            simFillDrawAAPixel(canvas, x1 + XDir, y1, 255);
        }
      }
      else
      {
        if (simFillCheckAAPixel(line_int_list+(y1-y_min), x1))
          simFillDrawAAPixel(canvas, x1, y1, 255-Weighting);

        if (simFillCheckAAPixel(line_int_list+(y1-y_min), x1 + XDir))
          simFillDrawAAPixel(canvas, x1 + XDir, y1, Weighting);
      }
    }
  }
  else
  {
    ErrorInc = (unsigned short)(((unsigned long)DeltaY << 16) / (unsigned long)DeltaX);

    /* Draw all pixels other than the first and last */
    while (--DeltaX) 
    {
      ErrorAccTemp = ErrorAcc;   /* remember currrent accumulated error */
      ErrorAcc += ErrorInc;      /* calculate error for next pixel */
      if (ErrorAcc <= ErrorAccTemp) 
        y1++;

      x1 += XDir; /* X-major, so always advance X */

      Weighting = ErrorAcc >> INTENSITYSHIFT;

      if (y1 < y_min || y1 > y_max) continue;

      if (no_antialias)
      {
        if (Weighting < 128)
        {
          if (simFillCheckAAPixel(line_int_list+(y1-y_min), x1))
            simFillDrawAAPixel(canvas, x1, y1, 255);
        }
        else
        {
          if (y1+1 < y_min || y1+1 > y_max) continue;

          if (simFillCheckAAPixel(line_int_list+(y1+1-y_min), x1))
            simFillDrawAAPixel(canvas, x1, y1+1, 255);
        }
      }
      else
      {
        if (simFillCheckAAPixel(line_int_list+(y1-y_min), x1))
          simFillDrawAAPixel(canvas, x1, y1, 255-Weighting);

        if (y1+1 < y_min || y1+1 > y_max) continue;

        if (simFillCheckAAPixel(line_int_list+(y1+1-y_min), x1))
          simFillDrawAAPixel(canvas, x1, y1+1, Weighting);
      }
    }
  }
}

static void simLineIntervallInit(simIntervalList* line_int_list, int count)
{
  line_int_list->xx = malloc(sizeof(int)*count);
  line_int_list->n = 0;
  line_int_list->count = count;
}

static void simLineIntervallAdd(simIntervalList* line_int_list, int x1, int x2)
{
  int i = line_int_list->n;
  line_int_list->xx[i] = x1;
  line_int_list->xx[i+1] = x2;
  line_int_list->n += 2;
}

void simPolyFill(cdSimulation* simulation, cdPoint* poly, int n) 
{
  simLineSegment *seg_i;
  simIntervalList* line_int_list, *line_il;
  int y_max, y_min, i, y, i1, fill_mode, num_lines,
      inter_count, width, height, *xx;

  simLineSegment *segment = (simLineSegment *)malloc(n*sizeof(simLineSegment));

  width = simulation->canvas->w;
  height = simulation->canvas->h;
  fill_mode = simulation->canvas->fill_mode;
  
  y_max = poly[0].y;
  y_min = poly[0].y;
  for(i = 0; i < n; i++)
  {
    i1 = (i+1)%n; /* next point(i+1), next of last(n-1) is first(0) */
    simAddSegment(segment+i, poly[i].x, poly[i].y, poly[i1].x, poly[i1].y, &y_max, &y_min);
  }
  
  if (y_min > height-1 || y_max < 0)
  {
    free(segment);
    return;
  }

  if (y_min < 0) 
    y_min = 0;

  if (y_max > height-1)
    num_lines = height-y_min;
  else
    num_lines = y_max-y_min+1;

  line_int_list = malloc(sizeof(simIntervalList)*num_lines);
  memset(line_int_list, 0, sizeof(simIntervalList)*num_lines);

  xx = (int*)malloc((n+1)*sizeof(int));

  /* for all horizontal lines between y_max and y_min */
  for(y = y_max; y >= y_min; y--)
  {
    inter_count = 0;

    /* for all segments, calculates the intervals to be filled 
       from the intersection with the horizontal line y. */
    for(i = 0; i < n; i++)
    {
      seg_i = segment + i;

      /* if the minimum Y coordinate of the segment is greater than the current y, then ignore the segment. */
      /* if it is an horizontal line, then ignore the segment. */
      if (seg_i->y1 > y ||
          seg_i->y1 == seg_i->y2)
        continue;

      if (y == seg_i->y1)  /* intersection at the start point (x1,y1) */
      {
        int i_next = (i==n-1)? 0: i+1;
        int i_prev = (i==0)? n-1: i-1;
        simLineSegment *seg_i_next = segment + i_next;
        simLineSegment *seg_i_prev = segment + i_prev;

        /* always save at least one intersection point for (y1) */

        xx[inter_count++] = seg_i->x1;  /* save the intersection point */

        /* check for missing bottom-corner points (|_|), must duplicate the intersection */
        if ((seg_i_next->y1 == y && seg_i_next->y2 == seg_i_next->y1) ||  /* next is an horizontal line */
            (seg_i_prev->y1 == y && seg_i_prev->y2 == seg_i_prev->y1))    /* previous is an horizontal line */
        {
          xx[inter_count++] = seg_i->x1;     /* save the intersection point */
        }
      }
      else if ((y > seg_i->y1) && (y < seg_i->y2))  /* intersection inside the segment, do not include y2 */
      {                                             
        xx[inter_count++] = simSegmentInc(seg_i, simulation->canvas, y);  /* save the intersection point */
      }
      else if (y == seg_i->y2)  /* intersection at the end point (x2,y2) */
      {
        int i_next = (i==n-1)? 0: i+1;
        int i_prev = (i==0)? n-1: i-1;
        simLineSegment *seg_i_next = segment + i_next;
        simLineSegment *seg_i_prev = segment + i_prev;

        /* only save the intersection point for (y2) if not handled by (y1) of another segment */

        /* check for missing top-corner points (^) or (|¯¯|) */
        if ((seg_i_next->y2 == y && seg_i_next->y2 == seg_i_next->y1) ||  /* next is an horizontal line */
            (seg_i_prev->y2 == y && seg_i_prev->y2 == seg_i_prev->y1) ||  /* previous is an horizontal line */
            (seg_i_next->y2 == y && seg_i_next->x2 == seg_i->x2 && seg_i_next->x1 != seg_i->x1) || 
            (seg_i_prev->y2 == y && seg_i_prev->x2 == seg_i->x2 && seg_i_prev->x1 != seg_i->x1))
        {
          xx[inter_count++] = seg_i->x2;     /* save the intersection point */
        }
      }
    }
    
    /* if outside the canvas, ignore the intervals and */
    /* continue since the segments where updated.      */
    if (y > height-1 || inter_count == 0)
      continue;

    /* sort the intervals */
    qsort(xx, inter_count, sizeof(int), (int (*)(const void*,const void*))compare_int);

    line_il = line_int_list+(y-y_min);
    simLineIntervallInit(line_il, inter_count*2);

    /* for all intervals, fill the interval */
    for(i = 0; i < inter_count; i += 2)  /* process only pairs */
    {
      if (fill_mode == CD_EVENODD)
      {
        /* since it fills only pairs of intervals, */          
        /* it is the EVENODD fill rule.            */          
        simFillHorizLine(simulation, xx[i], y, xx[i+1]);
        simLineIntervallAdd(line_il, xx[i], xx[i+1]);
      }
      else
      {
        simFillHorizLine(simulation, xx[i], y, xx[i+1]);
        simLineIntervallAdd(line_il, xx[i], xx[i+1]);
        if ((i+2 < inter_count) && (xx[i+1] < xx[i+2]))  /* avoid point intervals */
          if (simIsPointInPolyWind(poly, n, (xx[i+1]+xx[i+2])/2, y)) /* if the next interval is inside the polygon then fill it */
          {
            simFillHorizLine(simulation, xx[i+1], y, xx[i+2]);
            simLineIntervallAdd(line_il, xx[i+1], xx[i+2]);
          }
      }
    }
  }

  free(xx);
  free(segment);

  /* Once the polygon has been filled, now let's draw the
   * antialiased and incomplete pixels at the edges */

  if (y_max > height-1)
    y_max = height-1;

  /* Go through all line segments of the poly */
  for(i = 0; i < n; i++)
  {
    i1 = (i+1)%n;
    simPolyAAPixels(simulation->canvas, line_int_list, y_min, y_max, poly[i].x, poly[i].y, poly[i1].x, poly[i1].y);
  }

  for (i = 0; i < num_lines; i++)
  {
    if (line_int_list[i].xx) 
      free(line_int_list[i].xx);
  }
  free(line_int_list);
}

/*************************************************************************************/
/*************************************************************************************/

#define _cdLineDrawPixel(_canvas, _x1, _y1, _ls, _fgcolor, _bgcolor) \
{                                                                    \
  if (_ls & 1)                                                       \
    _canvas->cxPixel(_canvas->ctxcanvas, _x1, _y1, _fgcolor);        \
  else if (canvas->back_opacity == CD_OPAQUE)                        \
    _canvas->cxPixel(_canvas->ctxcanvas, _x1, _y1, _bgcolor);        \
}

void simLineThick(cdCanvas* canvas, int x1, int y1, int x2, int y2)
{
  const int interior = canvas->interior_style;
  const int width = canvas->line_width;
  const int style = canvas->line_style;

  const int dx = x2-x1;
  const int dy = y2-y1;

  const double len = hypot(dx,dy);

  const double dnx = dx/len;
  const double dny = dy/len;

  const int w1 = (int)width/2;
  const int w2 = width-w1;

  const int n1x = cdRound( w1*dny);
  const int n1y = cdRound(-w1*dnx);

  const int n2x = cdRound(-w2*dny);
  const int n2y = cdRound( w2*dnx);

  const int p1x = x1 + n1x;
  const int p1y = y1 + n1y;
  const int p2x = x1 + n2x;
  const int p2y = y1 + n2y;
  const int p3x = p2x + dx;
  const int p3y = p2y + dy;
  const int p4x = p1x + dx;
  const int p4y = p1y + dy;

  cdPoint poly[4];

  cdCanvasLineWidth(canvas, 1);
  cdCanvasInteriorStyle(canvas, CD_SOLID);
  cdCanvasLineStyle(canvas, CD_CONTINUOUS);

  poly[0].x = p1x;
  poly[0].y = p1y;
  poly[1].x = p2x;
  poly[1].y = p2y;
  poly[2].x = p3x;
  poly[2].y = p3y;
  poly[3].x = p4x;
  poly[3].y = p4y;

  simPolyFill(canvas->simulation, poly, 4);

  cdCanvasLineWidth(canvas, width);
  cdCanvasInteriorStyle(canvas, interior);
  cdCanvasLineStyle(canvas, style);
}

void simLineThin(cdCanvas* canvas, int x1, int y1, int x2, int y2)
{
  unsigned short ErrorInc, ErrorAcc;
  unsigned short ErrorAccTemp, Weighting;
  int DeltaX, DeltaY, XDir;
  long aa_fgcolor;
  unsigned char alpha = cdAlpha(canvas->foreground), aa_alpha;
  int no_antialias = !(canvas->simulation->antialias);
  unsigned short int ls;
  long fgcolor = canvas->foreground;
  long bgcolor = canvas->background;

  if (simLineStyleNoReset == 2)
    ls = simLineStyleLastBits;
  else
  {
    ls = simLineStyleBitTable[canvas->line_style];

    if (simLineStyleNoReset == 1)
      simLineStyleNoReset = 2;
  }

  /* Make sure p2.y > p1.y */
  if (y1 > y2) 
  {
    _cdSwapInt(y1, y2);
    _cdSwapInt(x1, x2);
  }

  /* Draw the initial pixel, which is always exactly intersected by
      the line and so needs no weighting */
  _cdLineDrawPixel(canvas, x1, y1, ls, fgcolor, bgcolor);
  ls = simRotateLineStyle(ls);

  DeltaX = x2 - x1;
  if (DeltaX >= 0)
    XDir = 1;
  else 
  {
    XDir = -1;
    DeltaX = -DeltaX; /* make DeltaX positive */
  }

  /* Special-case horizontal, vertical, and diagonal lines, which
  require no weighting because they go right through the center of
  every pixel */
  DeltaY = y2 - y1;
  if (DeltaY == 0) 
  {
    /* Horizontal line */
    while (DeltaX-- != 0) 
    {
      x1 += XDir;
      _cdLineDrawPixel(canvas, x1, y1, ls, fgcolor, bgcolor);
      ls = simRotateLineStyle(ls);
    }
    simLineStyleLastBits = ls;
    return;
  }

  if (DeltaX == 0) 
  {
    /* Vertical line */
    do 
    {
      y1++;
      _cdLineDrawPixel(canvas, x1, y1, ls, fgcolor, bgcolor);
      ls = simRotateLineStyle(ls);
    } while (--DeltaY != 0);
    simLineStyleLastBits = ls;
    return;
  }

  if (DeltaX == DeltaY) 
  {
    /* Perfect Diagonal line */
    do 
    {
      x1 += XDir;
      y1++;
      _cdLineDrawPixel(canvas, x1, y1, ls, fgcolor, bgcolor);
      ls = simRotateLineStyle(ls);
    } while (--DeltaY != 0);
    simLineStyleLastBits = ls;
    return;
  }

  /* Line is not horizontal, diagonal, or vertical */

  ErrorAcc = 0;  /* initialize the line error accumulator to 0 */

  /* Is this an X-major or Y-major line? */
  if (DeltaY > DeltaX) 
  {
    /* Y-major line; calculate 16-bit fixed-point fractional part of a
    pixel that X advances each time Y advances 1 pixel, truncating the
    result so that we won't overrun the endpoint along the X axis */
    ErrorInc = (unsigned short)(((unsigned long)DeltaX << 16) / (unsigned long)DeltaY);

    /* Draw all pixels other than the first and last */
    while (--DeltaY) 
    {
      ErrorAccTemp = ErrorAcc;   /* remember currrent accumulated error */
      ErrorAcc += ErrorInc;      /* calculate error for next pixel */
      if (ErrorAcc <= ErrorAccTemp) 
      {
        /* The error accumulator turned over, so advance the X coord */
        x1 += XDir;
      }

      y1++; /* Y-major, so always advance Y */

      Weighting = ErrorAcc >> INTENSITYSHIFT;

      if (no_antialias)
      {
        if (Weighting < 128)
          _cdLineDrawPixel(canvas, x1, y1, ls, fgcolor, bgcolor)
        else
          _cdLineDrawPixel(canvas, x1 + XDir, y1, ls, fgcolor, bgcolor)
        ls = simRotateLineStyle(ls);
      }
      else
      {
        /* The IntensityBits most significant bits of ErrorAcc give us the
        intensity weighting for this pixel, and the complement of the
        weighting for the paired pixel.
        Combine the Weighting with the existing alpha,
        When Weighting is zero alpha must be fully preserved. */
        aa_alpha = ((255-Weighting) * alpha) / 255;
        
        aa_fgcolor = cdEncodeAlpha(fgcolor, aa_alpha);
        _cdLineDrawPixel(canvas, x1, y1, ls, aa_fgcolor, bgcolor);
        aa_fgcolor = cdEncodeAlpha(fgcolor, 255-aa_alpha);
        _cdLineDrawPixel(canvas, x1 + XDir, y1, ls, aa_fgcolor, bgcolor);
        ls = simRotateLineStyle(ls);
      }
    }
    /* Draw the final pixel, which is always exactly intersected by the line
    and so needs no weighting */
    _cdLineDrawPixel(canvas, x2, y2, ls, fgcolor, bgcolor);
    ls = simRotateLineStyle(ls);
  }
  else
  {
    /* It's an X-major line; calculate 16-bit fixed-point fractional part of a
    pixel that Y advances each time X advances 1 pixel, truncating the
    result to avoid overrunning the endpoint along the X axis */
    ErrorInc = (unsigned short)(((unsigned long)DeltaY << 16) / (unsigned long)DeltaX);

    /* Draw all pixels other than the first and last */
    while (--DeltaX) 
    {
      ErrorAccTemp = ErrorAcc;   /* remember currrent accumulated error */
      ErrorAcc += ErrorInc;      /* calculate error for next pixel */
      if (ErrorAcc <= ErrorAccTemp) 
      {
        /* The error accumulator turned over, so advance the Y coord */
        y1++;
      }

      x1 += XDir; /* X-major, so always advance X */

      Weighting = ErrorAcc >> INTENSITYSHIFT;

      if (no_antialias)
      {
        if (Weighting < 128)
          _cdLineDrawPixel(canvas, x1, y1, ls, fgcolor, bgcolor)
        else
          _cdLineDrawPixel(canvas, x1, y1+1, ls, fgcolor, bgcolor)
        ls = simRotateLineStyle(ls);
      }
      else
      {
        /* The IntensityBits most significant bits of ErrorAcc give us the
        intensity weighting for this pixel, and the complement of the
        weighting for the paired pixel.
        Combine the Weighting with the existing alpha,
        When Weighting is zero alpha must be fully preserved. */
        aa_alpha = ((255-Weighting) * alpha) / 255;

        aa_fgcolor = cdEncodeAlpha(fgcolor, aa_alpha);
        _cdLineDrawPixel(canvas, x1, y1, ls, aa_fgcolor, bgcolor);
        aa_fgcolor = cdEncodeAlpha(fgcolor, 255-aa_alpha);
        _cdLineDrawPixel(canvas, x1, y1+1, ls, aa_fgcolor, bgcolor);
        ls = simRotateLineStyle(ls);
      }
    }

    /* Draw the final pixel, which is always exactly intersected by the line
    and so needs no weighting */
    _cdLineDrawPixel(canvas, x2, y2, ls, fgcolor, bgcolor);
    ls = simRotateLineStyle(ls);
  }

  simLineStyleLastBits = ls;
}

void simfLineThin(cdCanvas* canvas, double x1, double y1, double x2, double y2, int *last_xi_a, int *last_yi_a, int *last_xi_b, int *last_yi_b)
{
  double DeltaX, DeltaY, a, b;
  long aa_fgcolor;
  unsigned char alpha = cdAlpha(canvas->foreground), aa_alpha;
  int no_antialias = !(canvas->simulation->antialias);
  int yi, xi, update_a = 1, update_b = 1;
  unsigned short int ls;
  long fgcolor = canvas->foreground;
  long bgcolor = canvas->background;

  if (simLineStyleNoReset == 2)
    ls = simLineStyleLastBits;
  else
  {
    ls = simLineStyleBitTable[canvas->line_style];

    if (simLineStyleNoReset == 1)
      simLineStyleNoReset = 2;
  }

  DeltaX = fabs(x2 - x1);
  DeltaY = fabs(y2 - y1);

  if (DeltaX > 0.0001)
  {
    a = (y1-y2)/(x1-x2);
    b = y1 - a*x1;
  }
  else
  {
    a = 0;
    b = x1;
  }

  /* NOTICE: all the complexity of this function 
  is related to check and update the previous point */

  /* Is this an X-major or Y-major line? */
  if (DeltaY > DeltaX) 
  {
    /* Increment in Y */
    int y1i = _cdRound(y1), 
        y2i = _cdRound(y2);
    int yi_first = y1i;
    int yi_last = y2i, xi_last;

    if (y1i > y2i)
      _cdSwapInt(y1i, y2i);

    for (yi = y1i; yi <= y2i; yi++)
    {
      double x;
      if (a)
        x = (yi - b)/a;
      else
        x = b;

      xi = (int)floor(x);

      /* if at the last pixel, store the return value */
      if (yi == yi_last)
        xi_last = xi;

      /* Combine the Weighting with the existing alpha,
      When Weighting is zero alpha must be fully preserved. */
      aa_alpha = (int)((1.0-(x - xi)) * alpha);

      if (no_antialias)
      {
        if (aa_alpha > 128)
          _cdLineDrawPixel(canvas, xi, yi, ls, fgcolor, bgcolor)
        else
          _cdLineDrawPixel(canvas, xi+1, yi, ls, fgcolor, bgcolor)
      }
      else
      {
        if (yi == yi_first)
        {
          if (yi == yi_last)  /* one pixel only */
          {
            update_a = 0;
            update_b = 0;
          }

          /* if at first, compare with the last two previously drawn */
          /* if the new is equal to the previous, do NOT draw */
          if ((xi != *last_xi_a || yi != *last_yi_a) &&
              (xi != *last_xi_b || yi != *last_yi_b))
          {
            aa_fgcolor = cdEncodeAlpha(fgcolor, aa_alpha);
            _cdLineDrawPixel(canvas, xi, yi, ls, aa_fgcolor, bgcolor);

            if (yi == yi_last)  /* one pixel only */
              update_a = 1;
          }

          if ((xi+1 != *last_xi_a || yi != *last_yi_a) &&
              (xi+1 != *last_xi_b || yi != *last_yi_b))
          {
            aa_fgcolor = cdEncodeAlpha(fgcolor, 255-aa_alpha);
            _cdLineDrawPixel(canvas, xi+1, yi, ls, aa_fgcolor, bgcolor);

            if (yi == yi_last)  /* one pixel only */
              update_b = 1;
          }
        }
        else
        {
          aa_fgcolor = cdEncodeAlpha(fgcolor, aa_alpha);
          _cdLineDrawPixel(canvas, xi, yi, ls, aa_fgcolor, bgcolor);
          aa_fgcolor = cdEncodeAlpha(fgcolor, 255-aa_alpha);
          _cdLineDrawPixel(canvas, xi+1, yi, ls, aa_fgcolor, bgcolor);
        }
      }

      ls = simRotateLineStyle(ls);
    }

    if (update_a)
    {
      *last_xi_a = xi_last;
      *last_yi_a = yi_last;
    }
    if (update_b)
    {
      *last_xi_b = xi_last+1;
      *last_yi_b = yi_last;
    }
  }
  else
  {
    /* Increment in X */
    int x1i = _cdRound(x1), 
        x2i = _cdRound(x2);
    int xi_first = x1i;
    int xi_last = x2i, yi_last;

    if (x1i > x2i)
      _cdSwapInt(x1i, x2i);

    for (xi = x1i; xi <= x2i; xi++)
    {
      double y = a*xi + b;
      yi = (int)floor(y);

      /* if at the last pixel, store the return value */
      if (xi == xi_last)
        yi_last = yi;

      /* Combine the Weighting with the existing alpha,
      When Weighting is zero alpha must be fully preserved. */
      aa_alpha = (int)((1.0-(y - yi)) * alpha);

      if (no_antialias)
      {
        if (aa_alpha > 128)
          _cdLineDrawPixel(canvas, xi, yi, ls, fgcolor, bgcolor)
        else
          _cdLineDrawPixel(canvas, xi, yi+1, ls, fgcolor, bgcolor)
      }
      else
      {
        if (xi == xi_first)
        {
          if (xi == xi_last)  /* one pixel only */
          {
            update_a = 0;
            update_b = 0;
          }

          /* if at first, compare with the last to draw */
          /* if new is equal to the previous, do NOT draw */
          if ((xi != *last_xi_a || yi != *last_yi_a) &&
              (xi != *last_xi_b || yi != *last_yi_b))
          {
            aa_fgcolor = cdEncodeAlpha(fgcolor, aa_alpha);
            _cdLineDrawPixel(canvas, xi, yi, ls, aa_fgcolor, bgcolor);

            if (xi == xi_last) /* one pixel only */
              update_a = 1;
          }

          if ((xi != *last_xi_a || yi+1 != *last_yi_a) &&
              (xi != *last_xi_b || yi+1 != *last_yi_b))
          {
            aa_fgcolor = cdEncodeAlpha(fgcolor, 255-aa_alpha);
            _cdLineDrawPixel(canvas, xi, yi+1, ls, aa_fgcolor, bgcolor);

            if (xi == xi_last) /* one pixel only */
              update_b = 1;
          }
        }
        else
        {
          aa_fgcolor = cdEncodeAlpha(fgcolor, aa_alpha);
          _cdLineDrawPixel(canvas, xi, yi, ls, aa_fgcolor, bgcolor);
          aa_fgcolor = cdEncodeAlpha(fgcolor, 255-aa_alpha);
          _cdLineDrawPixel(canvas, xi, yi+1, ls, aa_fgcolor, bgcolor);
        }
      }

      ls = simRotateLineStyle(ls);
    }

    if (update_a)
    {
      *last_xi_a = xi_last;
      *last_yi_a = yi_last;
    }
    if (update_b)
    {
      *last_xi_b = xi_last;
      *last_yi_b = yi_last+1;
    }
  }

  simLineStyleLastBits = ls;
}
