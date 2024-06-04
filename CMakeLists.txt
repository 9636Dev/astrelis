cmake_minimum_required(VERSION 3.27)

if (LINUX)
    message(STATUS "NebulaEngine: Configuring for Linux")
    set(NEBULA_PLATFORM "Linux")

    # We need to use clang
    find_program(CLANG clang)
    find_program(CLANGXX clang++)

    if (NOT CLANG)
        message(FATAL_ERROR "NebulaEngine: Clang not found")
    endif()

    if (NOT CLANGXX)
        message(FATAL_ERROR "NebulaEngine: Clang++ not found")
    endif()

    set(CMAKE_C_COMPILER ${CLANG})
    set(CMAKE_CXX_COMPILER ${CLANGXX})
    message(STATUS "NebulaEngine: Using Clang as compiler")

    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
    message(STATUS "NebulaEngine: Using PIC")

    # We may need to link with math library
    # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lm")
    # message(STATUS "NebulaEngine: Linking with math library")
elseif(APPLE)
    message(STATUS "NebulaEngine: Configuring for Linux")
    set(NEBULA_PLATFORM "MacOS")
else()
    message(FATAL_ERROR "NebulaEngine: Unsupported platform")
endif()

set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

if (${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_BINARY_DIR})
    message(FATAL_ERROR "In-source builds are not allowed. Please create a build directory and run 'cmake path_to_source'")
endif()



project(NebulaEngine VERSION 0.0.1)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(NEBULA_COMPILER "CLANG")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
    set(NEBULA_COMPILER "CLANG")
    message(STATUS "NebulaEngine: AppleClang detected, using Clang flags")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(NEBULA_COMPILER "GCC")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(NEBULA_COMPILER "MSVC")
else()
    message(FATAL_ERROR "NebulaEngine: Unsupported compiler - '${CMAKE_CXX_COMPILER_ID}'")
endif()


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

add_subdirectory(NebulaEngine)
add_library(NebulaEngine::Engine ALIAS NebulaEngine_Engine)
#add_library(NebulaEngine::Format ALIAS NebulaEngine_Format)
add_subdirectory(Sandbox)

