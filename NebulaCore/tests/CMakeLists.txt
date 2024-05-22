set(NEBULA_CORE_TEST_SOURCES
    src/ResultTest.cpp
    src/PointerTest.cpp
)

add_executable(NebulaCoreTests ${NEBULA_CORE_TEST_SOURCES})
nebula_target(NebulaCoreTests)
nebula_format_project(NebulaCoreTests ${CMAKE_CURRENT_SOURCE_DIR} "${NEBULA_CORE_TEST_SOURCES}")
nebula_format_depends(NebulaCore NebulaCoreTests)

target_link_libraries(NebulaCoreTests
    NebulaCore
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(NebulaCoreTests)
