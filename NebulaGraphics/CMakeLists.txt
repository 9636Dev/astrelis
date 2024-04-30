cmake_minimum_required(VERSION 3.27)
project(NebulaGraphics)

# This is a layer of abstraction for the graphics API, which consist of:
# Core - Shared code between all graphics APIs
# Renderer - Dynamically loads DLls for the graphics API
# OpenGL - OpenGL implementation
# Metal - Metal implementation
# DirectX - DirectX implementation

option(NEBULA_GRAPHICS_OPENGL "Enable OpenGL implementation" ON)
option(NEBULA_GRAPHICS_METAL "Enable Metal implementation" $<BOOL:${APPLE}>)
option(NEBULA_GRAPHICS_DIRECTX "Enable DirectX implementation" OFF)
option(NEBULA_GRAPHICS_VULKAN "Enable Vulkan implementation" OFF)

add_subdirectory(Core)
add_subdirectory(Renderer)

set(RENDERER_INCLUDED FALSE)
if (NEBULA_GRAPHICS_OPENGL)
    set(RENDERER_INCLUDED TRUE)
    add_subdirectory(OpenGL)
endif()
if (NEBULA_GRAPHICS_METAL)
    set(RENDERER_INCLUDED TRUE)
    #add_subdirectory(Metal)
endif()
if (NEBULA_GRAPHICS_DIRECTX)
    set(RENDERER_INCLUDED TRUE)
    #add_subdirectory(DirectX)
endif()
if (NEBULA_GRAPHICS_VULKAN)
    set(RENDERER_INCLUDED TRUE)
    #add_subdirectory(Vulkan)
endif()

if (NOT RENDERER_INCLUDED)
    message(FATAL_ERROR "No graphics API implementations enabled")
endif()

if (NEBULA_BUILD_TESTS)
    add_subdirectory(Tests)
endif()

add_library(NebulaGraphics INTERFACE)
target_link_libraries(NebulaGraphics INTERFACE NebulaGraphicsRenderer)
