VERSION=1.6.1.1
ARCHDIR=archives/
ARCHNAME=nesmock-$(VERSION)
ARCHFILES=\
	main.cc \
	strfun.hh utility.hh \
	famtasia.hh \
	fceu.hh \
	virtuanes.hh \
	nintendulator.hh \
	nesticle.hh \
	md5.cc md5.hh md5.h \
	\
	COPYING boxart.png \
        Makefile.sets progdesc.php

include Makefile.sets

OPTIM=-O0

PREFIX=/usr/local
BINDIR=$(PREFIX)/bin
INSTALLPROGS=nesmock
INSTALL=install

WARNINGS += -Wno-unused

# Building for Windows:
#HOST=/opt/xmingw/bin/i386-mingw32msvc-
#LDOPTS += -L/opt/xmingw/lib

# Or:
#HOST=/usr/local/mingw32/bin/i586-mingw32msvc-
#LDOPTS = -L/usr/local/mingw32/lib


# Building for native:
HOST=


# Which compiler to use
CXX=$(HOST)g++
CC=$(HOST)gcc
CPP=$(HOST)gcc

nesmock: main.o md5.o
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LDFLAGS) $(LDLIBS)

thisclean: FORCE
	rm -f *.o nesmock
clean: thisclean
	#for s in $(SUBDIRS);do make -C $$s clean;done
distclean: thisclean
	rm -f *~
realclean: distclean


include depfun.mak


winzip: nesmock README.html COPYING.txt
	@rm -rf $(ARCHNAME)
	- mkdir $(ARCHNAME)
	for s in $^;do ln "$$s" $(ARCHNAME)/"$$s"; done
	for dir in .; do (\
	 cd $(ARCHNAME)/$$dir; \
	 /bin/ls|while read s;do echo "$$s"|grep -qF . || test -d "$$s" || mv -v "$$s" "$$s".exe;done; \
	                           ); done
	$(HOST)strip $(ARCHNAME)/*.exe
	- upx --overlay=strip -9 $(ARCHNAME)/*.exe
	zip -r9 $(ARCHNAME)-win32.zip $(ARCHNAME)
	rm -rf $(ARCHNAME)
	mv -f $(ARCHNAME)-win32.zip archives/
	- ln -f archives/$(ARCHNAME)-win32.zip /WWW/src/arch/

.PHONY: all install uninstall clean distclean realclean FORCE
FORCE: ;
