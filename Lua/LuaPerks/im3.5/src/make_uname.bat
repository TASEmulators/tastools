@echo off
REM This builds all the libraries of the folder for 1 uname  

if "%1"=="VCC" goto do-vcc
if "%1"=="vc-all" goto start-all-vc

call tecmake %1 %2 %3 %4 %5 %6 %7 %8
call tecmake %1 "MF=im_process" %2 %3 %4 %5 %6 %7 %8
call tecmake %1 "MF=im_jp2" %2 %3 %4 %5 %6 %7 %8
call tecmake %1 "MF=im_avi" %2 %3 %4 %5 %6 %7 %8
call tecmake %1 "MF=im_fftw" %2 %3 %4 %5 %6 %7 %8

REM call tecmake %1 "MF=imlua3" %2 %3 %4 %5 %6 %7 %8

call tecmake %1 "MF=imlua5" %2 %3 %4 %5 %6 %7 %8
call tecmake %1 "MF=imlua_process5" %2 %3 %4 %5 %6 %7 %8
call tecmake %1 "MF=imlua_jp2" %2 %3 %4 %5 %6 %7 %8
call tecmake %1 "MF=imlua_avi" %2 %3 %4 %5 %6 %7 %8
call tecmake %1 "MF=imlua_fftw5" %2 %3 %4 %5 %6 %7 %8

if "%1"=="vc6"  goto vc
if "%1"=="vc7"  goto vc
if "%1"=="vc8"  goto vc
if "%1"=="vc8_64"  goto vc
if "%1"=="vc9"  goto vc
if "%1"=="vc9_64"  goto vc
if "%1"=="dll"  goto vc
if "%1"=="dll7"  goto vc
if "%1"=="dll8"  goto vc
if "%1"=="dll8_64"  goto vc
if "%1"=="dll9"  goto vc
if "%1"=="dll9_64"  goto vc
if "%1"=="all"  goto start-all-vc
goto end

:vc
call tecmake %1 "MF=im_wmv" %2 %3 %4 %5 %6 %7 %8
call tecmake %1 "MF=imlua_wmv" %2 %3 %4 %5 %6 %7 %8
call tecmake %1 "MF=im_capture" %2 %3 %4 %5 %6 %7 %8
call tecmake %1 "MF=imlua_capture5" %2 %3 %4 %5 %6 %7 %8
if "%1"=="dll"  goto dll
goto end

:dll
call tecmake mingw4 "MF=im_capture" mingw4-dll
call tecmake mingw3 "MF=im_capture" mingw3-dll
call tecmake bc56 "MF=im_capture" bc56-dll
REM call tecmake owc1 "MF=im_capture" owc1-dll
goto end

:start-all-vc
call make_uname VCC vc6 %2 %3 %4 %5 %6
call make_uname VCC vc7 %2 %3 %4 %5 %6
call make_uname VCC vc8 %2 %3 %4 %5 %6
call make_uname VCC vc8_64 %2 %3 %4 %5 %6
call make_uname VCC vc9 %2 %3 %4 %5 %6
call make_uname VCC vc9_64 %2 %3 %4 %5 %6
call make_uname VCC dll %2 %3 %4 %5 %6
call make_uname VCC dll7 %2 %3 %4 %5 %6
call make_uname VCC dll8 %2 %3 %4 %5 %6
call make_uname VCC dll8_64 %2 %3 %4 %5 %6
call make_uname VCC dll9 %2 %3 %4 %5 %6
call make_uname VCC dll9_64 %2 %3 %4 %5 %6
goto end

:do-vcc
call tecmake %2 "MF=im_wmv" %3 %4 %5 %6 %7 %8
call tecmake %2 "MF=imlua_wmv" %3 %4 %5 %6 %7 %8
call tecmake %2 "MF=im_capture" %3 %4 %5 %6 %7 %8
call tecmake %2 "MF=imlua_capture5" %3 %4 %5 %6 %7 %8
if "%2"=="dll"  goto dll
goto end

:end
