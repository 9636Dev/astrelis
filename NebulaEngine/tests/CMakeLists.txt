enable_testing()

add_executable(NebulaEngine_EngineTests
    src/PointerTest.cpp
    src/ResultTest.cpp
    src/Renderer2DTest.cpp
)

target_link_libraries(NebulaEngine_EngineTests
    NebulaEngine_Engine
    GTest::gtest_main
)

gtest_discover_tests(NebulaEngine_EngineTests)
