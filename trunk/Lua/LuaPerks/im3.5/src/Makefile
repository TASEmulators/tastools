
.PHONY: do_all im im_jp2 im_process im_fftw imlua3 imlua5 imlua_jp2 imlua_process5 imlua_fftw5
#do_all: im im_jp2 im_process im_fftw imlua3 imlua5 imlua_jp2 imlua_process5 imlua_fftw5
do_all: im im_jp2 im_process im_fftw imlua5 imlua_jp2 imlua_process5 imlua_fftw5

im:
	$(MAKE) --no-print-directory -f tecmake_compact.mak
im_jp2:
	@$(MAKE) --no-print-directory -f tecmake_compact.mak MF=im_jp2
im_process:
	@$(MAKE) --no-print-directory -f tecmake_compact.mak MF=im_process
im_fftw:
	@$(MAKE) --no-print-directory -f tecmake_compact.mak MF=im_fftw
imlua3:
	@$(MAKE) --no-print-directory -f tecmake_compact.mak MF=imlua3
imlua5:
	@$(MAKE) --no-print-directory -f tecmake_compact.mak MF=imlua5
imlua_jp2:
	@$(MAKE) --no-print-directory -f tecmake_compact.mak MF=imlua_jp2
imlua_process5:
	@$(MAKE) --no-print-directory -f tecmake_compact.mak MF=imlua_process5
imlua_fftw5:
	@$(MAKE) --no-print-directory -f tecmake_compact.mak MF=imlua_fftw5
