cmake_minimum_required(VERSION 3.27)
project(NebulaGraphicsOpenGLTests)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(NEBULA_GRAPHICS_OPENGL_TESTS_SOURCE
    WindowTest.cpp
    RenderTest.cpp
)

enable_testing()
add_executable(NebulaGraphicsOpenGLTests ${NEBULA_GRAPHICS_OPENGL_TESTS_SOURCE})

add_dependencies(NebulaGraphicsOpenGLTests NebulaGraphicsOpenGL)

target_link_libraries(NebulaGraphicsOpenGLTests
    NebulaGraphicsOpenGL
    gtest_main
)

