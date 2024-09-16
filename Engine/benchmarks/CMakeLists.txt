project(Astrelis_EngineProfiling VERSION 0.0.1)

add_executable(Astrelis_EngineProfiling
    src/main.cpp
)

target_link_libraries(Astrelis_EngineProfiling
    PRIVATE
        Astrelis_Engine
        benchmark::benchmark
)

target_include_directories(Astrelis_EngineProfiling
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)
