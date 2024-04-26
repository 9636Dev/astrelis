cmake_minimum_required(VERSION 3.27)
project(NebulaShaderCLI)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(NEBULA_SHADER_CLI_SOURCES
    src/NebulaShaderCLI/Application.cpp
    src/NebulaShaderCLI/Arguments.hpp
    src/NebulaShaderCLI/Arguments.cpp
)

add_executable(NebulaShaderCLI ${NEBULA_SHADER_CLI_SOURCES})

target_include_directories(NebulaShaderCLI PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

add_dependencies(NebulaShaderCLI
    NebulaCore
    NebulaShaderCompiler
    NebulaShaderConductor
)
target_link_libraries(NebulaShaderCLI
    NebulaCore
    NebulaShaderCompiler
    NebulaShaderConductor
)
