cmake_minimum_required(VERSION 3.31)
project(AssaultCube)

set(CMAKE_CXX_STANDARD 20)

if (NOT MSVC)
    message(FATAL_ERROR "Unsupported compiler used '${CMAKE_CXX_COMPILER_ID}', AssaultCube only supports MSVC.")
endif ()

add_subdirectory(vendor/minhook)
add_subdirectory(vendor/imgui)

file(GLOB_RECURSE SOURCES "src/*.h" "src/*.cpp")
add_library(AssaultCube SHARED ${SOURCES})

add_compile_definitions(AssaultCube "_CRT_SECURE_NO_WARNINGS" "NOMINMAX" "WIN32_LEAN_AND_MEAN")

target_link_libraries(AssaultCube PRIVATE minhook imgui)