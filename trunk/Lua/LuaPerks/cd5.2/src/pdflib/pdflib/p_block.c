/*---------------------------------------------------------------------------*
 |              PDFlib - A library for generating PDF on the fly             |
 +---------------------------------------------------------------------------+
 | Copyright (c) 1997-2007 Thomas Merz and PDFlib GmbH. All rights reserved. |
 +---------------------------------------------------------------------------+
 |                                                                           |
 |    This software is subject to the PDFlib license. It is NOT in the       |
 |    public domain. Extended versions and commercial licenses are           |
 |    available, please check http://www.pdflib.com.                         |
 |                                                                           |
 *---------------------------------------------------------------------------*/

/* $Id: p_block.c,v 1.1 2008/10/17 06:11:49 scuri Exp $
 *
 * Block processing routines (require the PDI library)
 *
 */

#define P_BLOCK_C

#include "p_intern.h"
#include "p_color.h"
#include "p_font.h"
#include "p_image.h"
#include "p_defopt.h"

