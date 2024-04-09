cmake_minimum_required(VERSION 3.27)
project(NebulaGraphicsMetal)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(NEBULA_GRAPHICS_METAL_SOURCES
    src/NebulaGraphicsMetal/Core.hpp
    src/NebulaGraphicsMetal/MTLImplementation.cpp
    src/NebulaGraphicsMetal/Renderer.hpp
    src/NebulaGraphicsMetal/Renderer.mm
    src/NebulaGraphicsMetal/Window.hpp
    src/NebulaGraphicsMetal/Window.mm
)

if (NOT TARGET glfw)
    add_subdirectory(vendor/glfw)
endif()

add_library(NebulaGraphicsMetal SHARED ${NEBULA_GRAPHICS_METAL_SOURCES})
nebula_add_definitions(NebulaGraphicsMetal "NEBULA_GRAPHICS_METAL" SHARED)

target_include_directories(NebulaGraphicsMetal PUBLIC src)
target_include_directories(NebulaGraphicsMetal PRIVATE
    vendor/metal
)

add_dependencies(NebulaGraphicsMetal
    NebulaCore
    NebulaGraphicsCore
    glfw
)

target_link_libraries(NebulaGraphicsMetal PUBLIC
    "-framework Metal"
    "-framework QuartzCore"
    "-framework Foundation"
    NebulaCore
    NebulaGraphicsCore
    glfw
)

# Post build command to copy the shared library to the output directory
add_custom_command(TARGET NebulaGraphicsMetal POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:NebulaGraphicsMetal> ${NEBULA_RUNTIME_DIR}/lib/NebulaGraphicsMetal${CMAKE_SHARED_LIBRARY_SUFFIX}
)
