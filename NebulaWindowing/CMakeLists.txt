set(NEBULA_WINDOWING_SOURCES
    src/NebulaWindowing/Window.hpp
)

set(NEBULA_WINDOWING_PLATFORM_SOURCES
    $<$<PLATFORM_ID:Linux>:src/NebulaWindowing/Linux/LinuxWindow.hpp>
    $<$<PLATFORM_ID:Linux>:src/NebulaWindowing/Linux/LinuxWindow.cpp>
    $<$<PLATFORM_ID:Windows>:src/NebulaWindowing/Windows/WindowsWindow.hpp>
    $<$<PLATFORM_ID:Windows>:src/NebulaWindowing/Windows/WindowsWindow.cpp>
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
