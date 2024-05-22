cmake_minimum_required(VERSION 3.27)

set(NEBULA_EDITOR_SOURCES
    src/NebulaEditor/NebulaEditor.cpp # Maih Entry Point
)

add_executable(NebulaEditor ${NEBULA_EDITOR_SOURCES})
nebula_target(NebulaEditor)
nebula_format_project(NebulaEditor ${CMAKE_CURRENT_SOURCE_DIR} "${NEBULA_EDITOR_SOURCES}")

add_dependencies(NebulaEditor
    NebulaCore
)

add_dependencies(NebulaEditor
    NebulaCore
    NebulaWindowing
    NebulaRenderer
)

target_link_libraries(NebulaEditor
    NebulaCore
    NebulaWindowing
    NebulaRenderer
)

target_include_directories(NebulaEditor PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)
