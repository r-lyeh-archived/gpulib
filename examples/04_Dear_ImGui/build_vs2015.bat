call "\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" amd64
cl main.c -I..\..\thirdparty\include /link /LIBPATH:..\..\thirdparty\lib\ SDL2.lib /LIBPATH:..\..\ imgui.lib
xcopy /y ..\..\thirdparty\lib\SDL2.dll .
xcopy /y ..\..\imgui.dll .
pause