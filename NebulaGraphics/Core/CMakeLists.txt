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
)

add_subdirectory(vendor/Eigen)

add_library(NebulaGraphicsCore SHARED ${NEBULA_GRAPHICS_CORE_SOURCES})
add_dependencies(NebulaGraphicsCore NebulaCore)
nebula_add_definitions(NebulaGraphicsCore "NEBULA_GRAPHICS_CORE" SHARED)

target_include_directories(NebulaGraphicsCore PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

add_dependencies(NebulaGraphicsCore
    NebulaCore
    Eigen3::Eigen
    NebulaShaderConductor
)
target_link_libraries(NebulaGraphicsCore PUBLIC
    NebulaCore
    Eigen3::Eigen
    NebulaShaderConductor
)
