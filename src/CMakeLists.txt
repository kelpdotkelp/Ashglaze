cmake_minimum_required(VERSION 3.10)

add_subdirectory(input)
add_subdirectory(num)
add_subdirectory(geo)

add_executable(Stoneware3D
    camera.cpp
    framebufferGeoSelect.cpp
    main.cpp
    shader.cpp
    shaderProgram.cpp
    window.cpp

    ../glad.c
)

target_include_directories(Stoneware3D PRIVATE ${PROJECT_SOURCE_DIR}/include)

#target_link_libraries(Stoneware3D PUBLIC ${PROJECT_SOURCE_DIR}/libs/libglfw3.a)
target_link_libraries(Stoneware3D PUBLIC glfw)
target_link_libraries(Stoneware3D PUBLIC num)
target_link_libraries(Stoneware3D PUBLIC geo)
target_link_libraries(Stoneware3D PUBLIC input)