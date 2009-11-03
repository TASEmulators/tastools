PROJNAME = cd
LIBNAME = cdluapdf51

OPT = YES

DEFINES = CD_NO_OLD_INTERFACE
SRCDIR = lua5
SRC = cdluapdf5.c
DEF_FILE = cdluapdf5.def

LIBS = cdpdf

USE_LUA51 = Yes
NO_LUALINK = Yes
USE_CD = YES
USE_CDLUA = YES
CD = ..
