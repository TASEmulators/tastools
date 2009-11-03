PROJNAME = iup
LIBNAME = iupimglib
#OPT = YES

INCLUDES = ../include ../src

SRC = iup_image_library.c

LIBS = iup
LDIR = ../lib/$(TEC_UNAME)

ifneq ($(findstring AIX, $(TEC_UNAME)), )
  DEFINES += IUP_IMGLIB_DUMMY
endif
