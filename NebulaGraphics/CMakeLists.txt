cmake_minimum_required(VERSION 3.27)
project(NebulaGraphics)

# This is a layer of abstraction for the graphics API, which consist of:
# Core - Shared code between all graphics APIs
# Renderer - Dynamically loads DLls for the graphics API
# OpenGL - OpenGL implementation
# Metal - Metal implementation
# DirectX - DirectX implementation

option(NEBULA_GRAPHICS_OPENGL "Enable OpenGL implementation" ON)
if (APPLE)
    option(NEBULA_GRAPHICS_METAL "Enable Metal implementation" ON)
else()
    option(NEBULA_GRAPHICS_METAL "Enable Metal implementation" OFF)
endif()
option(NEBULA_GRAPHICS_DIRECTX "Enable DirectX implementation" OFF)
option(NEBULA_GRAPHICS_VULKAN "Enable Vulkan implementation" OFF)

add_subdirectory(Core)
add_subdirectory(Renderer)
add_subdirectory(Fonts)

set(RENDERER_INCLUDED FALSE)
set(NEBULA_GRAPHICS_FORMAT_DEPENDENCIES "")

if (NEBULA_GRAPHICS_OPENGL)
    message(STATUS "Nebula: Including OpenGL renderer")
    set(RENDERER_INCLUDED TRUE)
    add_subdirectory(OpenGL)
    LIST(APPEND NEBULA_GRAPHICS_FORMAT_DEPENDENCIES "NebulaGraphicsOpenGL_format")
    if (NOT TARGET glfw)
        add_subdirectory(vendor/glfw)
    endif()
endif()
if (NEBULA_GRAPHICS_METAL)
    message(STATUS "Nebula: Including Metal renderer")
    set(RENDERER_INCLUDED TRUE)
    add_subdirectory(Metal)
    LIST(APPEND NEBULA_GRAPHICS_FORMAT_DEPENDENCIES "NebulaGraphicsMetal_format")
    if (NOT TARGET glfw)
        add_subdirectory(vendor/glfw)
    endif()
endif()
if (NEBULA_GRAPHICS_DIRECTX)
    message(STATUS "Nebula: Including DirectX renderer")
    set(RENDERER_INCLUDED TRUE)
    #add_subdirectory(DirectX)
    #LIST(APPEND NEBULA_GRAPHICS_FORMAT_DEPENDENCIES "NebulaGraphicsDirectX_format")
endif()
if (NEBULA_GRAPHICS_VULKAN)
    message(STATUS "Nebula: Including Vulkan renderer")
    set(RENDERER_INCLUDED TRUE)
    #add_subdirectory(Vulkan)
    #LIST(APPEND NEBULA_GRAPHICS_FORMAT_DEPENDENCIES "NebulaGraphicsVulkan_format")
endif()

if (NOT RENDERER_INCLUDED)
    message(FATAL_ERROR "No graphics API implementations enabled")
endif()

if (NEBULA_BUILD_TESTS)
    add_subdirectory(Tests)
endif()

add_library(NebulaGraphics INTERFACE)
target_link_libraries(NebulaGraphics INTERFACE
    NebulaGraphicsCore
    NebulaGraphicsRenderer
)

if (NEBULA_ENABLE_CLANG_FORMAT)
    add_custom_target(NebulaGraphics_format)
    add_dependencies(NebulaGraphics_format
        NebulaGraphicsCore_format
        NebulaGraphicsRenderer_format
        ${NEBULA_GRAPHICS_FORMAT_DEPENDENCIES}
    )
endif()
