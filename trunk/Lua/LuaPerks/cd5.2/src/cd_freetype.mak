PROJNAME = cd
LIBNAME = freetype
OPT = YES   

# Changes to freetype-2.3.5 (search for CDLIB): 
#         freetype2\freetype\config\ftconfig.h 
#   were changed to configure the library.
SRC  := \
  autofit/autofit.c bdf/bdf.c cff/cff.c base/ftbase.c cache/ftcache.c \
  gzip/ftgzip.c lzw/ftlzw.c gxvalid/gxvalid.c otvalid/otvalid.c pcf/pcf.c \
  pfr/pfr.c psaux/psaux.c pshinter/pshinter.c psnames/psnames.c raster/raster.c \
  sfnt/sfnt.c smooth/smooth.c truetype/truetype.c type1/type1.c cid/type1cid.c \
  type42/type42.c winfonts/winfnt.c \
  base/ftapi.c base/ftbbox.c base/ftbdf.c base/ftbitmap.c base/ftdebug.c \
  base/ftgasp.c base/ftglyph.c base/ftgxval.c base/ftinit.c base/ftlcdfil.c \
  base/ftmm.c base/ftotval.c base/ftpatent.c base/ftpfr.c base/ftstroke.c \
  base/ftsynth.c base/ftsystem.c base/fttype1.c base/ftwinfnt.c base/ftxf86.c
SRC := $(addprefix freetype2/, $(SRC))

DEFINES += FT2_BUILD_LIBRARY
INCLUDES = freetype2
DEF_FILE = cd_freetype.def

ifneq ($(findstring dll, $(TEC_UNAME)), )
  SRC += cd_freetype.rc
endif

ifneq ($(findstring Win, $(TEC_SYSNAME)), )
  # To be compatible with the existing DLLs
  LIBNAME = freetype6
endif

ifneq ($(findstring gcc, $(TEC_UNAME)), )
  LIBNAME = freetype
endif

ifneq ($(findstring bc5, $(TEC_UNAME)), )
  FLAGS = -w-8004
endif

ifneq ($(findstring Darwin, $(TEC_UNAME)), )
  DEFINES += DARWIN_NO_CARBON
endif
