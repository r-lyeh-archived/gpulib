call "\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" amd64
cl /LD imgui.cpp imgui_demo.cpp imgui_draw.cpp cimgui.cpp fontAtlas.cpp drawList.cpp
xcopy /y imgui.dll ..
xcopy /y imgui.lib ..
pause