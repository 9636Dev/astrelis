project(AstrelisEditor)

add_subdirectory(ShaderCompiler)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PULSAR_EDITOR_SOURCES
    src/AstrelisEditor/Application.cpp
    src/AstrelisEditor/EditorLayer.hpp
    src/AstrelisEditor/EditorLayer.cpp
    src/AstrelisEditor/AssetPanel.hpp
    src/AstrelisEditor/AssetPanel.cpp
    src/AstrelisEditor/FileTree.hpp
    src/AstrelisEditor/FileTree.cpp
    src/AstrelisEditor/Console.hpp
    src/AstrelisEditor/Console.cpp
)

add_executable(Astrelis_Editor ${PULSAR_EDITOR_SOURCES})
target_include_directories(Astrelis_Editor PRIVATE src)

target_link_libraries(Astrelis_Editor
    Astrelis::Engine
    Astrelis_EditorShaderCompiler
)

if (ASTRELIS_PEDANTIC_WARNINGS)
    if (ASTRELIS_COMPILER STREQUAL "CLANG" OR ASTRELIS_COMPILE STREQUAL "GCC")
        target_compile_options(Astrelis_Editor PRIVATE -Wall -Wextra -Wpedantic -Werror)
    elseif (MSVC)
        set(Astrelis_Editor /W4 /WX)
    else()
        message(WARNING "Unknown compiler, can't enable warnings")
    endif()
endif()

if (ASTRELIS_FORMAT)
    file(GLOB_RECURSE ASTRELIS_ENGINE_FORMAT_SOURCES
        src/*.hpp
        src/*.cpp
    )
    add_custom_target(AstrelisEditor_Format
        COMMAND clang-format -style=file -i ${ASTRELIS_ENGINE_FORMAT_SOURCES}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
    add_dependencies(Astrelis_Editor AstrelisEditor_Format)
endif()
