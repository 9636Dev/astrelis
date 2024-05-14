cmake_minimum_required(VERSION 3.27)
project(NebulaEditor)

set(NEBULA_EDITOR_SOURCES
    src/NebulaEditor/NebulaEditor.cpp # Maih Entry Point
)

add_executable(NebulaEditor ${NEBULA_EDITOR_SOURCES})

add_dependencies(NebulaEditor
    NebulaCore
)

target_link_libraries(NebulaEditor
    NebulaCore
)

target_include_directories(NebulaEditor PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)
