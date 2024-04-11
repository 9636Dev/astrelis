cmake_minimum_required(VERSION 3.27)
project(NebulaShaderCompilerTests)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(NEBULA_SHADER_COMPILER_TEST_SOURCES
    LexerTests.cpp
    #LexerFuzz.cpp
)

add_executable(NebulaShaderCompilerTests ${NEBULA_SHADER_COMPILER_TEST_SOURCES})

enable_testing()
include(GoogleTest)

add_dependencies(NebulaShaderCompilerTests NebulaShaderCompiler)
target_link_libraries(NebulaShaderCompilerTests PRIVATE
    NebulaShaderCompiler
    GTest::gtest_main
)

#link_fuzztest(NebulaShaderCompilerTests)
gtest_discover_tests(NebulaShaderCompilerTests)
