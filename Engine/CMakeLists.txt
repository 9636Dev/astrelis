project(Astrelis_Engine)

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

set(ASTRELIS_DEBUG_LOGLEVEL "0" CACHE STRING "The log level for debug messages")
set_property(CACHE ASTRELIS_DEBUG_LOGLEVEL PROPERTY STRINGS "0" "1" "2" "3" "4" "5" "6")
set(ASTRELIS_RELEASE_LOGLEVEL "2" CACHE STRING "The log level for release messages")
set_property(CACHE ASTRELIS_RELEASE_LOGLEVEL PROPERTY STRINGS "0" "1" "2" "3" "4" "5" "6")
set(ASTRELIS_DIST_LOGLEVEL "3" CACHE STRING "The log level for dist messages")
set_property(CACHE ASTRELIS_DIST_LOGLEVEL PROPERTY STRINGS "0" "1" "2" "3" "4" "5" "6")


# =======================
# Build Type Configuration
# =======================

set(ASTRELIS_BUILD_TYPE "Debug" CACHE STRING "The build type for src/Astrelis")
set_property(CACHE ASTRELIS_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "Dist")

if(ASTRELIS_BUILD_TYPE STREQUAL "Debug")
    set(ASTRELIS_LOGLEVEL ${ASTRELIS_DEBUG_LOGLEVEL})
    if (NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "The build type for src/Astrelis" FORCE)
    elseif(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
        message(WARNING "CMAKE_BUILD_TYPE is set to ${CMAKE_BUILD_TYPE}, but ASTRELIS_BUILD_TYPE is set to Debug. This may cause unexpected behavior.")
        set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "The build type for src/Astrelis" FORCE)
    endif()
elseif(ASTRELIS_BUILD_TYPE STREQUAL "Release")
    set(ASTRELIS_LOGLEVEL ${ASTRELIS_RELEASE_LOGLEVEL})
    if (NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE "Release" CACHE STRING "The build type for src/Astrelis" FORCE)
    elseif(NOT CMAKE_BUILD_TYPE STREQUAL "Release")
        message(WARNING "CMAKE_BUILD_TYPE is set to ${CMAKE_BUILD_TYPE}, but ASTRELIS_BUILD_TYPE is set to Release. This may cause unexpected behavior.")
        set(CMAKE_BUILD_TYPE "Release" CACHE STRING "The build type for src/Astrelis" FORCE)
    endif()
elseif(ASTRELIS_BUILD_TYPE STREQUAL "Dist")
    set(ASTRELIS_LOGLEVEL ${ASTRELIS_DIST_LOGLEVEL})
    if (NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE "Release" CACHE STRING "The build type for src/Astrelis" FORCE)
    elseif(NOT CMAKE_BUILD_TYPE STREQUAL "Release")
        message(WARNING "CMAKE_BUILD_TYPE is set to ${CMAKE_BUILD_TYPE}, but ASTRELIS_BUILD_TYPE is set to Dist. This may cause unexpected behavior (should be Release).")
        set(CMAKE_BUILD_TYPE "Release" CACHE STRING "The build type for src/Astrelis" FORCE)
    endif()
endif()

# O2 for Release, O3 for Dist
if (ASTRELIS_BUILD_TYPE STREQUAL "Release")
    if (ASTRELIS_COMPILER STREQUAL "CLANG" OR ASTRELIS_COMPILER STREQUAL "GCC")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
    else()
        message(WARNING "Unknown compiler, can't set optimization level")
    endif()
elseif (ASTRELIS_BUILD_TYPE STREQUAL "Dist")
    if (ASTRELIS_COMPILER STREQUAL "CLANG" OR ASTRELIS_COMPILER STREQUAL "GCC")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
    else()
        message(WARNING "Unknown compiler, can't set optimization level")
    endif()
endif()

option(ASTRELIS_PEDANTIC_WARNINGS "Enable pedantic warnings" ON)
option(ASTRELIS_PROFILE "Enable profiling" OFF)
option(ASTRELIS_PROFILE_MEMORY "Enable memory profiling" OFF)
option(ASTRELIS_PROFILE_GPU "Enable GPU profiling" OFF)

