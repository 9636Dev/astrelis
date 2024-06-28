enable_testing()

add_executable(NebulaEngine_EngineTests
    src/PointerTest.cpp
)

target_link_libraries(NebulaEngine_EngineTests
    NebulaEngine_Engine
    gtest_main
)

gtest_discover_tests(NebulaEngine_EngineTests)
