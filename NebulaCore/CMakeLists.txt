cmake_minimum_required(VERSION 3.27)

add_subdirectory(vendor/spdlog)

set(NEBULA_CORE_SOURCES
    # Simplified linking
    src/NebulaCore/Linkage.hpp

    # Logging
    src/NebulaCore/Log/Log.hpp
    src/NebulaCore/Log/Log.cpp

    # Utilities
    src/NebulaCore/Util/Assert.hpp

    # Profiling
    src/NebulaCore/Profile/BasicJson.hpp
    src/NebulaCore/Profile/BaseInstrumentor.hpp
    src/NebulaCore/Profile/TimerInstrumentor.hpp
    src/NebulaCore/Profile/TimerInstrumentor.cpp
    src/NebulaCore/Profile/Instrumentor.hpp
    src/NebulaCore/Profile/Instrumentor.cpp
    src/NebulaCore/Profile/Profile.hpp
)

add_library(NebulaCore STATIC ${NEBULA_CORE_SOURCES})
nebula_target(NebulaCore)
nebula_format_project(NebulaCore ${CMAKE_CURRENT_SOURCE_DIR} "${NEBULA_CORE_SOURCES}")

target_include_directories(NebulaCore
PUBLIC
    src
    pch
)

target_compile_definitions(NebulaCore
PUBLIC
    SPDLOG_ACTIVE_LEVEL=${NEBULA_LOGLEVEL}
    $<$<BOOL:${NEBULA_ENABLE_PROFILING}>:NEBULA_ENABLE_PROFILING>
    $<$<STREQUAL:${NEBULA_PLATFORM},WINDOWS>:NEBULA_PLATFORM_WINDOWS>
    $<$<STREQUAL:${NEBULA_PLATFORM},LINUX>:NEBULA_PLATFORM_LINUX>
    $<$<STREQUAL:${NEBULA_PLATFORM},MACOS>:NEBULA_PLATFORM_MACOS>
    $<$<STREQUAL:${NEBULA_BUILD_TYPE},Debug>:NEBULA_DEBUG>
    $<$<STREQUAL:${NEBULA_BUILD_TYPE},Release>:NEBULA_RELEASE>
    $<$<STREQUAL:${NEBULA_BUILD_TYPE},Dist>:NEBULA_RELWITHDEBINFO>
)


target_link_libraries(NebulaCore
PUBLIC
    spdlog
)

if (NEBULA_ENABLE_TESTING)
    add_subdirectory(tests)
endif()
