cmake_minimum_required(VERSION 3.27)
project(NebulaShaderConductor)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(vendor/DirectXShaderCompiler/cmake/caches/PredefinedParams.cmake)
set(SPIRV_BUILD_TESTS OFF CACHE BOOL "Build SPIRV tests" FORCE)
set(LLVM_BUILD_TESTS OFF CACHE BOOL "Build LLVM tests" FORCE)
set(LLVM_INCLUDE_TESTS OFF CACHE BOOL "Include LLVM tests" FORCE)
set(HLSL_INCLUDE_TESTS OFF CACHE BOOL "Include HLSL tests" FORCE)
set(HLSL_COPY_GENERATED_SOURCES ON CACHE BOOL "Copy generated HLSL sources" FORCE)

add_subdirectory(vendor/DirectXShaderCompiler)

set(NEBULA_SHADER_CONDUCTOR_SOURCES
    src/NebulaShaderConductor/Core.hpp
    src/NebulaShaderConductor/ShaderInput.hpp
    src/NebulaShaderConductor/ShaderInput.cpp
    src/NebulaShaderConductor/ShaderOutput.hpp
    src/NebulaShaderConductor/Conductor.hpp
    src/NebulaShaderConductor/Conductor.cpp
    src/NebulaShaderConductor/TargetProfile.hpp
    src/NebulaShaderConductor/TargetProfile.cpp
)


add_library(NebulaShaderConductor SHARED ${NEBULA_SHADER_CONDUCTOR_SOURCES})
nebula_add_definitions(NebulaShaderConductor "NEBULA_SHADER_CONDUCTOR" SHARED)


target_compile_options(NebulaShaderConductor PRIVATE
    -fms-extensions
    -Wno-language-extension-token
)

target_include_directories(NebulaShaderConductor PRIVATE
    vendor/DirectXShaderCompiler/include
)

target_include_directories(NebulaShaderConductor PUBLIC
    src
)
add_dependencies(NebulaShaderConductor
    NebulaCore
    dxcompiler
    LLVMDxcSupport
    LLVMSupport
)
target_link_libraries(NebulaShaderConductor PRIVATE
    NebulaCore
    dxcompiler
    LLVMDxcSupport
    LLVMSupport
)
