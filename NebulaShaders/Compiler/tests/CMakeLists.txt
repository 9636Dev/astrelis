cmake_minimum_required(VERSION 3.27)
project(NebulaShaderCompilerTests)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(NEBULA_SHADER_COMPILER_TEST_SOURCES
    LexerTests.cpp
)

if (LINUX)
    set(NEBULA_SHADER_COMPILER_FUZZ_TEST_SOURCES
        LexerFuzz.cpp
    )
    fuzztest_setup_fuzzing_flags()
else()
    set(NEBULA_SHADER_COMPILER_FUZZ_TEST_SOURCES)
endif()
add_executable(NebulaShaderCompilerTests ${NEBULA_SHADER_COMPILER_TEST_SOURCES} ${NEBULA_SHADER_COMPILER_FUZZ_TEST_SOURCES})

enable_testing()
include(GoogleTest)

add_dependencies(NebulaShaderCompilerTests NebulaShaderCompiler)
target_link_libraries(NebulaShaderCompilerTests PRIVATE
    NebulaShaderCompiler
    GTest::gtest_main
)

if (LINUX)
    link_fuzztest(first_fuzz_test)
endif()
gtest_discover_tests(NebulaShaderCompilerTests)
