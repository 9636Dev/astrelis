cmake_minimum_required(VERSION 3.27)
project(NebulaCore)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(NEBULA_CORE_SOURCES
    # Simplified linking
    src/NebulaCore/Linkage.hpp

    # Logging
    src/NebulaCore/Log/Log.hpp
    src/NebulaCore/Log/Log.cpp
)

add_library(NebulaCore STATIC ${NEBULA_CORE_SOURCES})
target_include_directories(NebulaCore PUBLIC src)

target_compile_definitions(NebulaCore
PUBLIC
    SPDLOG_ACTIVE_LEVEL=${NEBULA_LOG_LEVEL}
    $<$<NEBULA_PLATFORM:WINDOWS>:NEBULA_PLATFORM_WINDOWS>
    $<$<NEBULA_PLATFORM:LINUX>:NEBULA_PLATFORM_LINUX>
    $<$<NEBULA_PLATFORM:MACOS>:NEBULA_PLATFORM_MACOS>
)
