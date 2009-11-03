PROJNAME = iup
LIBNAME  = iupgl
OPT = YES

ifdef DBG
  DEFINES += IUP_ASSERT
endif  

INCLUDES = ../include ../src
LDIR = ../lib/$(TEC_UNAME)  
LIBS = iup

USE_OPENGL = Yes

ifneq ($(findstring Win, $(TEC_SYSNAME)), )
  SRC = iup_glcanvas_win.c
else
  SRC = iup_glcanvas_x.c
  USE_X11 = Yes
endif
