@echo off
REM This builds all the libraries of the folder for 1 uname

if "%1"=="VC" goto gdiplus_VC
if "%1"=="vc-all" goto all-vc

call tecmake %1 "MF=cd_freetype" %2 %3 %4 %5 %6 %7 %8
call tecmake %1 %2 %3 %4 %5 %6 %7 %8
call tecmake %1 "MF=cd_pdflib" %2 %3 %4 %5 %6 %7 %8
call tecmake %1 "MF=cdpdf" %2 %3 %4 %5 %6 %7 %8
REM call tecmake %1 "MF=cdlua3" %2 %3 %4 %5 %6 %7 %8
REM call tecmake %1 "MF=cdluapdf3" %2 %3 %4 %5 %6 %7 %8
call tecmake %1 "MF=cdlua5" %2 %3 %4 %5 %6 %7 %8
call tecmake %1 "MF=cdluapdf5" %2 %3 %4 %5 %6 %7 %8
call tecmake %1 "MF=cdluaim5" %2 %3 %4 %5 %6 %7 %8

if "%1"=="vc6" goto gdiplus
if "%1"=="vc7" goto gdiplus
if "%1"=="vc8" goto gdiplus
if "%1"=="vc8_64" goto gdiplus
if "%1"=="vc9" goto gdiplus
if "%1"=="vc9_64" goto gdiplus
if "%1"=="dll" goto gdiplus
if "%1"=="dll7" goto gdiplus
if "%1"=="dll8" goto gdiplus
if "%1"=="dll8_64" goto gdiplus
if "%1"=="dll9" goto gdiplus
if "%1"=="dll9_64" goto gdiplus
if "%1"=="all" goto all-vc
goto end

:gdiplus
call tecmake %1 "MF=cdcontextplus" %2 %3 %4 %5 %6
call tecmake %1 "MF=cdluacontextplus5" %2 %3 %4 %5 %6 %7 %8
goto end

:gdiplus_VC
call tecmake %2 "MF=cdcontextplus" %3 %4 %5 %6
call tecmake %2 "MF=cdluacontextplus5" %3 %4 %5 %6 %7 %8
goto end

:all-vc
call make_uname VC vc6 %2 %3 %4 %5 %6
call make_uname VC vc7 %2 %3 %4 %5 %6
call make_uname VC vc8 %2 %3 %4 %5 %6
call make_uname VC vc8_64 %2 %3 %4 %5 %6
call make_uname VC vc9 %2 %3 %4 %5 %6
call make_uname VC vc9_64 %2 %3 %4 %5 %6
call make_uname VC dll %2 %3 %4 %5 %6
call make_uname VC dll7 %2 %3 %4 %5 %6
call make_uname VC dll8 %2 %3 %4 %5 %6
call make_uname VC dll8_64 %2 %3 %4 %5 %6
call make_uname VC dll9 %2 %3 %4 %5 %6
call make_uname VC dll9_64 %2 %3 %4 %5 %6
goto end

:end
