project(NebulaEngine_Engine)

# =======================
# Logging Configuration
# =======================

# Log Levels:
# 0 - Trace
# 1 - Debug
# 2 - Info
# 3 - Warn
# 4 - Error
# 5 - Fatal
# 6 - Off

set(NEBULA_DEBUG_LOGLEVEL "0" CACHE STRING "The log level for debug messages")
set_property(CACHE NEBULA_DEBUG_LOGLEVEL PROPERTY STRINGS "0" "1" "2" "3" "4" "5" "6")
set(NEBULA_RELEASE_LOGLEVEL "2" CACHE STRING "The log level for release messages")
set_property(CACHE NEBULA_RELEASE_LOGLEVEL PROPERTY STRINGS "0" "1" "2" "3" "4" "5" "6")
set(NEBULA_DIST_LOGLEVEL "3" CACHE STRING "The log level for dist messages")
set_property(CACHE NEBULA_DIST_LOGLEVEL PROPERTY STRINGS "0" "1" "2" "3" "4" "5" "6")


# =======================
# Build Type Configuration
# =======================

set(NEBULA_BUILD_TYPE "Debug" CACHE STRING "The build type for NebulaEngine")
set_property(CACHE NEBULA_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "Dist")

if(NEBULA_BUILD_TYPE STREQUAL "Debug")
    set(NEBULA_LOGLEVEL ${NEBULA_DEBUG_LOGLEVEL})
    if (NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "The build type for NebulaEngine" FORCE)
    elseif(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
        message(WARNING "CMAKE_BUILD_TYPE is set to ${CMAKE_BUILD_TYPE}, but NEBULA_BUILD_TYPE is set to Debug. This may cause unexpected behavior.")
        set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "The build type for NebulaEngine" FORCE)
    endif()
elseif(NEBULA_BUILD_TYPE STREQUAL "Release")
    set(NEBULA_LOGLEVEL ${NEBULA_RELEASE_LOGLEVEL})
    if (NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE "Release" CACHE STRING "The build type for NebulaEngine" FORCE)
    elseif(NOT CMAKE_BUILD_TYPE STREQUAL "Release")
        message(WARNING "CMAKE_BUILD_TYPE is set to ${CMAKE_BUILD_TYPE}, but NEBULA_BUILD_TYPE is set to Release. This may cause unexpected behavior.")
        set(CMAKE_BUILD_TYPE "Release" CACHE STRING "The build type for NebulaEngine" FORCE)
    endif()
elseif(NEBULA_BUILD_TYPE STREQUAL "Dist")
    set(NEBULA_LOGLEVEL ${NEBULA_DIST_LOGLEVEL})
    if (NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE "Release" CACHE STRING "The build type for NebulaEngine" FORCE)
    elseif(NOT CMAKE_BUILD_TYPE STREQUAL "Release")
        message(WARNING "CMAKE_BUILD_TYPE is set to ${CMAKE_BUILD_TYPE}, but NEBULA_BUILD_TYPE is set to Dist. This may cause unexpected behavior (should be Release).")
        set(CMAKE_BUILD_TYPE "Release" CACHE STRING "The build type for NebulaEngine" FORCE)
    endif()
endif()

option(NEBULA_PEDANTIC_WARNINGS "Enable pedantic warnings" ON)
option(NEBULA_PROFILE "Enable profiling" OFF)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(vendor)

if (NEBULA_PLATFORM STREQUAL "Linux")
    set(NEBULA_RENDERER_OPENGL ON)
elseif (NEBULA_PLATFORM STREQUAL "MacOS")
    set(NEBULA_RENDERER_OPENGL OFF)
else()
    message(FATAL_ERROR "Unknown platform: ${NEBULA_PLATFORM}")
endif()


