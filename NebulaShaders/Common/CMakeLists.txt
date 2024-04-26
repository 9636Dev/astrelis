cmake_minimum_required(VERSION 3.27)
project(NebulaShaderCommon)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(NEBULA_SHADER_COMMON_SOURCES
    src/ShaderCommon/ShaderMeta.hpp
    src/ShaderCommon/ShaderMeta.cpp
)

add_library(NebulaShaderCommon STATIC ${NEBULA_SHADER_COMMON_SOURCES})
target_include_directories(NebulaShaderCommon PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
