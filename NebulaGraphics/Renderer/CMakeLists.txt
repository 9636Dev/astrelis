cmake_minimum_required(VERSION 3.27)
project(NebulaGraphicsRenderer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(NEBULA_GRAPHICS_RENDERER_SOURCES
    src/NebulaGraphicsRenderer/Core.hpp
    src/NebulaGraphicsRenderer/Loader.hpp
    src/NebulaGraphicsRenderer/Loader.cpp
    src/NebulaGraphicsRenderer/Window.hpp
    src/NebulaGraphicsRenderer/Window.cpp
    src/NebulaGraphicsRenderer/Renderer.hpp
    src/NebulaGraphicsRenderer/Renderer.cpp
)

add_library(NebulaGraphicsRenderer SHARED ${NEBULA_GRAPHICS_RENDERER_SOURCES})
add_dependencies(NebulaGraphicsRenderer NebulaCore)
nebula_add_definitions(NebulaGraphicsRenderer "NEBULA_GRAPHICS_RENDERER" SHARED)
nebula_format_project(NebulaGraphicsRenderer ${CMAKE_CURRENT_SOURCE_DIR} "${NEBULA_GRAPHICS_RENDERER_SOURCES}")
target_compile_options(NebulaGraphicsRenderer PRIVATE "${NEBULA_CXX_FLAGS}")

target_include_directories(NebulaGraphicsRenderer PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

add_dependencies(NebulaGraphicsRenderer NebulaGraphicsCore)
target_link_libraries(NebulaGraphicsRenderer
    NebulaCore
    NebulaGraphicsCore
)
