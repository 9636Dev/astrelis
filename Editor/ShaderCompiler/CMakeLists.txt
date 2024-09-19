cmake_minimum_required(VERSION 3.27)
project(Astrelis_Editor_ShaderCompiler)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(vendor/DirectXShaderCompiler/cmake/caches/PredefinedParams.cmake)
set(SPIRV_BUILD_TESTS OFF CACHE BOOL "Build SPIRV tests" FORCE)
set(LLVM_INCLUDE_TESTS OFF CACHE BOOL "Include LLVM tests" FORCE)
set(HLSL_INCLUDE_TESTS OFF CACHE BOOL "Include HLSL tests" FORCE)
set(HLSL_COPY_GENERATED_SOURCES ON CACHE BOOL "Copy generated HLSL sources" FORCE)
# We need to include tools to build DXC
#set(LLVM_INCLUDE_TOOLS OFF CACHE BOOL "Include LLVM tools" FORCE)
set(LLVM_INCLUDE_UTILS OFF CACHE BOOL "Generate build targets for the LLVM utils." FORCE)
set(LLVM_BUILD_RUNTIME OFF CACHE BOOL "Build the LLVM runtime libraries." FORCE)
set(LLVM_BUILD_EXAMPLES OFF CACHE BOOL "Build the LLVM example programs. If OFF, just generate build targets." FORCE)
set(LLVM_INCLUDE_EXAMPLES OFF CACHE BOOL "Generate build targets for the LLVM examples" FORCE)
set(LLVM_BUILD_DOCS OFF CACHE BOOL "Build the llvm documentation." FORCE)
set(LLVM_INCLUDE_DOCS OFF CACHE BOOL "Generate build targets for llvm documentation." FORCE)

if (ASTRELIS_BUILD_TYPE STREQUAL "Debug")
    set(HLSL_ENABLE_DEBUG_ITERATORS ON CACHE BOOL "Enable HLSL debug iterator" FORCE)
endif()

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
option(SPIRV_CROSS_ENABLE_UTIL "Enable util module support." ON)

add_subdirectory(vendor/SPIRV-Cross)

# We restore the old value
set(CMAKE_SUPPRESS_DEVELOPER_WARNINGS ${CMAKE_SUPPRESS_DEVELOPER_WARNINGS_OLD} CACHE STRING "No dev warnings" FORCE)

set(ASTRELIS_EDITOR_SHADER_COMPILER_SOURCES
    src/ShaderCompiler/ShaderConductor.cpp
    src/ShaderCompiler/ShaderCompiler.cpp
)

add_library(Astrelis_EditorShaderCompiler ${ASTRELIS_EDITOR_SHADER_COMPILER_SOURCES})
if (ASTRELIS_COMPILER STREQUAL "CLANG")
    target_compile_options(Astrelis_EditorShaderCompiler PRIVATE
        -fms-extensions
        -Wno-language-extension-token
        -Wambiguous-reversed-operator
    )
else()
    message(FATAL_ERROR "Unsupported compiler: ${ASTRELIS_COMPILER}")
endif()

target_include_directories(Astrelis_EditorShaderCompiler PRIVATE
    vendor/DirectXShaderCompiler/include
    vendor/SPIRV-Cross
)

target_include_directories(Astrelis_EditorShaderCompiler
    PUBLIC
        src

    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/vendor/DirectXShaderCompiler/include
        ${CMAKE_CURRENT_BINARY_DIR}/vendor/DirectXShaderCompiler/include
)
add_dependencies(Astrelis_EditorShaderCompiler
    Astrelis_Engine

    # DXC
    dxc-headers
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
target_link_libraries(Astrelis_EditorShaderCompiler PRIVATE
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

target_link_libraries(Astrelis_EditorShaderCompiler PUBLIC
    Astrelis_Engine
)

#add_subdirectory(tests)

if (ASTRELIS_FORMAT)
    file(GLOB_RECURSE ASTRELIS_ENGINE_FORMAT_SOURCES
        src/*.hpp
        src/*.cpp
    )
    add_custom_target(Astrelis_EditorShaderCompiler_Format
        COMMAND clang-format -style=file -i ${ASTRELIS_ENGINE_FORMAT_SOURCES}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
    add_dependencies(Astrelis_EditorShaderCompiler Astrelis_EditorShaderCompiler_Format)
endif()
