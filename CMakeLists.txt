cmake_minimum_required(VERSION 3.27)
project(NebulaEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(cmake/RequireOutOfSourceBuild.cmake)
include(cmake/DefaultConfig.cmake)
include(cmake/Platform.cmake)
include(cmake/Testing.cmake)

add_subdirectory(NebulaCore)
