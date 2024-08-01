cmake_minimum_required(VERSION 3.10)

add_library(geo STATIC
    edge.cpp
    face.cpp
    modelObject.cpp
    object3D.cpp
    vertex.cpp
)

target_include_directories(geo PRIVATE ${PROJECT_SOURCE_DIR}/src)
target_include_directories(geo PRIVATE ${PROJECT_SOURCE_DIR}/include)