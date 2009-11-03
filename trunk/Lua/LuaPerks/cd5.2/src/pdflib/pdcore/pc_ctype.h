/*---------------------------------------------------------------------------*
 |           PDFlib - A library for generating PDF on the fly                |
 +---------------------------------------------------------------------------+
 |          Copyright (c) 1997-2006 PDFlib GmbH. All rights reserved.        |
 *---------------------------------------------------------------------------*
 |          Proprietary source code -- do not redistribute!                  |
 *---------------------------------------------------------------------------*/

/* $Id: pc_ctype.h,v 1.1 2008/10/17 06:10:43 scuri Exp $ */

#ifndef PC_CTYPE_H_INCLUDED
#define PC_CTYPE_H_INCLUDED

#include "pc_util.h"

/* these are the locale-free replacements for the standard library
** isXXX() functions. use the macros below, DO NOT use the pdc__isXXX()
** functions directly.
*/
#define pdc_isalnum(c)		pdc__isalnum((pdc_byte) (c))
#define pdc_isalpha(c)		pdc__isalpha((pdc_byte) (c))
#define pdc_isdigit(c)		pdc__isdigit((pdc_byte) (c))
#define pdc_islower(c)		pdc__islower((pdc_byte) (c))
#define pdc_isprint(c)		pdc__isprint((pdc_byte) (c))
#define pdc_ispunct(c)		pdc__ispunct((pdc_byte) (c))
#define pdc_isspace(c)		pdc__isspace((pdc_byte) (c))
#define pdc_isupper(c)		pdc__isupper((pdc_byte) (c))
#define pdc_isxdigit(c)		pdc__isxdigit((pdc_byte) (c))

#define pdc_tolower(c)		pdc__tolower((pdc_byte) (c))
#define pdc_toupper(c)		pdc__toupper((pdc_byte) (c))

pdc_bool pdc__isalnum(pdc_byte c);
pdc_bool pdc__isalpha(pdc_byte c);
pdc_bool pdc__isdigit(pdc_byte c);
pdc_bool pdc__islower(pdc_byte c);
pdc_bool pdc__isprint(pdc_byte c);
pdc_bool pdc__ispunct(pdc_byte c);
pdc_bool pdc__isspace(pdc_byte c);
pdc_bool pdc__isupper(pdc_byte c);
pdc_bool pdc__isxdigit(pdc_byte c);

pdc_byte pdc__tolower(pdc_byte c);
pdc_byte pdc__toupper(pdc_byte c);


/* these macros are for the various flavors of the token scanner. they
** expect ASCII input even on EBCDIC platforms (thus the "_a" suffix),
** and they implement special rules for PDF character classification.
*/
#define pdc_isalpha_a(c)	pdc__isalpha_a((pdc_byte) (c))
#define pdc_isdecdt_a(c)	pdc__isdecdt_a((pdc_byte) (c))
#define pdc_isdelim_a(c)	pdc__isdelim_a((pdc_byte) (c))
#define pdc_ishexdt_a(c)	pdc__ishexdt_a((pdc_byte) (c))
#define pdc_islower_a(c)	pdc__islower_a((pdc_byte) (c))
#define pdc_isnum0_a(c)		pdc__isnum0_a((pdc_byte) (c))
#define pdc_isoctdt_a(c)	pdc__isoctdt_a((pdc_byte) (c))

#define pdc_isregular_a(c)	\
	((c) != -1 && !pdc__isspecial_a((pdc_byte) (c)))

#define pdc_isspace_a(c)	pdc__isspace_a((pdc_byte) (c))
#define pdc_isspecial_a(c)	pdc__isspecial_a((pdc_byte) (c))
#define pdc_isupper_a(c)	pdc__isupper_a((pdc_byte) (c))

pdc_bool pdc__isalpha_a(pdc_byte c);
pdc_bool pdc__isdecdt_a(pdc_byte c);
pdc_bool pdc__isdelim_a(pdc_byte c);
pdc_bool pdc__ishexdt_a(pdc_byte c);
pdc_bool pdc__islower_a(pdc_byte c);
pdc_bool pdc__isnum0_a(pdc_byte c);
pdc_bool pdc__isoctdt_a(pdc_byte c);
pdc_bool pdc__isspace_a(pdc_byte c);
pdc_bool pdc__isspecial_a(pdc_byte c);
pdc_bool pdc__isupper_a(pdc_byte c);

#endif	/* PC_CTYPE_H_INCLUDED */
