PROJNAME = iup
LIBNAME = iup
OPT = YES

#ifdef DBG
  DEFINES += IUP_ASSERT
#endif  

INCLUDES = ../include .

SRC = iup_array.c iup_callback.c iup_dlglist.c iup_attrib.c iup_focus.c iup_font.c \
      iup_globalattrib.c iup_object.c iup_key.c iup_layout.c iup_ledlex.c iup_names.c iup_open.c \
      iup_ledparse.c iup_predial.c iup_register.c iup_scanf.c iup_show.c iup_str.c iup_table.c \
      iup_func.c iup_childtree.c iup.c iup_classattrib.c iup_dialog.c iup_assert.c iup_canvas.c \
      iup_messagedlg.c iup_timer.c iup_image.c iup_label.c iup_fill.c iup_zbox.c \
      iup_colordlg.c iup_fontdlg.c iup_filedlg.c iup_strmessage.c iup_menu.c iup_frame.c \
      iup_user.c iup_button.c iup_radio.c iup_toggle.c iup_progressbar.c iup_text.c iup_val.c \
      iup_box.c iup_hbox.c iup_vbox.c iup_cbox.c iup_class.c iup_classbase.c iup_maskmatch.c \
      iup_mask.c iup_maskparse.c iup_tabs.c iup_spin.c iup_list.c iup_getparam.c \
      iup_sbox.c iup_normalizer.c iup_tree.c

ifdef USE_GTK
  ifndef GTK_DEFAULT
    # Build GTK version in IRIX,SunOS,AIX,Win32
    LIBNAME := iupgtk
  endif

  DEFINES += GTK_DISABLE_DEPRECATED
  INCLUDES += gtk
    SRC += gtk/iupgtk_common.c gtk/iupgtk_focus.c gtk/iupgtk_font.c gtk/iupgtk_clipboard.c \
           gtk/iupgtk_globalattrib.c gtk/iupgtk_key.c gtk/iupgtk_tips.c \
           gtk/iupgtk_loop.c gtk/iupgtk_open.c gtk/iupgtk_messagedlg.c \
           gtk/iupgtk_dialog.c gtk/iupgtk_timer.c gtk/iupgtk_image.c gtk/iupgtk_label.c \
           gtk/iupgtk_colordlg.c gtk/iupgtk_fontdlg.c gtk/iupgtk_filedlg.c  \
           gtk/iupgtk_button.c gtk/iupgtk_toggle.c gtk/iupgtk_progressbar.c \
           gtk/iupgtk_text.c gtk/iupgtk_val.c gtk/iupgtk_frame.c gtk/iupgtk_canvas.c \
           gtk/iupgtk_tabs.c gtk/iupgtk_menu.c gtk/iupgtk_list.c gtk/iupgtk_tree.c
           
  ifneq ($(findstring Win, $(TEC_SYSNAME)), )
    DEFINES += _WIN32_WINNT=0x0500 _WIN32_IE=0x0500 WINVER=0x0500 NOTREEVIEW
    SRC += win/iupwindows_main.c win/iupwindows_help.c win/iupwindows_info.c
  else
    SRC += gtk/iupgtk_help.c mot/iupunix_info.c
  endif
  
  ifdef USE_HILDON
    DEFINES += HILDON
    INCLUDES += /usr/include/hildon-1
    LIBS += hildon-1
  endif
else
  ifneq ($(findstring Win, $(TEC_SYSNAME)), )
  
    SRC += win/iupwin_common.c win/iupwin_brush.c win/iupwin_focus.c win/iupwin_font.c \
           win/iupwin_globalattrib.c win/iupwin_handle.c win/iupwin_key.c \
           win/iupwin_loop.c win/iupwin_open.c win/iupwin_tips.c win/iupwin_info.c \
           win/iupwin_dialog.c win/iupwin_messagedlg.c win/iupwin_timer.c \
           win/iupwin_image.c win/iupwin_label.c win/iupwin_canvas.c win/iupwin_frame.c \
           win/iupwin_colordlg.c win/iupwin_fontdlg.c win/iupwin_filedlg.c \
           win/iupwin_button.c win/iupwin_draw.c win/iupwin_toggle.c win/iupwin_clipboard.c \
           win/iupwin_progressbar.c win/iupwin_text.c win/iupwin_val.c \
           win/iupwin_tabs.c win/iupwin_menu.c win/iupwin_list.c win/iupwin_tree.c
           
    SRC += win/iupwindows_main.c win/iupwindows_help.c win/iupwindows_info.c
  
    INCLUDES += win
    DEFINES += _WIN32_WINNT=0x0500 _WIN32_IE=0x0500 WINVER=0x0500 NOTREEVIEW
  else
    ifdef GTK_DEFAULT
      # Build Motif version in Linux,Darwin,FreeBSD
      LIBNAME := iupmot
    endif  
    
    SRC += mot/iupmot_common.c mot/iupmot_color.c mot/iupmot_focus.c mot/iupmot_font.c \
           mot/iupmot_key.c mot/iupmot_loop.c mot/iupmot_open.c mot/iupmot_tips.c \
           mot/iupmot_globalattrib.c mot/iupmot_dialog.c mot/iupmot_messagedlg.c \
           mot/iupmot_timer.c mot/iupmot_image.c mot/iupmot_label.c mot/iupmot_canvas.c \
           mot/iupmot_colordlg.c mot/iupmot_fontdlg.c mot/iupmot_filedlg.c mot/iupmot_frame.c \
           mot/iupmot_button.c mot/iupmot_toggle.c mot/iupmot_progressbar.c mot/iupmot_clipboard.c \
           mot/iupmot_text.c mot/iupmot_val.c mot/iupmot_tabs.c mot/iupmot_menu.c \
           mot/iupmot_list.c mot/iupmot_tree.c
           
    SRC += mot/iupunix_help.c mot/iupunix_info.c
  
    INCLUDES += mot
    USE_MOTIF=Yes   
  endif
endif

ifeq "$(TEC_SYSNAME)" "SunOS"
  # Necessary or the fileopen will not work in SunOS (needs to be retested)
  #DEFINES += NO_PATH_MODE_RELATIVE
endif

ifneq ($(findstring dll, $(TEC_UNAME)), )
  DEFINES += IUP_DLL
  SRC += iup_dll.rc
  DEF_FILE = iup.def
endif

ifeq "$(TEC_UNAME)" "vc6"
  # Necessary because VC6 has an old WinSDK
  #WINSDK = d:/lng/vc7/PlatformSDK
  #INCLUDES += $(WINSDK)/include
  #LDIR = $(WINSDK)/lib
endif

ifeq "$(TEC_UNAME)" "dll"
  # Necessary because VC6 has an old WinSDK
  #WINSDK = d:/lng/vc7/PlatformSDK
  #INCLUDES += $(WINSDK)/include
  #LDIR = $(WINSDK)/lib
endif

ifeq "$(TEC_UNAME)" "owc1"
  # Necessary or IUP will not work in Open Watcom
  DBG=Yes
endif
