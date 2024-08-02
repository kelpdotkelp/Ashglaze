cmake_minimum_required(VERSION 3.10)

add_library(geo STATIC
    edge.cpp
    face.cpp
    GLData/GLData.cpp
    GLData/GLDataEdge.cpp
    GLData/GLDataFace.cpp
    GLData/GLDataVertex.cpp
    modelObject.cpp
    object3D.cpp
    vertex.cpp
)

target_include_directories(geo PRIVATE ${PROJECT_SOURCE_DIR}/src)
target_include_directories(geo PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_include_directories(geo PRIVATE ${PROJECT_SOURCE_DIR}/src/GLData)