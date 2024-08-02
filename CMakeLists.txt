cmake_minimum_required(VERSION 3.10)

project(Stoneware3D VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(REMOVE ./build/src/Debug/Stoneware3D.exe)

add_subdirectory(glfw-3.4)

add_subdirectory(src)

file(COPY shaders DESTINATION .)