set(NEBULA_CORE_TEST_SOURCES
    src/ResultTest.cpp
    src/PointerTest.cpp
)

add_executable(NebulaCoreTests ${NEBULA_CORE_TEST_SOURCES})
target_link_libraries(NebulaCoreTests
    NebulaCore
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(NebulaCoreTests)
