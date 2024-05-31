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
else()
    message(FATAL_ERROR "NebulaEngine: Unsupported platform")
endif()
