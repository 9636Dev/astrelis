cmake_minimum_required(VERSION 3.27)
project(NebulaGraphics)

# This is a layer of abstraction for the graphics API, which consist of:
# Core - Shared code between all graphics APIs
# Renderer - Dynamically loads DLls for the graphics API
# OpenGL - OpenGL implementation
# Metal - Metal implementation
# DirectX - DirectX implementation

add_subdirectory(Core)
add_subdirectory(Renderer)
add_subdirectory(OpenGL)
if (APPLE)
    add_subdirectory(Metal)
endif()

# TODO: Add DirectX implementations

add_library(NebulaGraphics INTERFACE)
target_link_libraries(NebulaGraphics INTERFACE NebulaGraphicsRenderer)