if (NOT ASTRELIS_PROFILE)
    message(NOTICE "Profiling is disabled")
    set(ASTRELIS_PROFILE_MEMORY OFF)
    set(ASTRELIS_PROFILE_GPU OFF)
endif()

# Feature Options
if (ASTRELIS_FEATURES_ALL)
    # This way the options can be disabled individually after enabling all
    option(ASTRELIS_FEATURE_FRAMEBUFFER "Enable framebuffer support" ON)
    option(ASTRELIS_FEATURES_ALL "Enable all features" ON)
else()
    option(ASTRELIS_FEATURE_FRAMEBUFFER "Enable framebuffer support" OFF)
    option(ASTRELIS_FEATURES_ALL "Enable all features" OFF)

endif()

# Print the configuration
message(STATUS "Astrelis Configuration:")
message(STATUS "  Build Type: ${ASTRELIS_BUILD_TYPE}")
message(STATUS "  Log Level: ${ASTRELIS_LOGLEVEL}")
message(STATUS "  Pedantic Warnings: ${ASTRELIS_PEDANTIC_WARNINGS}")
message(STATUS "  Profiling: ${ASTRELIS_PROFILE}")
message(STATUS "  Memory Profiling: ${ASTRELIS_PROFILE_MEMORY}")
message(STATUS "  GPU Profiling: ${ASTRELIS_PROFILE_GPU}")
message(STATUS "  Features:")
message(STATUS "    Framebuffer: ${ASTRELIS_FEATURE_FRAMEBUFFER}")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


if (ASTRELIS_PLATFORM STREQUAL "Linux")
    set(ASTRELIS_RENDERER_OPENGL OFF)
    set(ASTRELIS_RENDERER_METAL OFF)
    set(ASTRELIS_RENDERER_VULKAN ON)
elseif (ASTRELIS_PLATFORM STREQUAL "MacOS")
    set(ASTRELIS_RENDERER_OPENGL OFF)
    set(ASTRELIS_RENDERER_METAL ON)
    set(ASTRELIS_RENDERER_VULKAN ON)
elseif (ASTRELIS_PLATFORM STREQUAL "Windows")
    set(ASTRELIS_RENDERER_OPENGL OFF)
    set(ASTRELIS_RENDERER_METAL OFF)
    set(ASTRELIS_RENDERER_VULKAN ON)
else()
    message(FATAL_ERROR "Unknown platform: ${ASTRELIS_PLATFORM}")
endif()

if (ASTRELIS_RENDERER_VULKAN)
    find_package(Vulkan REQUIRED)
endif()


add_subdirectory(vendor)

