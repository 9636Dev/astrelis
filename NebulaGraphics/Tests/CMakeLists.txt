cmake_minimum_required(VERSION 3.27)
project(NebulaGraphicsTests)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_NEBULA_SHADER_TEST_MOCK_SOURCES
    mock/WindowMock.hpp
    mock/WindowMock.cpp
)

add_library(NebulaShaderTestMock STATIC ${CMAKE_NEBULA_SHADER_TEST_MOCK_SOURCES})

target_include_directories(NebulaShaderTestMock PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/mock)

target_link_libraries(NebulaShaderTestMock PUBLIC NebulaGraphicsCore)

set(CMAKE_NEBULA_SHADER_TEST_SOURCES
    src/WindowTest.cpp
)

enable_testing()
add_executable(NebulaShaderTest ${CMAKE_NEBULA_SHADER_TEST_SOURCES})

target_link_libraries(NebulaShaderTest PRIVATE
    NebulaShaderTestMock
    gtest_main
)

