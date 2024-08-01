cmake_minimum_required(VERSION 3.10)

add_library(input STATIC
    callbacks.cpp
    inputManager.cpp
)

target_include_directories(input PRIVATE ${PROJECT_SOURCE_DIR}/include)