set(ASTRELIS_ENGINE_SOURCES
    src/Astrelis/Astrelis.hpp
    # Core
    src/Astrelis/Core/Application.cpp
    src/Astrelis/Core/Application.hpp
    src/Astrelis/Core/Base.hpp
    src/Astrelis/Core/Entrypoint.hpp
    src/Astrelis/Core/Geometry.hpp
    src/Astrelis/Core/GlobalConfig.cpp
    src/Astrelis/Core/GlobalConfig.hpp
    src/Astrelis/Core/Layer.cpp
    src/Astrelis/Core/Layer.hpp
    src/Astrelis/Core/LayerStack.cpp
    src/Astrelis/Core/LayerStack.hpp
    src/Astrelis/Core/Log.cpp
    src/Astrelis/Core/Log.hpp
    src/Astrelis/Core/Math.hpp
    src/Astrelis/Core/Pointer.hpp
    src/Astrelis/Core/Result.hpp
    src/Astrelis/Core/Time.cpp
    src/Astrelis/Core/Time.hpp
    src/Astrelis/Core/Types.hpp
    src/Astrelis/Core/Window.cpp
    src/Astrelis/Core/Window.hpp
    src/Astrelis/Core/Utils/Assert.hpp
    src/Astrelis/Core/Utils/Debug.hpp
    src/Astrelis/Core/Utils/Function.hpp
    src/Astrelis/Core/Utils/Platform.hpp
    src/Astrelis/Core/Utils/PlatformDetection.hpp
    src/Astrelis/Core/Utils/Profiling.cpp
    src/Astrelis/Core/Utils/Profiling.hpp
    src/Astrelis/Core/Utils/Renderer.hpp
    src/Astrelis/Core/Utils/Version.hpp

    # Events
    src/Astrelis/Events/Event.hpp
    src/Astrelis/Events/KeyEvent.hpp
    src/Astrelis/Events/MouseEvent.hpp
    src/Astrelis/Events/WindowEvent.hpp

    # IO
    src/Astrelis/IO/File.cpp
    src/Astrelis/IO/File.hpp
    src/Astrelis/IO/Image.cpp
    src/Astrelis/IO/Image.hpp

    # Renderer
    src/Astrelis/Renderer/BaseRenderer.cpp
    src/Astrelis/Renderer/BaseRenderer.hpp
    src/Astrelis/Renderer/Camera.hpp
    src/Astrelis/Renderer/DescriptorSetLayout.hpp
    src/Astrelis/Renderer/DescriptorSets.hpp
    src/Astrelis/Renderer/GraphicsContext.cpp
    src/Astrelis/Renderer/GraphicsContext.hpp
    src/Astrelis/Renderer/GraphicsPipeline.hpp
    src/Astrelis/Renderer/IndexBuffer.hpp
    src/Astrelis/Renderer/RenderSystem.cpp
    src/Astrelis/Renderer/RenderSystem.hpp
    src/Astrelis/Renderer/RendererAPI.cpp
    src/Astrelis/Renderer/RendererAPI.hpp
    src/Astrelis/Renderer/TextureImage.hpp
    src/Astrelis/Renderer/TextureSampler.hpp
    src/Astrelis/Renderer/UniformBuffer.hpp
    src/Astrelis/Renderer/Vertex.hpp
    src/Astrelis/Renderer/VertexBuffer.hpp

    src/Astrelis/Renderer/Renderer2D.cpp
    src/Astrelis/Renderer/Renderer2D.hpp

    src/Astrelis/Renderer/TilemapRenderer.cpp
    src/Astrelis/Renderer/TilemapRenderer.hpp

    # Scene
    src/Astrelis/Scene/Material.hpp

    # UI
    src/Astrelis/UI/ImGui/ImGuiBackend.cpp
    src/Astrelis/UI/ImGui/ImGuiBackend.hpp
    src/Astrelis/UI/ImGui/ImGuiLayer.cpp
    src/Astrelis/UI/ImGui/ImGuiLayer.hpp

    # src/Platform
    # # GLFW
    src/Platform/GLFW/GLFWWindowHelper.hpp
    src/Platform/GLFW/GLFWWindowHelper.cpp

    # # Linux
    $<$<STREQUAL:${ASTRELIS_PLATFORM},Linux>:
        src/Platform/Linux/LinuxWindow.hpp
        src/Platform/Linux/LinuxWindow.cpp
    >

    # # MacOS
    $<$<STREQUAL:${ASTRELIS_PLATFORM},MacOS>:
        src/Platform/MacOS/MacOSWindow.hpp
        src/Platform/MacOS/MacOSWindow.cpp
    >

    # # Windows
    $<$<STREQUAL:${ASTRELIS_PLATFORM},Windows>:
        src/Platform/Windows/WindowsWindow.hpp
        src/Platform/Windows/WindowsWindow.cpp
    >

    # # OpenGL
    $<$<BOOL:${ASTRELIS_RENDERER_OPENGL}>:
    >

    # # Metal
    $<$<BOOL:${ASTRELIS_RENDERER_METAL}>:
        src/Platform/Metal/MetalGraphicsContext.hpp
        src/Platform/Metal/MetalGraphicsContext.mm

    >

    # # Vulkan
    $<$<BOOL:${ASTRELIS_RENDERER_VULKAN}>:
        src/Platform/Vulkan/Vulkan2DRendererAPI.cpp
        src/Platform/Vulkan/Vulkan2DRendererAPI.hpp
        src/Platform/Vulkan/VulkanGraphicsContext.cpp
        src/Platform/Vulkan/VulkanGraphicsContext.hpp
        src/Platform/Vulkan/VulkanImGuiBackend.cpp
        src/Platform/Vulkan/VulkanImGuiBackend.hpp
        src/Platform/Vulkan/VulkanRenderSystem.cpp
        src/Platform/Vulkan/VulkanRenderSystem.hpp
        src/Platform/Vulkan/VulkanRendererHelper.cpp
        src/Platform/Vulkan/VulkanRendererHelper.hpp
        src/Platform/Vulkan/VK/CommandBuffer.cpp
        src/Platform/Vulkan/VK/CommandBuffer.hpp
        src/Platform/Vulkan/VK/CommandPool.cpp
        src/Platform/Vulkan/VK/CommandPool.hpp
        src/Platform/Vulkan/VK/DebugMessenger.cpp
        src/Platform/Vulkan/VK/DebugMessenger.hpp
        src/Platform/Vulkan/VK/DescriptorPool.cpp
        src/Platform/Vulkan/VK/DescriptorPool.hpp
        src/Platform/Vulkan/VK/DescriptorSetLayout.cpp
        src/Platform/Vulkan/VK/DescriptorSetLayout.hpp
        src/Platform/Vulkan/VK/DescriptorSets.cpp
        src/Platform/Vulkan/VK/DescriptorSets.hpp
        src/Platform/Vulkan/VK/Fence.cpp
        src/Platform/Vulkan/VK/Fence.hpp
        src/Platform/Vulkan/VK/FrameBuffer.cpp
        src/Platform/Vulkan/VK/FrameBuffer.hpp
        src/Platform/Vulkan/VK/GraphicsPipeline.cpp
        src/Platform/Vulkan/VK/GraphicsPipeline.hpp
        src/Platform/Vulkan/VK/ImageView.cpp
        src/Platform/Vulkan/VK/ImageView.hpp
        src/Platform/Vulkan/VK/IndexBuffer.cpp
        src/Platform/Vulkan/VK/IndexBuffer.hpp
        src/Platform/Vulkan/VK/Instance.cpp
        src/Platform/Vulkan/VK/Instance.hpp
        src/Platform/Vulkan/VK/LogicalDevice.cpp
        src/Platform/Vulkan/VK/LogicalDevice.hpp
        src/Platform/Vulkan/VK/PhysicalDevice.cpp
        src/Platform/Vulkan/VK/PhysicalDevice.hpp
        src/Platform/Vulkan/VK/RenderPass.cpp
        src/Platform/Vulkan/VK/RenderPass.hpp
        src/Platform/Vulkan/VK/Semaphore.cpp
        src/Platform/Vulkan/VK/Semaphore.hpp
        src/Platform/Vulkan/VK/Surface.cpp
        src/Platform/Vulkan/VK/Surface.hpp
        src/Platform/Vulkan/VK/SwapChain.cpp
        src/Platform/Vulkan/VK/SwapChain.hpp
        src/Platform/Vulkan/VK/TextureImage.cpp
        src/Platform/Vulkan/VK/TextureImage.hpp
        src/Platform/Vulkan/VK/TextureSampler.cpp
        src/Platform/Vulkan/VK/TextureSampler.hpp
        src/Platform/Vulkan/VK/UniformBuffer.cpp
        src/Platform/Vulkan/VK/UniformBuffer.hpp
        src/Platform/Vulkan/VK/Utils.cpp
        src/Platform/Vulkan/VK/Utils.hpp
        src/Platform/Vulkan/VK/VertexBuffer.cpp
        src/Platform/Vulkan/VK/VertexBuffer.hpp
        src/Platform/Vulkan/VK/VulkanExt.cpp
        src/Platform/Vulkan/VK/VulkanExt.hpp
    >

)

