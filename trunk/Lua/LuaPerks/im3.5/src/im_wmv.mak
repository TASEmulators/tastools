PROJNAME = im
LIBNAME = im_wmv
OPT = YES

SRC = im_format_wmv.cpp
                                       
ifneq ($(findstring _64, $(TEC_UNAME)), )
  WMFSDK = d:/lng/wmfsdk95
  INCLUDES = $(WMFSDK)/include
else
#  WMFSDK = d:/lng/wmfsdk11
#  EXTRAINCS = $(WMFSDK)/include
  WMFSDK = d:/lng/wmfsdk9
  INCLUDES = $(WMFSDK)/include
endif
  
DEFINES = _CRT_NON_CONFORMING_SWPRINTFS                                     

LDIR = $(WMFSDK)/lib
LIBS = wmvcore

USE_IM = Yes
IM = ..
