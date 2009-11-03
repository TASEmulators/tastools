PROJNAME = im
LIBNAME = imlua_process51

OPT = YES

LOHDIR = lua5/loh
SRC = lua5/imlua_process.c lua5/imlua_kernel.c
DEF_FILE = lua5/imlua_process.def

SRCLUA = lua5/im_process.lua
SRCLUADIR = lua5

LIBS = im_process
INCLUDES = lua5

USE_IMLUA = YES
USE_LUA51 = YES
NO_LUALINK = Yes
IM = ..