set(ASTRELIS_ENGINE_PCH_SOURCES
)


add_library(Astrelis_Engine STATIC
    ${ASTRELIS_ENGINE_SOURCES}
    ${ASTRELIS_ENGINE_PCH_SOURCES}
)

add_library(Astrelis_External STATIC
    src/External/StbImpl.cpp
)
target_link_libraries(Astrelis_External PUBLIC stb)

target_include_directories(Astrelis_Engine
PUBLIC
    src/
    vendor/tracy/Public
)

# As long as we are not using objective-c++ (Metal Renderer), we can use precompiled headers
if (NOT ASTRELIS_RENDERER_METAL)
    target_precompile_headers(Astrelis_Engine PRIVATE
        src/Astrelis/Core/Base.hpp
    )
endif()

target_link_libraries(Astrelis_Engine PUBLIC
    Astrelis_External
    spdlog::spdlog
    glfw
    $<$<BOOL:${ASTRELIS_RENDERER_OPENGL}>:
        glad
    >
    $<$<BOOL:${ASTRELIS_RENDERER_METAL}>:
        "-framework Metal"
        "-framework Cocoa"
        "-framework QuartzCore"
    >
    $<$<BOOL:${ASTRELIS_RENDERER_VULKAN}>:
        Vulkan::Vulkan
    >
    glm
    ImGui
    EnTT::EnTT
    stb
    TracyClient
    Cereal
)

