@echo off
REM This builds all the libraries of the folder for 1 uname

call tecmake %1 %2 %3 %4 %5 %6
call tecmake %1 "USE_GTK=Yes" %2 %3 %4 %5 %6

if "%1"=="dll" goto stub_dll
if "%1"=="dll7" goto stub_dll7
if "%1"=="dll8" goto stub_dll8
if "%1"=="dll8_64" goto stub_dll8_64
if "%1"=="dll9" goto stub_dll9
if "%1"=="dll9_64" goto stub_dll9_64
if "%1"=="all" goto all_dll
goto fim

:stub_dll
call tecmake vc6 "MF=iupstub" %2 %3 %4 %5 %6 %7
move /y ..\lib\vc6\iupstub.lib ..\lib\dll
goto fim

:stub_dll7
call tecmake vc7 "MF=iupstub" %2 %3 %4 %5 %6 %7
move /y ..\lib\vc7\iupstub.lib ..\lib\dll7
goto fim

:stub_dll8
call tecmake vc8 "MF=iupstub" %2 %3 %4 %5 %6 %7
move /y ..\lib\vc8\iupstub.lib ..\lib\dll8
goto fim

:stub_dll8_64
call tecmake vc8_64 "MF=iupstub" %2 %3 %4 %5 %6 %7
move /y ..\lib\vc8_64\iupstub.lib ..\lib\dll8_64
goto fim

:stub_dll9
call tecmake vc9 "MF=iupstub" %2 %3 %4 %5 %6 %7
move /y ..\lib\vc9\iupstub.lib ..\lib\dll9
goto fim

:stub_dll9_64
call tecmake vc9_64 "MF=iupstub" %2 %3 %4 %5 %6 %7
move /y ..\lib\vc9_64\iupstub.lib ..\lib\dll9_64
goto fim

:all_dll
call make_uname dll %2 %3 %4 %5 %6
call make_uname dll7 %2 %3 %4 %5 %6
call make_uname dll8 %2 %3 %4 %5 %6
call make_uname dll8_64 %2 %3 %4 %5 %6
call make_uname dll9 %2 %3 %4 %5 %6
call make_uname dll9_64 %2 %3 %4 %5 %6
goto fim

:fim
