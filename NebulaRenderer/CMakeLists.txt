set(NEBULA_RENDERER_SOURCES
    src/NebulaRenderer/Renderer.hpp
)

set(NEBULA_RENDERER "OpenGL")
set(NEBULA_RENDERER_PLATFORM_SOURCES
    $<$<STREQUAL:${NEBULA_RENDERER},OpenGL>:
        src/NebulaRenderer/OpenGL/OpenGLRenderer.hpp
        src/NebulaRenderer/OpenGL/OpenGLRenderer.cpp
    >
)

if (NEBULA_PLATFORM STREQUAL "LINUX")
    set(OpenGL_GL_PREFERENCE "GLVND")
endif()

if (NEBULA_RENDERER STREQUAL "OpenGL")
    if (NEBULA_BUILD_GLEW)
        message(FATAL_ERROR "Building GLEW is not supported yet!")
    else()
        find_package(GLEW QUIET)
        if (NOT GLEW_FOUND)
            message(FATAL_ERROR "GLEW not found, please build it from source using -DNEBULA_BUILD_GLEW=ON")
        endif()

        FIND_PACKAGE(OpenGL REQUIRED)
    endif()

    add_library(NebulaRenderer STATIC
    ${NEBULA_RENDERER_SOURCES}
    ${NEBULA_RENDERER_PLATFORM_SOURCES}
)
    nebula_target(NebulaRenderer)
    nebula_format_project(NebulaRenderer ${CMAKE_CURRENT_SOURCE_DIR} "${NEBULA_RENDERER_SOURCES};${NEBULA_RENDERER_PLATFORM_SOURCES}")
endif()

target_include_directories(NebulaRenderer PUBLIC
    src
)

add_dependencies(NebulaRenderer
    NebulaCore
    NebulaWindowing
)

target_link_libraries(NebulaRenderer
    NebulaCore
    NebulaWindowing
    $<$<STREQUAL:${NEBULA_RENDERER},OpenGL>:GLEW::GLEW>
)