target_compile_definitions(Astrelis_Engine
PUBLIC
    SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_${ASTRELIS_LOGLEVEL}
    $<$<STREQUAL:${ASTRELIS_BUILD_TYPE},Debug>:ASTRELIS_DEBUG>
    $<$<STREQUAL:${ASTRELIS_BUILD_TYPE},Release>:ASTRELIS_RELEASE>
    $<$<STREQUAL:${ASTRELIS_BUILD_TYPE},Dist>:ASTRELIS_DIST>

    # Profiling
    $<$<BOOL:${ASTRELIS_PROFILE}>:ASTRELIS_PROFILE>
    $<$<BOOL:${ASTRELIS_PROFILE_MEMORY}>:ASTRELIS_PROFILE_MEMORY>
    $<$<BOOL:${ASTRELIS_PROFILE_GPU}>:ASTRELIS_PROFILE_GPU>

    # Renderer
    $<$<BOOL:${ASTRELIS_RENDERER_OPENGL}>:ASTRELIS_RENDERER_OPENGL>
    $<$<BOOL:${ASTRELIS_RENDERER_METAL}>:ASTRELIS_RENDERER_METAL>
    $<$<BOOL:${ASTRELIS_RENDERER_VULKAN}>:ASTRELIS_RENDERER_VULKAN>

    # Features
    $<$<BOOL:${ASTRELIS_FEATURE_FRAMEBUFFER}>:ASTRELIS_FEATURE_FRAMEBUFFER>
)

# Module Definition
if (ASTRELIS_PEDANTIC_WARNINGS)
    if (ASTRELIS_COMPILER STREQUAL "CLANG" OR ASTRELIS_COMPILE STREQUAL "GCC")
        target_compile_options(Astrelis_Engine PRIVATE -Wall -Wextra -Wpedantic -Werror)
    elseif (MSVC)
        set(Astrelis_Engine /W4 /WX)
    else()
        message(WARNING "Unknown compiler, can't enable warnings")
    endif()
endif()

if (ASTRELIS_FORMAT)
    file(GLOB_RECURSE ASTRELIS_ENGINE_FORMAT_SOURCES
        src/*.hpp
        src/*.cpp
    )
    add_custom_target(Astrelis_Format
        COMMAND clang-format -style=file -i ${ASTRELIS_ENGINE_FORMAT_SOURCES}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
    add_dependencies(Astrelis_Engine Astrelis_Format)
endif()

add_subdirectory(tests)
add_subdirectory(benchmarks)
