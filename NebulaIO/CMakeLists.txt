cmake_minimum_required(VERSION 3.27)
project(NebulaIO)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(NEBULA_IO_SOURCES
    src/NebulaIO/Core.hpp
    src/NebulaIO/File.hpp
    src/NebulaIO/File.cpp
)

set(BUILD_DOCS OFF CACHE BOOL "Build documentation" FORCE)
set(BUILD_SANDBOX OFF CACHE BOOL "Build sandbox" FORCE)
set(SKIP_PERFORMANCE_COMPARISON ON CACHE BOOL "Skip performance comparison to avoid Boost dependency" FORCE)
add_subdirectory(vendor/cereal)

set(NEBULA_IO_LIBRARY_TYPE "Inherit" CACHE STRING "The type of library to build")
set_property(CACHE NEBULA_IO_LIBRARY_TYPE PROPERTY STRINGS "Inherit" "Static" "Shared")
nebula_library_type(NEBULA_IO_LIBRARY_TYPE NEBULA_IO_LIBRARY_TYPE_FINAL)
message(STATUS "NebulaEngine: NebulaIO built as: ${NEBULA_IO_LIBRARY_TYPE_FINAL}")
add_library(NebulaIO ${NEBULA_IO_LIBRARY_TYPE_FINAL} ${NEBULA_IO_SOURCES})
nebula_add_definitions(NebulaIO "NEBULA_IO" ${NEBULA_IO_LIBRARY_TYPE_FINAL})
nebula_format_project(NebulaIO ${CMAKE_CURRENT_SOURCE_DIR} "${NEBULA_IO_SOURCES}")

target_include_directories(NebulaIO PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Dependencies
add_dependencies(NebulaIO
    NebulaCore
    cereal
)
target_link_libraries(NebulaIO PUBLIC
    NebulaCore
    cereal
)
