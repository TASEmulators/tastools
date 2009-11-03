/** \file
 * \brief PPlot component for Iup.
 *
 * See Copyright Notice in "iup.h"
 */
 
#ifndef __IUPPPLOT_H 
#define __IUPPPLOT_H

#ifdef __cplusplus
extern "C" {
#endif

/* Initialize PPlot widget class */
void IupPPlotOpen(void);

/* Create an PPlot widget instance */
Ihandle* IupPPlot(void);

/* Add dataset to plot */
void IupPPlotBegin(Ihandle *ih, int strXdata);
void IupPPlotAdd(Ihandle *ih, float x, float y);
void IupPPlotAddStr(Ihandle *ih, const char* x, float y);
int IupPPlotEnd(Ihandle *ih);

void IupPPlotInsertStr(Ihandle *ih, int index, int sample_index, const char* x, float y);
void IupPPlotInsert(Ihandle *ih, int index, int sample_index, float x, float y);

/* convert from plot coordinates to pixels */
void IupPPlotTransform(Ihandle* ih, float x, float y, int *ix, int *iy);

/* Plot on the given device. Uses a "cdCanvas*". */
void IupPPlotPaintTo(Ihandle *ih, void *cnv);


#ifdef __cplusplus
}
#endif

#endif
