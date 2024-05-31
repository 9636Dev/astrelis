set(NEBULA_EDITOR_SOURCES
    # Editor Entry Point
    src/NebulaEditor/Application.cpp
)

add_executable(NebulaEditor ${NEBULA_EDITOR_SOURCES})

target_include_directories(NebulaEditor PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_dependencies(NebulaEditor
    NebulaCore
    NebulaRenderer
)

target_link_libraries(NebulaEditor
    NebulaCore
    NebulaRenderer
)
