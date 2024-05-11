cmake_minimum_required(VERSION 3.27)
project(NebulaGraphicsCore)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(NEBULA_GRAPHICS_CORE_SOURCES
    src/NebulaGraphicsCore/Core.hpp

    # Math
    src/NebulaGraphicsCore/Math.hpp # Eigen3 math typedefs

    # Geometry
    src/NebulaGraphicsCore/Geometry/Transform.hpp
    src/NebulaGraphicsCore/Geometry/Transform.cpp

    # Window System
    src/NebulaGraphicsCore/Window.hpp
    src/NebulaGraphicsCore/Event/KeyCodes.hpp
    src/NebulaGraphicsCore/Event/MouseCodes.hpp
    src/NebulaGraphicsCore/Event/Event.hpp
    src/NebulaGraphicsCore/Event/KeyEvent.hpp
    src/NebulaGraphicsCore/Event/MouseEvent.hpp
    src/NebulaGraphicsCore/Event/WindowEvent.hpp

    # Renderer
    src/NebulaGraphicsCore/Renderer.hpp
    src/NebulaGraphicsCore/Renderer.cpp
    src/NebulaGraphicsCore/RenderableObject.hpp
    src/NebulaGraphicsCore/Mesh/BaseMesh.hpp
    src/NebulaGraphicsCore/Mesh/2DStaticMesh.hpp
    src/NebulaGraphicsCore/Mesh/2DStaticMesh.cpp

    # Assets
    src/NebulaGraphicsCore/AssetLoader.hpp
)

add_subdirectory(vendor/Eigen)

set(NEBULA_GRAPHICS_CORE_LIBRARY_TYPE "Inherit" CACHE STRING "The type of library to build")
set_property(CACHE NEBULA_GRAPHICS_CORE_LIBRARY_TYPE PROPERTY STRINGS "Inherit" "Static" "Shared")
nebula_library_type(NEBULA_GRAPHICS_CORE_LIBRARY_TYPE NEBULA_GRAPHICS_CORE_LIBRARY_TYPE_FINAL)
message(STATUS "NebulaEngine: NebulaGraphicsCore built as: ${NEBULA_GRAPHICS_CORE_LIBRARY_TYPE_FINAL}")
add_library(NebulaGraphicsCore ${NEBULA_GRAPHICS_CORE_LIBRARY_TYPE_FINAL} ${NEBULA_GRAPHICS_CORE_SOURCES})
nebula_add_definitions(NebulaGraphicsCore "NEBULA_GRAPHICS_CORE" ${NEBULA_GRAPHICS_CORE_LIBRARY_TYPE_FINAL})
nebula_format_project(NebulaGraphicsCore ${CMAKE_CURRENT_SOURCE_DIR} "${NEBULA_GRAPHICS_CORE_SOURCES}")
target_compile_definitions(NebulaGraphicsCore PRIVATE "${NEBULA_CXX_FLAGS}")

target_include_directories(NebulaGraphicsCore PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_definitions(NebulaGraphicsCore PUBLIC
    NEBULA_INCLUDE_IMGUI
)

add_dependencies(NebulaGraphicsCore
    NebulaCore
    NebulaIO
    Eigen3::Eigen
)
target_link_libraries(NebulaGraphicsCore PUBLIC
    NebulaCore
    NebulaIO
    Eigen3::Eigen
)

add_subdirectory(vendor)
