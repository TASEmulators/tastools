del ..\output\TAS-Movie-Editor.zip

copy /y .\CHANGELOG.txt .\Bin\Release\CHANGELOG.txt

cd .\bin
..\..\ReleaseUtilities\zip -X -9 -r ..\..\output\TAS-Movie-Editor .\Release\tas-movie-editor.exe
..\..\ReleaseUtilities\zip -X -9 -r ..\..\output\TAS-Movie-Editor .\Release\CHANGELOG.txt
cd..
