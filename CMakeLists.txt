# Main CMake List for NebulaEngine


# CMake version
cmake_minimum_required(VERSION 3.27)
if (UNIX) # We still use msvc on windows
    set(CMAKE_CXX_COMPILER "clang++")
endif()

project(NebulaEngine VERSION 0.0.1)

# Set C++ Flags
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if (UNIX)
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread -lm")
endif()


# Fuzzing
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)

# Add Project Dependencies
#add_subdirectory(vendor/fuzztest)
add_subdirectory(vendor/googletest)

# Set Directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
# Used by Visual Studio
set(NEBULA_RUNTIME_DIR ${CMAKE_SOURCE_DIR}/run)

# We add the submodules
add_subdirectory(NebulaCore)
add_subdirectory(NebulaGraphics)
add_subdirectory(NebulaShaders)
add_subdirectory(examples) # NebulaExamples
