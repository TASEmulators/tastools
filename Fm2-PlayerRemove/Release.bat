..\ReleaseUtilities\upx.exe .\Release\Fm2-PlayerRemove.exe
..\ReleaseUtilities\zip.exe -X -9 -r Fm2-PlayerRemove.zip .\Release\Fm2-PlayerRemove.exe .\Release\Readme.txt
move Fm2-PlayerRemove.zip ..\output\
