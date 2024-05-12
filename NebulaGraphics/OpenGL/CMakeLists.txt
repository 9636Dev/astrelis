cmake_minimum_required(VERSION 3.27)
project(NebulaGraphicsOpenGL)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(NEBULA_GRAPHICS_OPENGL_SOURCES
    src/NebulaGraphicsOpenGL/Core.hpp
    src/NebulaGraphicsOpenGL/AssetLoader.hpp
    src/NebulaGraphicsOpenGL/AssetLoader.cpp
    src/NebulaGraphicsOpenGL/Window.hpp
    src/NebulaGraphicsOpenGL/Window.cpp
    src/NebulaGraphicsOpenGL/Renderer.hpp
    src/NebulaGraphicsOpenGL/Renderer.cpp

    # OpenGL Wrappers
    src/NebulaGraphicsOpenGL/OpenGL/Enum.hpp
    src/NebulaGraphicsOpenGL/OpenGL/GL.hpp
    src/NebulaGraphicsOpenGL/OpenGL/GL.cpp
    src/NebulaGraphicsOpenGL/OpenGL/VertexArray.hpp
    src/NebulaGraphicsOpenGL/OpenGL/VertexArray.cpp
    src/NebulaGraphicsOpenGL/OpenGL/VertexBuffer.hpp
    src/NebulaGraphicsOpenGL/OpenGL/VertexBuffer.cpp
    src/NebulaGraphicsOpenGL/OpenGL/IndexBuffer.hpp
    src/NebulaGraphicsOpenGL/OpenGL/IndexBuffer.cpp
    src/NebulaGraphicsOpenGL/OpenGL/UniformBuffer.hpp
    src/NebulaGraphicsOpenGL/OpenGL/UniformBuffer.cpp
    src/NebulaGraphicsOpenGL/OpenGL/Shader.hpp
    src/NebulaGraphicsOpenGL/OpenGL/Shader.cpp
    src/NebulaGraphicsOpenGL/OpenGL/Texture.hpp
    src/NebulaGraphicsOpenGL/OpenGL/Texture.cpp
)

if(POLICY CMP0072)
  cmake_policy(SET CMP0072 NEW)
endif()

if (LINUX)
    set(OpenGL_GL_PREFERENCE "GLVND")
endif()

option(NEBULA_BUILD_GLEW "Build GLEW from source" OFF)

if (NEBULA_BUILD_GLEW)
    add_subdirectory(vendor/glew/build/cmake)
    # Alias glew target to GLEW::GLEW
    add_library(GLEW::GLEW ALIAS glew)
else()
    # We make it quiet so we can handle the error message ourselves
    find_package(GLEW QUIET)
    if (NOT GLEW_FOUND)
        message(FATAL_ERROR "GLEW not found. Please set NEBULA_BUILD_GLEW to ON to build GLEW from source.")
    endif()
    find_package(OpenGL REQUIRED)
endif()

add_subdirectory(vendor/glm)

add_library(NebulaGraphicsOpenGL SHARED ${NEBULA_GRAPHICS_OPENGL_SOURCES})
add_dependencies(NebulaGraphicsOpenGL NebulaCore)
nebula_add_definitions(NebulaGraphicsOpenGL "NEBULA_GRAPHICS_OPENGL" SHARED)
nebula_format_project(NebulaGraphicsOpenGL ${CMAKE_CURRENT_SOURCE_DIR} "${NEBULA_GRAPHICS_OPENGL_SOURCES}")
target_compile_options(NebulaGraphicsOpenGL PRIVATE "${NEBULA_CXX_FLAGS}")

target_include_directories(NebulaGraphicsOpenGL PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_options(NebulaGraphicsOpenGL PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/wd4190>
)

if (APPLE)
    target_compile_definitions(NebulaGraphicsOpenGL PRIVATE GL_SILENCE_DEPRECATION)
endif()

target_include_directories(NebulaGraphicsOpenGL PRIVATE vendor/glew/include)

add_dependencies(NebulaGraphicsOpenGL
    glfw
    glm::glm
    GLEW::GLEW
    NebulaGraphicsCore
    NebulaShaderBuilder
)
target_link_libraries(NebulaGraphicsOpenGL PUBLIC
    NebulaCore
    NebulaGraphicsCore
    NebulaShaderBuilder
)
target_link_libraries(NebulaGraphicsOpenGL PRIVATE
    glfw
    GLEW::GLEW
    glm::glm
)
ImGuiAddLibrary(NebulaGraphicsOpenGL "GLFWOpenGL3")

# Post build command to copy the shared library to the output directory
add_custom_command(TARGET NebulaGraphicsOpenGL POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:NebulaGraphicsOpenGL> ${NEBULA_RUNTIME_DIR}/lib/NebulaGraphicsOpenGL${CMAKE_SHARED_LIBRARY_SUFFIX}
)

if (NOT DEFINED NEBULA_DISABLE_TESTS)
    add_subdirectory(tests)
endif()
