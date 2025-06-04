cmake_minimum_required(VERSION 3.27)
project(imgui)

add_library(imgui)
target_sources(imgui PRIVATE source/backends/imgui_impl_win32.cpp source/imgui.cpp source/imgui_demo.cpp source/imgui_draw.cpp source/backends/imgui_impl_opengl3.cpp source/imgui_tables.cpp source/imgui_widgets.cpp)

target_include_directories(imgui PUBLIC source)