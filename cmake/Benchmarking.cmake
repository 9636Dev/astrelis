# ==============================================================================
# Benchmarking
# ==============================================================================

if (NEBULA_ENABLE_BENCHMARKING)
    enable_testing()
    add_subdirectory(vendor/benchmark)
    set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "Suppressing benchmark's tests" FORCE)
    if (NEBULA_COMPILER STREQUAL "MSVC")
        target_compile_options(benchmark PRIVATE /W0)
    else()
        target_compile_options(benchmark PRIVATE -w)
    endif()

    message(STATUS "NebulaEngine: Benchmarking is enabled")
else()
    message(STATUS "NebulaEngine: Benchmarking is disabled")
endif()

