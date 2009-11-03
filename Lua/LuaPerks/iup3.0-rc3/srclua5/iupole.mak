PROJNAME = iup
LIBNAME = iupluaole51
OPT = YES
DEF_FILE = iupluaole5.def

IUP := ..

DEFINES = IUPLUA_USELOH

USE_IUPLUA = Yes
LIBS = iupole
USE_LUA51 = Yes
NO_LUALINK = Yes

LOHDIR = loh
SRCLUA = olecontrol.lua
GC = $(addsuffix .c, $(basename $(SRCLUA)))
GC := $(addprefix il_, $(GC))

$(GC) : il_%.c : %.lua generator.lua
	$(LUABIN) generator.lua $<

SRC	= $(GC)
