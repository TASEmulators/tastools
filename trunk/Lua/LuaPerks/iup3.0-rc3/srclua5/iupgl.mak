PROJNAME = iup
LIBNAME = iupluagl51
OPT = YES

DEF_FILE = iupluagl5.def
DEFINES = IUPLUA_USELOH

IUP := ..

# Can not use USE_IUPLUA because Tecmake will include "iupluagl51" in linker

USE_IUP3 = Yes
USE_OPENGL = Yes
LIBS = iuplua51

USE_LUA51 = Yes
NO_LUALINK = Yes

LOHDIR = loh
SRCLUA = glcanvas.lua
GC = $(addsuffix .c, $(basename $(SRCLUA)))
GC := $(addprefix il_, $(GC))

$(GC) : il_%.c : %.lua generator.lua
	$(LUABIN) generator.lua $<

SRC	= iuplua_glcanvas.c $(GC)
