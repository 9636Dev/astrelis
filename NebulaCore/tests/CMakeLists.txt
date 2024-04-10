cmake_minimum_required(VERSION 3.27)
project(NebulaCoreTests)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(NEBULA_CORE_TEST_SOURCES
    AssertionTests.cpp
)
add_executable(NebulaCoreTests ${NEBULA_CORE_TEST_SOURCES})

enable_testing()
include(GoogleTest)

add_dependencies(NebulaCoreTests NebulaCore)
target_link_libraries(NebulaCoreTests PRIVATE
    NebulaCore
    GTest::gtest_main
)

gtest_discover_tests(NebulaCoreTests)
