set(NEBULA_WINDOWING_SOURCES
    src/NebulaWindowing/Window.hpp
    src/NebulaWindowing/WindowHelper.hpp
    src/NebulaWindowing/WindowHelper.cpp

    src/NebulaWindowing/Event/Event.hpp
    src/NebulaWindowing/Event/KeyEvent.hpp
    src/NebulaWindowing/Event/MouseEvent.hpp
    src/NebulaWindowing/Event/WindowEvent.hpp
    src/NebulaWindowing/Event/KeyCodes.hpp
    src/NebulaWindowing/Event/MouseCodes.hpp
)

set(NEBULA_WINDOWING_PLATFORM_SOURCES
    # Linux
    $<$<PLATFORM_ID:Linux>:src/NebulaWindowing/Linux/LinuxWindow.hpp>
    $<$<PLATFORM_ID:Linux>:src/NebulaWindowing/Linux/LinuxWindow.cpp>

    # Windows
    $<$<PLATFORM_ID:Windows>:src/NebulaWindowing/Windows/WindowsWindow.hpp>
    $<$<PLATFORM_ID:Windows>:src/NebulaWindowing/Windows/WindowsWindow.cpp>

    # MacOS
    $<$<PLATFORM_ID:Darwin>:src/NebulaWindowing/MacOS/MacOSWindow.hpp>
    $<$<PLATFORM_ID:Darwin>:src/NebulaWindowing/MacOS/MacOSWindow.cpp>
)

add_subdirectory(vendor/glfw)

add_library(NebulaWindowing STATIC
    ${NEBULA_WINDOWING_SOURCES}
    ${NEBULA_WINDOWING_PLATFORM_SOURCES}
)
nebula_target(NebulaWindowing)
nebula_format_project(NebulaWindowing ${CMAKE_CURRENT_SOURCE_DIR} "${NEBULA_WINDOWING_SOURCES};${NEBULA_WINDOWING_PLATFORM_SOURCES}")

target_include_directories(NebulaWindowing PUBLIC
    src
)

target_link_libraries(NebulaWindowing
    NebulaCore
    glfw
)
