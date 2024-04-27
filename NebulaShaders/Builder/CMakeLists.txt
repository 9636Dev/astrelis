cmake_minimum_required(VERSION 3.27)
project(NebulaShaderBuilder)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(NEBULA_SHADER_BUILDER_SOURCES
    src/NebulaShaderBuilder/FileFormat.hpp
    src/NebulaShaderBuilder/FileFormat.cpp
)

set(NEBULA_SHADER_BUILDER_LIBRARY_TYPE "Inherit" CACHE STRING "The type of library to build")
set_property(CACHE NEBULA_SHADER_BUILDER_LIBRARY_TYPE PROPERTY STRINGS "Inherit" "Static" "Shared")
nebula_library_type(NEBULA_SHADER_BUILDER_LIBRARY_TYPE NEBULA_SHADER_BUILDER_LIBRARY_TYPE_FINAL)
message(STATUS "NebulaEngine: NebulaShaderBuilder built as: ${NEBULA_SHADER_BUILDER_LIBRARY_TYPE_FINAL}")
add_library(NebulaShaderBuilder ${NEBULA_SHADER_BUILDER_LIBRARY_TYPE_FINAL} ${NEBULA_SHADER_BUILDER_SOURCES})
nebula_add_definitions(NebulaShaderBuilder "NEBULA_SHADER_BUILDER" ${NEBULA_SHADER_BUILDER_LIBRARY_TYPE_FINAL})

target_include_directories(NebulaShaderBuilder PUBLIC src)

add_dependencies(NebulaShaderBuilder
    NebulaCore
    NebulaIO
    NebulaShaderCommon
    NebulaShaderCompiler
    NebulaShaderConductor
)

target_link_libraries(NebulaShaderBuilder PUBLIC
    NebulaCore
    NebulaIO
    NebulaShaderCommon
    NebulaShaderCompiler
    NebulaShaderConductor
)

if (NEBULA_BUILD_TESTS)
    add_subdirectory(tests)
endif()
