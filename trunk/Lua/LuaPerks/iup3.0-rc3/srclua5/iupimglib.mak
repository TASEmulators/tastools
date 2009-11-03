PROJNAME = iup
LIBNAME = iupluaimglib51
OPT = YES

SRC = iuplua_imglib.c
DEF_FILE = iupluaimglib5.def

INCLUDES = ../include
LIBS = iupimglib
LDIR = ../lib/$(TEC_UNAME)

IUP := ..

USE_IUP3 = Yes
USE_IUPLUA = Yes
USE_LUA51 = Yes
NO_LUALINK = Yes
