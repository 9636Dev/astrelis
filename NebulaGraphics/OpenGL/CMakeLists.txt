cmake_minimum_required(VERSION 3.27)
project(NebulaGraphicsOpenGL)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(NEBULA_GRAPHICS_OPENGL_SOURCES
    src/NebulaGraphicsOpenGL/Core.hpp
    src/NebulaGraphicsOpenGL/Window.hpp
    src/NebulaGraphicsOpenGL/Window.cpp
)

add_subdirectory(vendor/glfw)

add_library(NebulaGraphicsOpenGL SHARED ${NEBULA_GRAPHICS_OPENGL_SOURCES})
add_dependencies(NebulaGraphicsOpenGL NebulaCore)
nebula_add_definitions(NebulaGraphicsOpenGL "NEBULA_GRAPHICS_OPENGL" SHARED)

target_include_directories(NebulaGraphicsOpenGL PUBLIC src)

add_dependencies(NebulaGraphicsOpenGL
    glfw
    NebulaGraphicsCore
)
target_link_libraries(NebulaGraphicsOpenGL 
    NebulaCore
    NebulaGraphicsCore
    glfw
)

# Post build command to copy the shared library to the output directory
add_custom_command(TARGET NebulaGraphicsOpenGL POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:NebulaGraphicsOpenGL> ${NEBULA_RUNTIME_DIR}/lib/NebulaGraphicsOpenGL${CMAKE_SHARED_LIBRARY_SUFFIX}
)
