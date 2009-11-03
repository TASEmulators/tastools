PROJNAME = cd
LIBNAME = cd
OPT = YES   

DEFINES = CD_NO_OLD_INTERFACE

SRCINTCGM = circle.c ellipse.c intcgm1.c \
  intcgm2.c intcgm4.c intcgm6.c list.c \
  sism.c tparse.c bparse.c 
SRCINTCGM  := $(addprefix intcgm/, $(SRCINTCGM))

SRCWIN32 = cdwclp.c cdwemf.c cdwimg.c cdwin.c cdwnative.c cdwprn.c cdwwmf.c wmf_emf.c cdwdbuf.c cdwdib.c
SRCWIN32  := $(addprefix win32/, $(SRCWIN32))

SRCSIM := cdfontex.c sim.c cd_truetype.c sim_other.c sim_primitives.c sim_text.c sim_linepolyfill.c
SRCSIM  := $(addprefix sim/, $(SRCSIM))

SRCX11 = cdx11.c cdxclp.c cdximg.c cdxnative.c cdxdbuf.c xvertex.c
SRCX11  := $(addprefix x11/, $(SRCX11))

SRCDRV = cddgn.c cdcgm.c cgm.c cddxf.c cdirgb.c cdmf.c cdps.c cdpicture.c cddebug.c
SRCDRV  := $(addprefix drv/, $(SRCDRV))

SRCNULL = cd0prn.c cd0emf.c cd0wmf.c
SRCNULL  := $(addprefix drv/, $(SRCNULL))

SRCCOMM = cd.c wd.c wdhdcpy.c rgb2map.c cd_vectortext.c cd_active.c \
          cd_attributes.c cd_bitmap.c cd_image.c cd_primitives.c cd_text.c cd_util.c
      
SRC = $(SRCCOMM) $(SRCINTCGM) $(SRCDRV) $(SRCSIM)

ifneq ($(findstring Win, $(TEC_SYSNAME)), )
  SRC += $(SRCWIN32)
  LIBS = freetype6
else
  SRC += $(SRCNULL) $(SRCX11)
  USE_X11 = Yes
  LIBS = freetype
endif

ifneq ($(findstring dll, $(TEC_UNAME)), )
  SRC += cd.rc
endif

LDIR = ../lib/$(TEC_UNAME)

INCLUDES = . drv x11 win32 intcgm freetype2 sim ../include
