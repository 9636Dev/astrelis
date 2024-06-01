add_subdirectory(vendor/spdlog)
add_subdirectory(vendor/gsl)

set(NEBULA_CORE_SOURCES
    src/NebulaCore/Log.hpp
    src/NebulaCore/Log.cpp
    src/NebulaCore/Util/Assert.hpp
    src/NebulaCore/Util/Debug.hpp
    src/NebulaCore/Util/Result.hpp
    src/NebulaCore/Util/Pointer.hpp
)

add_library(NebulaCore STATIC ${NEBULA_CORE_SOURCES})
nebula_target(NebulaCore)
nebula_format_project(NebulaCore ${CMAKE_CURRENT_SOURCE_DIR} "${NEBULA_CORE_SOURCES}")

target_include_directories(NebulaCore PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(NebulaCore PUBLIC
    spdlog::spdlog
    Microsoft.GSL::GSL
)

target_compile_definitions(NebulaCore PUBLIC
    SPDLOG_ACTIVE_LEVEL=${NEBULA_LOGLEVEL}
    $<$<BOOL:${NEBULA_ENABLE_PROFILING}>:NEBULA_ENABLE_PROFILING>
    $<$<STREQUAL:${NEBULA_PLATFORM},Windows>:NEBULA_PLATFORM_WINDOWS>
    $<$<STREQUAL:${NEBULA_PLATFORM},Linux>:NEBULA_PLATFORM_LINUX>
    $<$<STREQUAL:${NEBULA_PLATFORM},MacOS>:NEBULA_PLATFORM_MACOS>
    $<$<STREQUAL:${NEBULA_BUILD_TYPE},Debug>:NEBULA_DEBUG>
    $<$<STREQUAL:${NEBULA_BUILD_TYPE},Release>:NEBULA_RELEASE>
    $<$<STREQUAL:${NEBULA_BUILD_TYPE},Dist>:NEBULA_RELWITHDEBINFO>
)
