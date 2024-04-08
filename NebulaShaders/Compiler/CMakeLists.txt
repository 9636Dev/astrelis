cmake_minimum_required(VERSION 3.27)

project(NebulaShaderCompiler)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(NEBULA_SHADER_COMPILER_SOURCE
    src/NebulaShaderCompiler/Core.hpp
    src/NebulaShaderCompiler/Lexer.hpp
    src/NebulaShaderCompiler/Lexer.cpp
    src/NebulaShaderCompiler/Parser.hpp
    src/NebulaShaderCompiler/Parser.cpp
)

add_library(NebulaShaderCompiler ${NEBULA_LIBRARY_TYPE} ${NEBULA_SHADER_COMPILER_SOURCE})
nebula_add_definitions(NebulaShaderCompiler "NEBULA_SHADER_COMPILER" SHARED)
target_include_directories(NebulaShaderCompiler PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

add_dependencies(NebulaShaderCompiler NebulaCore)
target_link_libraries(NebulaShaderCompiler NebulaCore)
