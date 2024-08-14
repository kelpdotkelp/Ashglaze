cmake_minimum_required(VERSION 3.10)

add_library(world STATIC
    coordinateAxis.cpp
)

target_include_directories(world PRIVATE ${PROJECT_SOURCE_DIR}/include)