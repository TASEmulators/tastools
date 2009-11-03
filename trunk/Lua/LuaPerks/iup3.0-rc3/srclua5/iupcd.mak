PROJNAME = iup
LIBNAME = iupluacd51
OPT = YES

DEFINES = CD_NO_OLD_INTERFACE
SRC = iuplua_cd.c
DEF_FILE = iupluacd5.def

# Can not use USE_IUPLUA because Tecmake will include "iupluacd51" in linker

INCLUDES = ../include
LIBS = iuplua51
LDIR = ../lib/$(TEC_UNAME)

IUP := ..

USE_CD = YES
USE_IUP3 = YES
USE_LUA51 = Yes
NO_LUALINK = Yes
USE_CDLUA = YES
