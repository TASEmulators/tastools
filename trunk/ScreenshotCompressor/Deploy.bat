..\ReleaseUtilities\upx.exe .\output\ScreenshotCompress.exe
..\ReleaseUtilities\zip.exe -X -9 -r ScreenshotCompress.zip .\output\pngbat.bat .\output\ScreenshotCompress.exe .\output\ScreenshotCompress-Documentation.txt
move ScreenshotCompress.zip ..\output\