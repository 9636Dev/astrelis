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


option(SPIRV_CROSS_EXCEPTIONS_TO_ASSERTIONS "Instead of throwing exceptions assert" OFF)
option(SPIRV_CROSS_SHARED "Build the C API as a single shared library." OFF)
option(SPIRV_CROSS_STATIC "Build the C and C++ API as static libraries." ON)
option(SPIRV_CROSS_CLI "Build the CLI binary. Requires SPIRV_CROSS_STATIC." OFF)
option(SPIRV_CROSS_ENABLE_TESTS "Enable SPIRV-Cross tests." OFF)

option(SPIRV_CROSS_ENABLE_GLSL "Enable GLSL support." ON)
option(SPIRV_CROSS_ENABLE_HLSL "Enable HLSL target support." ON)
option(SPIRV_CROSS_ENABLE_MSL "Enable MSL target support." ON)
option(SPIRV_CROSS_ENABLE_CPP "Enable C++ target support." OFF)
option(SPIRV_CROSS_ENABLE_REFLECT "Enable JSON reflection target support." OFF)
option(SPIRV_CROSS_ENABLE_C_API "Enable C API wrapper support in static library." OFF)
option(SPIRV_CROSS_ENABLE_UTIL "Enable util module support." OFF)

add_subdirectory(vendor/SPIRV-Cross)

# We restore the old value
set(CMAKE_SUPPRESS_DEVELOPER_WARNINGS ${CMAKE_SUPPRESS_DEVELOPER_WARNINGS_OLD} CACHE STRING "No dev warnings" FORCE)

set(NEBULA_SHADER_CONDUCTOR_SOURCES
    src/NebulaShaderConductor/Core.hpp
    src/NebulaShaderConductor/ShaderInput.hpp
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
nebula_format_project(NebulaShaderConductor ${CMAKE_CURRENT_SOURCE_DIR} "${NEBULA_SHADER_CONDUCTOR_SOURCES}")

target_compile_options(NebulaShaderConductor PRIVATE
    -fms-extensions
    -Wno-language-extension-token
    -Wambiguous-reversed-operator
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
    NebulaShaderCommon

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

target_link_libraries(NebulaShaderConductor PUBLIC
    NebulaCore
    NebulaShaderCommon
)

if (NEBULA_BUILD_TESTS)
    add_subdirectory(tests)
endif()
