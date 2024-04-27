cmake_minimum_required(VERSION 3.27)
project(NebulaShaderBuilderTests)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(NEBULA_SHADER_BUILDER_TEST_SOURCES
    BuilderGlslTests.cpp
)

add_executable(NebulaShaderBuilderTests ${NEBULA_SHADER_BUILDER_TEST_SOURCES})

enable_testing()
include(GoogleTest)

add_dependencies(NebulaShaderBuilderTests NebulaShaderBuilder)
target_link_libraries(NebulaShaderBuilderTests PRIVATE
    NebulaShaderBuilder
    gtest_main
)

#link_fuzztest(NebulaShaderBuilderTests)
gtest_discover_tests(NebulaShaderBuilderTests)
