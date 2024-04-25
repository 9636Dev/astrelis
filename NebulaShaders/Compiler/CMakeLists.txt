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

add_library(NebulaShaderCompiler SHARED ${NEBULA_SHADER_COMPILER_SOURCES})
nebula_add_definitions(NebulaShaderCompiler "NEBULA_SHADER_COMPILER" SHARED)
target_include_directories(NebulaShaderCompiler PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

add_dependencies(NebulaShaderCompiler
    NebulaCore
    NebulaShaderConductor
)
target_link_libraries(NebulaShaderCompiler PUBLIC
    NebulaCore
    NebulaShaderConductor
)

if (NOT DEFINED NEBULA_DISABLE_TESTS)
    add_subdirectory(tests)
endif()
