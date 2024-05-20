set(NEBULA_WINDOWING_SOURCES
    src/NebulaWindowing/Window.hpp
    src/NebulaWindowing/WindowHelper.hpp
    src/NebulaWindowing/WindowHelper.cpp
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

target_include_directories(NebulaWindowing PUBLIC
    src
)

target_link_libraries(NebulaWindowing
    NebulaCore
    glfw
)
