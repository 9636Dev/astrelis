set(NEBULA_CORE_BENCHMARK_SOURCES
    src/Benchmark.cpp
    src/BMLogger.cpp
    src/BMResult.cpp
    src/BMPointer.cpp
)

add_executable(NebulaCoreBenchmarks ${NEBULA_CORE_BENCHMARK_SOURCES})
nebula_target(NebulaCoreBenchmarks)
nebula_format_project(NebulaCoreBenchmarks ${CMAKE_CURRENT_SOURCE_DIR} "${NEBULA_CORE_BENCHMARK_SOURCES}")
nebula_format_depends(NebulaCore NebulaCoreBenchmarks)

target_link_libraries(NebulaCoreBenchmarks
    NebulaCore
    benchmark::benchmark
)
