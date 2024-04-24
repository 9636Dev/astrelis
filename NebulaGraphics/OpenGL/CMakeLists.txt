cmake_minimum_required(VERSION 3.27)
project(NebulaGraphicsOpenGL)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(NEBULA_GRAPHICS_OPENGL_SOURCES
    src/NebulaGraphicsOpenGL/Core.hpp
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
)

if(POLICY CMP0072)
  cmake_policy(SET CMP0072 NEW)
endif()
set(OpenGL_GL_PREFERENCE "GLVND")
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
add_subdirectory(vendor/glfw)
add_subdirectory(vendor/glm)

add_library(NebulaGraphicsOpenGL SHARED ${NEBULA_GRAPHICS_OPENGL_SOURCES})
add_dependencies(NebulaGraphicsOpenGL NebulaCore)
nebula_add_definitions(NebulaGraphicsOpenGL "NEBULA_GRAPHICS_OPENGL" SHARED)

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
    NebulaGraphicsCore
)
target_link_libraries(NebulaGraphicsOpenGL
    NebulaCore
    NebulaGraphicsCore
    glfw
    GLEW::GLEW
    glm::glm
)

# Post build command to copy the shared library to the output directory
add_custom_command(TARGET NebulaGraphicsOpenGL POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:NebulaGraphicsOpenGL> ${NEBULA_RUNTIME_DIR}/lib/NebulaGraphicsOpenGL${CMAKE_SHARED_LIBRARY_SUFFIX}
)

if (NOT DEFINED NEBULA_DISABLE_TESTS)
    add_subdirectory(tests)
endif()
