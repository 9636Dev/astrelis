add_subdirectory(vendor/glfw)

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
)

set(NEBULA_RENDERER_PLATFORM_SOURCES
    $<$<PLATFORM_ID:Linux>:src/NebulaRenderer/Windowing/Linux/LinuxWindow.hpp>
    $<$<PLATFORM_ID:Linux>:src/NebulaRenderer/Windowing/Linux/LinuxWindow.cpp>
)

add_library(NebulaRenderer STATIC ${NEBULA_RENDERER_SOURCES} ${NEBULA_RENDERER_PLATFORM_SOURCES})

target_include_directories(NebulaRenderer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
add_dependencies(NebulaRenderer glfw NebulaCore)
target_link_libraries(NebulaRenderer
PUBLIC
    NebulaCore
PRIVATE
    glfw
)
