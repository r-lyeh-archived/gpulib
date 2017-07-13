#!/bin/bash
cd "$(dirname -- "$(readlink -fn -- "${0}")")"

clang++ -c imgui.cpp imgui_demo.cpp imgui_draw.cpp cimgui.cpp fontAtlas.cpp drawList.cpp -static -stdlib=libc++ ${@}
ar -cr ../libimgui.a cimgui.o drawList.o fontAtlas.o imgui.o imgui_demo.o imgui_draw.o
rm -f cimgui.o drawList.o fontAtlas.o imgui.o imgui_demo.o imgui_draw.o
