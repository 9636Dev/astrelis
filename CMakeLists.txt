cmake_minimum_required(VERSION 3.27)

include(cmake/PreInit.cmake)

project(NebulaEngine VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

include(cmake/OutOfSourceBuild.cmake)
include(cmake/Config.cmake)
include(cmake/Compiler.cmake)
include(cmake/Pedantic.cmake)
include(cmake/ClangFormat.cmake)

add_subdirectory(Core)
add_subdirectory(Renderer)
add_subdirectory(Editor)

if (NEBULA_ENABLE_CLANG_FORMAT)
    add_custom_target(NebulaEngineFormat)
    add_dependencies(NebulaEngineFormat
        NebulaCore_Format
        NebulaEditor_Format
        NebulaRenderer_Format
    )
endif()
