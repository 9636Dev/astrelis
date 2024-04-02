cmake_minimum_required(VERSION 3.27)
project(NebulaCore)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ============================
# Variables and Util Functions
# ============================

if (NOT DEFINED NEBULA_LIBRARY_TYPE)
    set(NEBULA_LIBRARY_TYPE SHARED)
else()
    if (NEBULA_LIBRARY_TYPE STREQUAL "STATIC")
        set(NEBULA_LIBRARY_TYPE STATIC)
    elseif(NEBULA_LIBRARY_TYPE STREQUAL "SHARED")
        set(NEBULA_LIBRARY_TYPE SHARED)
    else()
        message(FATAL_ERROR "Invalid NEBULA_LIBRARY_TYPE: ${NEBULA_LIBRARY_TYPE}")
    endif()
endif()

function(nebula_add_definitions target define_prefix type)
    # If it is shared
    if(${type} STREQUAL "SHARED")
        target_compile_definitions(${target} PRIVATE NEBULA_EXPORT_DLL)
        target_compile_definitions(${target} PUBLIC ${define_prefix}_SHARED)
    else()
        target_compile_definitions(${target} PUBLIC ${define_prefix}_STATIC)
    endif()
endfunction()

# ============================

# Initialize Libraries
add_subdirectory(vendor/spdlog)

set(NEBULA_CORE_SOURCES
    src/NebulaCore/Core.hpp
    src/NebulaCore/Util.hpp
    src/NebulaCore/Log.hpp
    src/NebulaCore/Log.cpp
)

add_library(NebulaCore ${NEBULA_LIBRARY_TYPE} ${NEBULA_CORE_SOURCES})
nebula_add_definitions(NebulaCore "NEBULA_CORE" SHARED)
target_include_directories(NebulaCore PUBLIC src)

target_compile_definitions(NebulaCore PUBLIC
    # Logging
    $<$<CONFIG:Debug>:SPDLOG_ACTIVE_LEVEL=0>
    $<$<CONFIG:Release>:SPDLOG_ACTIVE_LEVEL=1>
    $<$<CONFIG:RelWithDebInfo>:SPDLOG_ACTIVE_LEVEL=1>
    $<$<CONFIG:MinSizeRel>:SPDLOG_ACTIVE_LEVEL=1>

    # Build type
    $<$<CONFIG:Debug>:NEBULA_DEBUG>
    $<$<CONFIG:Release>:NEBULA_RELEASE>
    $<$<CONFIG:RelWithDebInfo>:NEBULA_RELWITHDEBINFO>
    $<$<CONFIG:RelWithDebInfo>:NEBULA_RELEASE>
    $<$<CONFIG:MinSizeRel>:NEBULA_MINSIZEREL>
    $<$<CONFIG:MinSizeRel>:NEBULA_RELEASE>
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
