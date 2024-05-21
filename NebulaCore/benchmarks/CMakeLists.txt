set(NEBULA_CORE_BENCHMARK_SOURCES
    src/Benchmark.cpp
    src/BMLogger.cpp
    src/BMResult.cpp
    src/BMPointer.cpp
)

add_executable(NebulaCoreBenchmarks ${NEBULA_CORE_BENCHMARK_SOURCES})
target_link_libraries(NebulaCoreBenchmarks
    NebulaCore
    benchmark::benchmark
)
