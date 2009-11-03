PROJNAME = im
LIBNAME = imlua_fftw51
DEF_FILE = imlua_fftw.def

OPT = YES

LOHDIR = lua5/loh
SRC = lua5/imlua_fftw.c
DEF_FILE = lua5/imlua_fftw.def

SRCLUA = lua5/im_fftw.lua
SRCLUADIR = lua5

LIBS = im_fftw
INCLUDES = lua5

USE_IMLUA = YES
USE_LUA51 = YES
NO_LUALINK = Yes
IM = ..
