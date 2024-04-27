cmake_minimum_required(VERSION 3.27)
project(NebulaShaderConductorTests)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(NEBULA_SHADER_CONDUCTOR_TEST_SOURCES
    TestSources.hpp
    SpirvTest.cpp
    GlslTest.cpp
)

add_executable(NebulaShaderConductorTests ${NEBULA_SHADER_CONDUCTOR_TEST_SOURCES})

enable_testing()
include(GoogleTest)

add_dependencies(NebulaShaderConductorTests NebulaShaderConductor)
target_link_libraries(NebulaShaderConductorTests PRIVATE
    NebulaShaderConductor
    gtest_main
)

#link_fuzztest(NebulaShaderConductorTests)
gtest_discover_tests(NebulaShaderConductorTests)
