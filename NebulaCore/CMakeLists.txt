cmake_minimum_required(VERSION 3.27)
project(NebulaCore)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Initialize Libraries
add_subdirectory(vendor/spdlog)

set(NEBULA_CORE_SOURCES
    src/NebulaCore/Core.hpp
    src/NebulaCore/Util.hpp
    src/NebulaCore/Result.hpp
    src/NebulaCore/Log.hpp
    src/NebulaCore/Log.cpp
)

set(NEBULA_CORE_LIBRARY_TYPE "Inherit" CACHE STRING "The type of library to build")
set_property(CACHE NEBULA_CORE_LIBRARY_TYPE PROPERTY STRINGS "Inherit" "Static" "Shared")

if (NEBULA_CORE_LIBRARY_TYPE STREQUAL "Inherit")
    add_library(NebulaCore ${NEBULA_LIBRARY_TYPE} ${NEBULA_CORE_SOURCES})
    nebula_add_definitions(NebulaCore "NEBULA_CORE" ${NEBULA_LIBRARY_TYPE})
else()
    if (NEBULA_CORE_LIBRARY_TYPE STREQUAL "Static")
        add_library(NebulaCore STATIC ${NEBULA_CORE_SOURCES})
    elseif (NEBULA_CORE_LIBRARY_TYPE STREQUAL "Shared")
        add_library(NebulaCore SHARED ${NEBULA_CORE_SOURCES})
    endif()

    nebula_add_definitions(NebulaCore "NEBULA_CORE" ${NEBULA_CORE_LIBRARY_TYPE})
endif()

target_include_directories(NebulaCore PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_definitions(NebulaCore PUBLIC
    # Logging
    $<$<NEBULA_BUILD_TYPE:Debug>:SPDLOG_ACTIVE_LEVEL=0>
    $<$<NEBULA_BUILD_TYPE:Release>:SPDLOG_ACTIVE_LEVEL=1>

    # Build type
    $<$<NEBULA_BUILD_TYPE:Debug>:NEBULA_DEBUG>
    $<$<NEBULA_BUILD_TYPE:Release>:NEBULA_RELEASE>
)

# Dependencies
add_dependencies(NebulaCore spdlog::spdlog)
target_link_libraries(NebulaCore PUBLIC spdlog::spdlog)

# Silence fmtlib warning on MSVC
if (MSVC)
    target_compile_definitions(spdlog PUBLIC _SILENCE_ALL_MS_EXT_DEPRECATION_WARNINGS)
endif()

if (NOT DEFINED NEBULA_DISABLE_TESTS)
    add_subdirectory(tests)
endif()
