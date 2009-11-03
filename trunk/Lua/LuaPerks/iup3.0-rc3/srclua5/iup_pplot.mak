PROJNAME = iup
LIBNAME = iuplua_pplot51
OPT = YES
DEF_FILE = iuplua_pplot5.def

IUP := ..

DEFINES = IUPLUA_USELOH

USE_IUP3 = Yes
USE_IUPLUA = Yes
USE_CDLUA = Yes
LIBS = iup_pplot

USE_LUA51 = Yes
NO_LUALINK = Yes

LOHDIR = loh
SRCLUA = pplot.lua
GC := $(addsuffix .c, $(basename $(SRCLUA)))
GC := $(addprefix il_, $(GC))

$(GC) : il_%.c : %.lua generator.lua
	$(LUABIN) generator.lua $<

SRC := iuplua_pplot.c $(GC)
