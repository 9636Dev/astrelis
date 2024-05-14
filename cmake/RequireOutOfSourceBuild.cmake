# Require out of source build

set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

project(NebulaRequireOutOfSourceBuild NONE)


# We also check manually if the CMAKE_CURRENT_SOURCE_DIR is the same as the CMAKE_CURRENT_BINARY_DIR
if (${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_BINARY_DIR})
    message(FATAL_ERROR "In-source build is not allowed. Please create a build directory and run CMake from there. You may need to remove CMakeCache.txt.")
endif()
