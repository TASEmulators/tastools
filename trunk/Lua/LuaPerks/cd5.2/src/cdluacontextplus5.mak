PROJNAME = cd
LIBNAME = cdluacontextplus51

OPT = YES

DEFINES = CD_NO_OLD_INTERFACE
SRCDIR = lua5
SRC = cdluacontextplus5.c
DEF_FILE = cdluacontextplus5.def

LIBS = cdcontextplus

USE_LUA51 = Yes
NO_LUALINK = Yes
USE_CD = YES
USE_CDLUA = YES
CD = ..
