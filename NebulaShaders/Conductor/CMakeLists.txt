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

# We suppress the warnings for cmake
# We cache the current value
set(CMAKE_SUPPRESS_DEVELOPER_WARNINGS_OLD ${CMAKE_SUPPRESS_DEVELOPER_WARNINGS})
set(CMAKE_SUPPRESS_DEVELOPER_WARNINGS 1 CACHE STRING "No dev warnings" FORCE)

add_subdirectory(vendor/DirectXShaderCompiler)
add_subdirectory(vendor/SPIRV-Cross)

# We restore the old value
set(CMAKE_SUPPRESS_DEVELOPER_WARNINGS ${CMAKE_SUPPRESS_DEVELOPER_WARNINGS_OLD} CACHE STRING "No dev warnings" FORCE)

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

set(NEBULA_SHADER_CONDUCTOR_LIBRARY_TYPE "Inherit" CACHE STRING "The type of library to build")
set_property(CACHE NEBULA_SHADER_CONDUCTOR_LIBRARY_TYPE PROPERTY STRINGS "Inherit" "Static" "Shared")
nebula_library_type(NEBULA_SHADER_CONDUCTOR_LIBRARY_TYPE NEBULA_SHADER_CONDUCTOR_LIBRARY_TYPE_FINAL)
message(STATUS "NebulaEngine: NebukaShaderConductor built as: ${NEBULA_SHADER_CONDUCTOR_LIBRARY_TYPE_FINAL}")
add_library(NebulaShaderConductor ${NEBULA_SHADER_CONDUCTOR_LIBRARY_TYPE_FINAL} ${NEBULA_SHADER_CONDUCTOR_SOURCES})
nebula_add_definitions(NebulaShaderConductor "NEBULA_SHADER_CONDUCTOR" ${NEBULA_SHADER_CONDUCTOR_LIBRARY_TYPE_FINAL})

target_compile_options(NebulaShaderConductor PRIVATE
    -fms-extensions
    -Wno-language-extension-token
)

target_include_directories(NebulaShaderConductor PRIVATE
    vendor/DirectXShaderCompiler/include
    vendor/SPIRV-Cross
)

target_include_directories(NebulaShaderConductor PUBLIC
    src
)
add_dependencies(NebulaShaderConductor
    NebulaCore

    # DXC
    dxcompiler
    LLVMDxcSupport
    LLVMSupport

    # SPIRV-Cross
    spirv-cross-core
    spirv-cross-glsl
    spirv-cross-hlsl
    spirv-cross-msl
    spirv-cross-util
    SPIRV-Tools
)
target_link_libraries(NebulaShaderConductor PRIVATE
    NebulaCore

    # DXC
    dxcompiler
    LLVMDxcSupport
    LLVMSupport

    # SPIRV-Cross
    spirv-cross-core
    spirv-cross-glsl
    spirv-cross-hlsl
    spirv-cross-msl
    spirv-cross-util
    SPIRV-Tools
)
