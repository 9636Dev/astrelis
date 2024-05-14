# ==============================================================================
# Platform Specific Configuration for NebulaEngine
# ==============================================================================

if (LINUX)
    message(STATUS "NebulaEngine: Configuring for Linux")
    set(NEBULA_PLATFORM "LINUX")

    find_program(CLANG "clang")
    find_program(CLANGXX "clang++")

    if (NOT CLANG OR NOT CLANGXX)
        message(FATAL_ERROR "NebulaEngine: Clang not found")
    endif()

    set(CMAKE_CXX_COMPILER "${CLANGXX}")
    set(CMAKE_C_COMPILER "${CLANG}")
    message(STATUS "NebulaEngine: Using Clang as compiler")

    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
    message(STATUS "NebulaEngine: Using PIC")

    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -lm")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lm")
    message(STATUS "NebulaEngine: Linking math library")
elseif (WIN32)
    message(STATUS "NebulaEngine: Configuring for Windows")
    set(NEBULA_PLATFORM "WINDOWS")
elseif (APPLE)
    message(STATUS "NebulaEngine: Configuring for MacOS")
    set(NEBULA_PLATFORM "MACOS")

    # On mac gcc is an alias to clang
else()
    message(FATAL_ERROR "NebulaEngine: Unsupported platform")
endif()
