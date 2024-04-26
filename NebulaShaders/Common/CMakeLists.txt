cmake_minimum_required(VERSION 3.27)
project(NebulaShaderCommon)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(NEBULA_SHADER_COMMON_SOURCES
    src/ShaderCommon/ShaderMeta.hpp
    src/ShaderCommon/Bindings.hpp
    src/ShaderCommon/Inputs.hpp
)

add_library(NebulaShaderCommon INTERFACE ${NEBULA_SHADER_COMMON_SOURCES})
target_include_directories(NebulaShaderCommon INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/src)