set(NEBULA_ENGINE_SOURCES
    # Core
    src/NebulaEngine/Core/PlatformDetection.hpp
    src/NebulaEngine/Core/Log.hpp
    src/NebulaEngine/Core/Log.cpp
    src/NebulaEngine/Core/Assert.hpp
    src/NebulaEngine/Core/Base.hpp
    src/NebulaEngine/Core/Result.hpp
    src/NebulaEngine/Core/Pointer.hpp
    src/NebulaEngine/Core/Bounds.hpp
    src/NebulaEngine/Core/Application.hpp
    src/NebulaEngine/Core/Application.cpp
    src/NebulaEngine/Core/Entrypoint.hpp
    src/NebulaEngine/Core/Window.hpp
    src/NebulaEngine/Core/Window.cpp
    src/NebulaEngine/Core/Utils/Function.hpp
    src/NebulaEngine/Core/Layer.hpp
    src/NebulaEngine/Core/Layer.cpp
    src/NebulaEngine/Core/LayerStack.hpp
    src/NebulaEngine/Core/LayerStack.cpp
    src/NebulaEngine/Core/Math.hpp
    src/NebulaEngine/Core/Time.hpp
    src/NebulaEngine/Core/Time.cpp

    # Renderer
    src/NebulaEngine/Renderer/RenderContext.hpp
    src/NebulaEngine/Renderer/RenderContext.cpp
    src/NebulaEngine/Renderer/RenderCommand.hpp
    src/NebulaEngine/Renderer/RenderCommand.cpp
    src/NebulaEngine/Renderer/RendererAPI.hpp
    src/NebulaEngine/Renderer/RendererAPI.cpp
    src/NebulaEngine/Renderer/Renderer.hpp
    src/NebulaEngine/Renderer/Renderer.cpp

    # UI
    src/NebulaEngine/UI/ImGui/ImGuiLayer.hpp
    src/NebulaEngine/UI/ImGui/ImGuiLayer.cpp

    # Platform
    $<$<STREQUAL:${NEBULA_PLATFORM},Linux>:
        src/Platform/Linux/LinuxWindow.hpp
        src/Platform/Linux/LinuxWindow.cpp
    >
    $<$<STREQUAL:${NEBULA_PLATFORM},MacOS>:
        src/Platform/MacOS/MacOSWindow.hpp
        src/Platform/MacOS/MacOSWindow.cpp
    >
    src/Platform/GLFW/GLFWWindowHelper.hpp
    src/Platform/GLFW/GLFWWindowHelper.cpp

    $<$<BOOL:${NEBULA_RENDERER_OPENGL}>:
        src/Platform/OpenGL/OpenGLContext.hpp
        src/Platform/OpenGL/OpenGLContext.cpp
        src/Platform/OpenGL/OpenGLRendererAPI.hpp
        src/Platform/OpenGL/OpenGLRendererAPI.cpp
        src/Platform/OpenGL/API/GL.hpp
        src/Platform/OpenGL/API/GL.cpp
        src/Platform/OpenGL/API/Enum.hpp
        src/Platform/OpenGL/API/Enum.cpp
    >
)

set(NEBULA_ENGINE_PCH_SOURCES
)


add_library(NebulaEngine_Engine STATIC
    ${NEBULA_ENGINE_SOURCES}
    ${NEBULA_ENGINE_PCH_SOURCES}
)

target_include_directories(NebulaEngine_Engine
PUBLIC
    src/
)

target_link_libraries(NebulaEngine_Engine PUBLIC
    spdlog::spdlog
    Microsoft.GSL::GSL
    glfw
    glad
    ImGui
    Eigen3::Eigen
    QProfile
    EnTT::EnTT
)

target_compile_definitions(NebulaEngine_Engine
PUBLIC
    SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_${NEBULA_LOGLEVEL}
    $<$<STREQUAL:${NEBULA_BUILD_TYPE},Debug>:NEBULA_DEBUG>
    $<$<STREQUAL:${NEBULA_BUILD_TYPE},Release>:NEBULA_RELEASE>
    $<$<STREQUAL:${NEBULA_BUILD_TYPE},Dist>:NEBULA_DIST>
    $<$<BOOL:${NEBULA_PROFILE}>:NEBULA_PROFILE>
    $<$<BOOL:${NEBULA_RENDERER_OPENGL}>:NEBULA_RENDERER_OPENGL>
)

# Module Definition
if (NEBULA_PEDANTIC_WARNINGS)
    if (NEBULA_COMPILER STREQUAL "CLANG" OR NEBULA_COMPILE STREQUAL "GCC")
        target_compile_options(NebulaEngine_Engine PRIVATE -Wall -Wextra -Wpedantic -Werror)
    elseif (MSVC)
        set(NebulaEngine_Engine /W4 /WX)
    else()
        message(WARNING "Unknown compiler, can't enable warnings")
    endif()
endif()

