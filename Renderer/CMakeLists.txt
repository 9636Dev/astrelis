add_subdirectory(vendor/glfw)
add_subdirectory(vendor) # ImGUI

set(NEBULA_RENDERER_SOURCES
    # Windowing System
    src/NebulaRenderer/Windowing/Window.hpp
    src/NebulaRenderer/Windowing/WindowProps.hpp
    src/NebulaRenderer/Windowing/WindowHelper.hpp
    src/NebulaRenderer/Windowing/WindowHelper.cpp
    src/NebulaRenderer/Windowing/Event/Event.hpp
    src/NebulaRenderer/Windowing/Event/KeyEvent.hpp
    src/NebulaRenderer/Windowing/Event/MouseEvent.hpp
    src/NebulaRenderer/Windowing/Event/WindowEvent.hpp

    # Renderer
    src/NebulaRenderer/Renderer.hpp

    src/NebulaRenderer/OpenGL/GLRenderer.hpp
    src/NebulaRenderer/OpenGL/GLRenderer.cpp
    src/NebulaRenderer/OpenGL/OpenGL/Enum.hpp
    src/NebulaRenderer/OpenGL/OpenGL/Enum.cpp
    src/NebulaRenderer/OpenGL/OpenGL/GL.hpp
    src/NebulaRenderer/OpenGL/OpenGL/GL.cpp

    # UI
    src/NebulaRenderer/UI/UI.hpp
    src/NebulaRenderer/UI/UI.cpp
)

set(NEBULA_RENDERER_PLATFORM_SOURCES
    $<$<PLATFORM_ID:Linux>:src/NebulaRenderer/Windowing/Linux/LinuxWindow.hpp>
    $<$<PLATFORM_ID:Linux>:src/NebulaRenderer/Windowing/Linux/LinuxWindow.cpp>
)

# Find GLEW and OpenGL
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

add_library(NebulaRenderer STATIC ${NEBULA_RENDERER_SOURCES} ${NEBULA_RENDERER_PLATFORM_SOURCES})
nebula_target(NebulaRenderer)
nebula_format_project(NebulaRenderer ${CMAKE_CURRENT_SOURCE_DIR} "${NEBULA_RENDERER_SOURCES};${NEBULA_RENDERER_PLATFORM_SOURCES}")

target_include_directories(NebulaRenderer PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)
add_dependencies(NebulaRenderer glfw NebulaCore)
target_link_libraries(NebulaRenderer
PUBLIC
    NebulaCore
PRIVATE
    glfw
    GLEW::GLEW
    OpenGL::GL
    OpenGL::GLU
)
ImGuiAddLibrary(NebulaRenderer PUBLIC "GLFWOpenGL3")
