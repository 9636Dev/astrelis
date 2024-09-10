cmake_minimum_required(VERSION 3.27)
# Pulsar Editor is a editor for NebulaEngine, which uses NebulaEngine to render the editor.

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PULSAR_EDITOR_SOURCES
    src/Pulsar/Application.cpp
    src/Pulsar/EditorLayer.hpp
    src/Pulsar/EditorLayer.cpp
    src/Pulsar/AssetPanel.hpp
    src/Pulsar/AssetPanel.cpp
    src/Pulsar/FileTree.hpp
    src/Pulsar/FileTree.cpp
    src/Pulsar/GamePreview.hpp
    src/Pulsar/GamePreview.cpp
    src/Pulsar/Console.hpp
    src/Pulsar/Console.cpp
)

add_executable(Astrelis_Editor ${PULSAR_EDITOR_SOURCES})
target_include_directories(Astrelis_Editor PRIVATE src)

target_link_libraries(Astrelis_Editor Astrelis::Engine)
