@echo off
copy "D:\Visual Studio Projects\SQLite.NET\"SQLiteClient\bin\debug\SQLiteClient.dll "D:\Visual Studio Projects\SQLite.NET\SQLiteTest\bin\Release\"
copy "D:\Visual Studio Projects\SQLite.NET\"SQLiteClient\bin\debug\SQLiteClient.pdb "D:\Visual Studio Projects\SQLite.NET\SQLiteTest\bin\Release\"
if errorlevel 1 goto CSharpReportError
goto CSharpEnd
:CSharpReportError
echo Project error: A tool returned an error code from the build event
exit 1
:CSharpEnd