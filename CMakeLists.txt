cmake_minimum_required(VERSION 3.27)

if (LINUX)
    message(STATUS "Astrelis: Configuring for Linux")
    set(ASTRELIS_PLATFORM "Linux")

    # We need to use clang
    find_program(CLANG clang)
    find_program(CLANGXX clang++)

    if (NOT CLANG)
        message(FATAL_ERROR "Astrelis: Clang not found")
    endif()

    if (NOT CLANGXX)
        message(FATAL_ERROR "Astrelis: Clang++ not found")
    endif()

    set(CMAKE_C_COMPILER ${CLANG})
    set(CMAKE_CXX_COMPILER ${CLANGXX})
    message(STATUS "Astrelis: Using Clang as compiler")

    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
    message(STATUS "Astrelis: Using PIC")

    # We may need to link with math library
    # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lm")
    # message(STATUS "Astrelis: Linking with math library")
elseif(APPLE)
    message(STATUS "Astrelis: Configuring for MacOS")
    set(ASTRELIS_PLATFORM "MacOS")
elseif(WIN32)
    message(STATUS "Astrelis: Configuring for Windows")
    set(ASTRELIS_PLATFORM "Windows")
else()
    message(FATAL_ERROR "Astrelis: Unsupported platform")
endif()

# DXC modifies its own source
#set(CMAKE_DISABLE_SOURCE_CHANGES ON)
#set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

if (${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_BINARY_DIR})
    message(FATAL_ERROR "In-source builds are not allowed. Please create a build directory and run 'cmake path_to_source'")
endif()

project(Astrelis VERSION 0.0.1)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(ASTRELIS_COMPILER "CLANG")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
    set(ASTRELIS_COMPILER "CLANG")
    message(STATUS "Astrelis: AppleClang detected, using Clang flags")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(ASTRELIS_COMPILER "GCC")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(ASTRELIS_COMPILER "MSVC")
else()
    message(FATAL_ERROR "Astrelis: Unsupported compiler - '${CMAKE_CXX_COMPILER_ID}'")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Find clang format
find_program(CLANG_FORMAT clang-format)
if (NOT CLANG_FORMAT)
    message(STAUS "Astrelis: Disabling clang-format (not found)")
    option(ASTRELIS_FORMAT "Enable clang-format" OFF)
else()
    message(STATUS "Astrelis: Found clang-format")
    option(ASTRELIS_FORMAT "Enable clang-format" ON)
endif()

find_package(GTest)
find_package(benchmark)

include(GoogleTest)
add_subdirectory(Engine)
add_library(Astrelis::Engine ALIAS Astrelis_Engine)
add_subdirectory(Editor) # Editor
add_executable(Astrelis::Editor ALIAS Astrelis_Editor)

