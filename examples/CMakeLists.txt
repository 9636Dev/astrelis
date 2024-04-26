cmake_minimum_required(VERSION 3.27)
project(NebulaExamples)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(NEBULA_EXAMPLES_SOURCES
    src/Application.cpp
)

add_executable(NebulaExamples ${NEBULA_EXAMPLES_SOURCES})
add_dependencies(NebulaExamples
    NebulaCore
    NebulaGraphics
)
target_link_libraries(NebulaExamples NebulaCore)
target_link_libraries(NebulaExamples NebulaGraphicsCore)
target_link_libraries(NebulaExamples NebulaGraphicsRenderer)
