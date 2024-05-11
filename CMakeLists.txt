# Main CMake List for NebulaEngine


# We assume you are using Clang or MSVC, GCC support may come later

# CMake version
cmake_minimum_required(VERSION 3.27)

if (LINUX)
    message(STATUS "NebulaEngine: Linux detected - Setting up to use Clang")

    set(CMAKE_CXX_COMPILER "clang++")
    set(CMAKE_C_COMPILER "clang")

    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread -lm")
endif()

project(NebulaEngine VERSION 0.0.1)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(NEBULA_RUNTIME_DIR ${CMAKE_SOURCE_DIR}/run)

include(cmake/UtilFunctions.cmake)
include(cmake/DefaultConfig.cmake)
include(cmake/VerifyConfig.cmake)

set(NEBULA_CXX_FLAGS)
if (NEBULA_ENABLE_PEDANTIC)
    if (MSVC)
        # set(NEBULA_CXX_FLAGS ${NEBULA_CXX_FLAGS} /W4)
        # We need to disable warning 4251 for MSVC
        set(NEBULA_CXX_FLAGS ${NEBULA_CXX_FLAGS} /wd4251)
    else()
        set(NEBULA_CXX_FLAGS ${NEBULA_CXX_FLAGS} -Wall -Wextra -Wpedantic)
    endif()
endif()

# Find Clang format
if (NEBULA_ENABLE_CLANG_FORMAT)
    find_program(CLANG_FORMAT "clang-format")
    if (NOT CLANG_FORMAT)
        message(FATAL_ERROR "Clang-format not found")
    endif()
endif()

# Set C++ Flags
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


if (NEBULA_USE_PTHREADS)
    set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
    set(THREADS_PREFER_PTHREAD_FLAG TRUE)
    find_package(Threads REQUIRED)
endif()

# ========================
# Project Dependencies
# ========================
#add_subdirectory(vendor/fuzztest)
if (NEBULA_BUILD_TESTS AND NOT TARGET gtest_main) # If we are building tests and gtest_main is not a target
    add_subdirectory(vendor/googletest)
endif()

# We add the submodules
add_subdirectory(NebulaCore)
add_subdirectory(NebulaIO)
add_subdirectory(NebulaGraphics)
add_subdirectory(NebulaShaders)
add_subdirectory(examples) # NebulaExamples

# We define a target for format
if (NEBULA_ENABLE_CLANG_FORMAT)
    add_custom_target(format)
    add_dependencies(format
        NebulaCore_format
        NebulaGraphics_format
        NebulaIO_format
        NebulaShaders_format
    )
endif()
