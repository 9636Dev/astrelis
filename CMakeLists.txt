cmake_minimum_required(VERSION 3.27)

include(cmake/PlatformPreinit.cmake)

project(NebulaEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

include(cmake/Compiler.cmake)
include(cmake/RequireOutOfSourceBuild.cmake)
include(cmake/DefaultConfig.cmake)
include(cmake/Pedantic.cmake)
include(cmake/Testing.cmake)
include(cmake/Util.cmake)
include(cmake/ClangFormat.cmake)

add_subdirectory(NebulaCore)
add_subdirectory(NebulaWindowing)
add_subdirectory(NebulaEditor)

if (NEBULA_ENABLE_TESTING)
    add_custom_target(NebulaEngineTests)
    add_dependencies(NebulaEngineTests
        NebulaCoreTests
    )
endif()

if (NEBULA_ENABLE_CLANG_FORMAT)
    add_custom_target(NebulaEngineFormat)
    add_dependencies(NebulaEngineFormat
        NebulaCore_Format
        NebulaEditor_Format
    )
endif()
