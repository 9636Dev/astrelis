cmake_minimum_required(VERSION 3.27)

project(NebulaShaderCompiler)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(NEBULA_SHADER_COMPILER_SOURCES
    src/NebulaShaderCompiler/Core.hpp
    src/NebulaShaderCompiler/Token.hpp
    src/NebulaShaderCompiler/Token.cpp
    src/NebulaShaderCompiler/Lexer.hpp
    src/NebulaShaderCompiler/Lexer.cpp
    src/NebulaShaderCompiler/ShaderCompiler.hpp
    src/NebulaShaderCompiler/ShaderCompiler.cpp
)

set(NEBULA_SHADER_COMPILER_LIBRARY_TYPE "Inherit" CACHE STRING "The type of library to build")
set_property(CACHE NEBULA_SHADER_COMPILER_LIBRARY_TYPE PROPERTY STRINGS "Inherit" "Static" "Shared")
nebula_library_type(NEBULA_SHADER_COMPILER_LIBRARY_TYPE NEBULA_SHADER_COMPILER_LIBRARY_TYPE_FINAL)
message(STATUS "NebulaEngine: NebukaShaderCompiler built as: ${NEBULA_SHADER_COMPILER_LIBRARY_TYPE_FINAL}")
add_library(NebulaShaderCompiler ${NEBULA_SHADER_COMPILER_LIBRARY_TYPE_FINAL} ${NEBULA_SHADER_COMPILER_SOURCES})
nebula_add_definitions(NebulaShaderCompiler "NEBULA_SHADER_COMPILER" ${NEBULA_SHADER_COMPILER_LIBRARY_TYPE_FINAL})
nebula_format_project(NebulaShaderCompiler ${CMAKE_CURRENT_SOURCE_DIR} "${NEBULA_SHADER_COMPILER_SOURCES}")

target_include_directories(NebulaShaderCompiler PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

add_dependencies(NebulaShaderCompiler
    NebulaCore
    NebulaShaderCommon
)
target_link_libraries(NebulaShaderCompiler PUBLIC
    NebulaCore
    NebulaShaderCommon
)

if (NEBULA_BUILD_TESTS)
    add_subdirectory(tests)
endif()
