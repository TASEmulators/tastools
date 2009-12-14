del /q /s includes-copy
mkdir includes-copy
xcopy /e include\* includes-copy
xcopy /y /e lib\vc_lib\msw\* includes-copy