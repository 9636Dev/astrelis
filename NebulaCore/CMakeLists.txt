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
nebula_library_type(NEBULA_CORE_LIBRARY_TYPE NEBULA_CORE_LIBRARY_TYPE_FINAL)
message(STATUS "NebulaEngine: NebulaCore built as: ${NEBULA_CORE_LIBRARY_TYPE_FINAL}")
add_library(NebulaCore ${NEBULA_CORE_LIBRARY_TYPE_FINAL} ${NEBULA_CORE_SOURCES})
nebula_add_definitions(NebulaCore "NEBULA_CORE" ${NEBULA_CORE_LIBRARY_TYPE_FINAL})
nebula_format_project(NebulaCore ${CMAKE_CURRENT_SOURCE_DIR} "${NEBULA_CORE_SOURCES}")
target_compile_options(NebulaCore PRIVATE "${NEBULA_CXX_FLAGS}")

target_include_directories(NebulaCore PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

if (NEBULA_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(NebulaCore PUBLIC
        SPDLOG_ACTIVE_LEVEL=0
        NEBULA_DEBUG
    )
elseif (NEBULA_BUILD_TYPE STREQUAL "Release")
    target_compile_definitions(NebulaCore PUBLIC
        SPDLOG_ACTIVE_LEVEL=1
        NEBULA_RELEASE
    )
endif()
# Dependencies
add_dependencies(NebulaCore spdlog::spdlog)
target_link_libraries(NebulaCore PUBLIC spdlog::spdlog)

# Silence fmtlib warning on MSVC
if (MSVC)
    target_compile_definitions(spdlog PUBLIC _SILENCE_ALL_MS_EXT_DEPRECATION_WARNINGS)
endif()

if (NEBULA_BUILD_TESTS)
    add_subdirectory(tests)
endif()
