enable_testing()

add_executable(Astrelis_EngineTests
    src/PointerTest.cpp
    src/ResultTest.cpp
    src/Renderer2DTest.cpp
)

target_link_libraries(Astrelis_EngineTests
    Astrelis_Engine
    GTest::gtest_main
)

gtest_discover_tests(Astrelis_EngineTests)
