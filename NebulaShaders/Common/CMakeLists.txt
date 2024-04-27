cmake_minimum_required(VERSION 3.27)
project(NebulaShaderCommon)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(NEBULA_SHADER_COMMON_SOURCES
    src/NebulaShaderCommon/ShaderMeta.hpp
    src/NebulaShaderCommon/ShaderStage.hpp
    src/NebulaShaderCommon/Bindings.hpp
    src/NebulaShaderCommon/Meta.hpp
    src/NebulaShaderCommon/Inputs.hpp
    src/NebulaShaderCommon/ShaderSource.hpp
)

add_library(NebulaShaderCommon INTERFACE ${NEBULA_SHADER_COMMON_SOURCES})
target_include_directories(NebulaShaderCommon INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_dependencies(NebulaShaderCommon cereal)
target_link_libraries(NebulaShaderCommon INTERFACE cereal)
