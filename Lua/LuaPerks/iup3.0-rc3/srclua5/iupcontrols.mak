PROJNAME = iup
LIBNAME = iupluacontrols51
OPT = YES
DEF_FILE = iupluacontrols5.def

IUP := ..

DEFINES = IUPLUA_USELOH

USE_IUP3 = Yes
USE_IUPLUA = Yes
USE_CDLUA = Yes
LIBS = iupcontrols

USE_LUA51 = Yes
NO_LUALINK = Yes

LOHDIR = loh
SRCLUA = dial.lua gauge.lua colorbrowser.lua colorbar.lua matrix.lua cells.lua
GC := $(addsuffix .c, $(basename $(SRCLUA)))
GC := $(addprefix il_, $(GC))

$(GC) : il_%.c : %.lua generator.lua
	$(LUABIN) generator.lua $<

SRC := iuplua_controls.c il_mask.c il_matrix_aux.c $(GC)
