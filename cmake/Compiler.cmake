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

