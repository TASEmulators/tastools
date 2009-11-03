PROJNAME = iup
APPNAME = iuplua51
APPTYPE = console

LOHDIR = loh
SRCLUA = console5.lua
SRC = iup_lua51.c


# Disable strip
STRIP = 
# Optimize
OPT = YES      

# IM and IupPPlot uses C++
LINKER = $(CPPC)

ifdef USE_GTK
  APPNAME = iuplua51gtk
endif

ifdef USE_MOTIF
  APPNAME = iuplua51mot
endif

ifdef DBG
  # Statically link everything only when debugging
  IUP := ..
  USE_IUPLUA = Yes
  USE_IUP3 = Yes
  USE_STATIC = Yes
  USE_LUA51 = Yes
  
  DEFINES = USE_STATIC

  ifeq "$(TEC_UNAME)" "SunOS510x86"
    IUPLUA_NO_GL = Yes
  endif
    
  #IUPLUA_NO_GL = Yes
  ifndef IUPLUA_NO_GL 
    USE_OPENGL = Yes
  else
    DEFINES += IUPLUA_NO_GL
  endif

  #IUPLUA_NO_CD = Yes
  ifndef IUPLUA_NO_CD 
    USE_CDLUA = Yes
    USE_IUPCONTROLS = Yes
    ifneq ($(findstring Win, $(TEC_SYSNAME)), )
      LIBS += iuplua_pplot$(LIBLUASUFX) iup_pplot
    else
      IUPLIB = $(IUP)/lib/$(TEC_UNAME)
      SLIB += $(IUPLIB)/libiuplua_pplot$(LIBLUASUFX).a $(IUPLIB)/libiup_pplot.a
    endif
      
    ifndef IUPLUA_NO_IM
      ifneq ($(findstring Win, $(TEC_SYSNAME)), )
        LIBS += cdluaim$(LIBLUASUFX)
      else
        CDLIB = $(CD)/lib/$(TEC_UNAME)
        SLIB += $(CDLIB)/libcdluaim$(LIBLUASUFX).a
      endif
    endif
    ifneq ($(findstring Win, $(TEC_SYSNAME)), )
      USE_GDIPLUS=Yes
    else
  #    USE_XRENDER=Yes
    endif
  else
    DEFINES += IUPLUA_NO_CD
  endif

  #IUPLUA_NO_IM = Yes
  ifndef IUPLUA_NO_IM
    USE_IMLUA = Yes
    
    ifneq ($(findstring Win, $(TEC_SYSNAME)), )
      LIBS += imlua_process$(LIBLUASUFX) iupluaim$(LIBLUASUFX) im_process iupim
    else
      IUPLIB = $(IUP)/lib/$(TEC_UNAME)
      IMLIB = $(IM)/lib/$(TEC_UNAME)
      SLIB +=  $(IMLIB)/libimlua_process$(LIBLUASUFX).a $(IUPLIB)/libiupluaim$(LIBLUASUFX).a $(IMLIB)/libim_process.a $(IUPLIB)/libiupim.a
    endif
    
  else
    DEFINES += IUPLUA_NO_IM
  endif

  IUPLUA_IMGLIB = Yes
  ifdef IUPLUA_IMGLIB
    DEFINES += IUPLUA_IMGLIB
    ifneq ($(findstring Win, $(TEC_SYSNAME)), )
      LIBS += iupluaimglib$(LIBLUASUFX) iupimglib
    else
      IUPLIB = $(IUP)/lib/$(TEC_UNAME)
      SLIB += $(IUPLIB)/libiupluaimglib$(LIBLUASUFX).a $(IUPLIB)/libiupimglib.a
    endif
  endif
else
  ifneq ($(findstring Win, $(TEC_SYSNAME)), )
    # Dinamically link in Windows, when not debugging
    # Must call "tecmake dll8"
    USE_LUA51 = Yes
    USE_DLL = Yes
    GEN_MANIFEST = No
  else
    # In UNIX Lua is always statically linked, late binding is used.
    USE_STATIC = Yes
    USE_LUA51 = Yes
  endif
endif


ifneq ($(findstring Win, $(TEC_SYSNAME)), )
  SLIB += setargv.obj
  SRC += iuplua5.rc
endif

ifneq ($(findstring cygw, $(TEC_UNAME)), )
  LDFLAGS = -s
  LIBS += readline history
endif

ifneq ($(findstring Linux, $(TEC_UNAME)), )
  LIBS += dl 
  #To allow late binding
  LFLAGS = -Wl,-E
  LIBS += readline history curses ncurses
endif

ifneq ($(findstring BSD, $(TEC_UNAME)), )
  #To allow late binding
  LFLAGS = -Wl,-E
  LIBS += readline history curses ncurses
endif

ifneq ($(findstring SunOS, $(TEC_UNAME)), )
  LIBS += dl
endif

ifneq ($(findstring AIX, $(TEC_UNAME)), )
  FLAGS  += -mminimal-toc
  OPTFLAGS = -mminimal-toc -ansi -pedantic 
  LFLAGS = -Xlinker "-bbigtoc"
endif

ifeq ($(TEC_UNAME), vc8)
  ifdef DBG
    #debug info not working for vc8 linker
    define DBG
    endef
  endif
endif         
