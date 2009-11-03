PROJNAME = im
LIBNAME = imlua51

OPT = YES

LOHDIR = lua5/loh
SRC = lua5/imlua.c lua5/imlua_aux.c lua5/imlua_convert.c lua5/imlua_file.c lua5/imlua_image.c lua5/imlua_palette.c lua5/imlua_util.c
DEF_FILE = lua5/imlua.def

SRCLUA = lua5/im_image.lua lua5/im_convert.lua
SRCLUADIR = lua5

INCLUDES = lua5

USE_IM = YES
USE_LUA51 = YES
NO_LUALINK = Yes
IM = ..
