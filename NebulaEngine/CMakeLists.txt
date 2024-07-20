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


if (NEBULA_PLATFORM STREQUAL "Linux")
    set(NEBULA_RENDERER_OPENGL OFF)
    set(NEBULA_RENDERER_METAL OFF)
    set(NEBULA_RENDERER_VULKAN ON)
elseif (NEBULA_PLATFORM STREQUAL "MacOS")
    # TODO: Add Metal renderer, and disable OpenGL
    set(NEBULA_RENDERER_OPENGL OFF)
    set(NEBULA_RENDERER_METAL OFF)
    set(NEBULA_RENDERER_VULKAN ON)
elseif (NEBULA_PLATFORM STREQUAL "Windows")
    set(NEBULA_RENDERER_OPENGL OFF)
    set(NEBULA_RENDERER_METAL OFF)
    set(NEBULA_RENDERER_VULKAN ON)
else()
    message(FATAL_ERROR "Unknown platform: ${NEBULA_PLATFORM}")
endif()

if (NEBULA_RENDERER_VULKAN)
    find_package(Vulkan REQUIRED)
endif()


add_subdirectory(vendor)


set(NEBULA_ENGINE_SOURCES
    # Core
    src/NebulaEngine/Core/Application.cpp
    src/NebulaEngine/Core/Application.hpp
    src/NebulaEngine/Core/Assert.hpp
    src/NebulaEngine/Core/Base.hpp
    src/NebulaEngine/Core/Entrypoint.hpp
    src/NebulaEngine/Core/Geometry.hpp
    src/NebulaEngine/Core/GlobalConfig.cpp
    src/NebulaEngine/Core/GlobalConfig.hpp
    src/NebulaEngine/Core/Layer.cpp
    src/NebulaEngine/Core/Layer.hpp
    src/NebulaEngine/Core/LayerStack.cpp
    src/NebulaEngine/Core/LayerStack.hpp
    src/NebulaEngine/Core/Log.cpp
    src/NebulaEngine/Core/Log.hpp
    src/NebulaEngine/Core/PlatformDetection.hpp
    src/NebulaEngine/Core/Pointer.hpp
    src/NebulaEngine/Core/Profiler.cpp
    src/NebulaEngine/Core/Profiler.hpp
    src/NebulaEngine/Core/Result.hpp
    src/NebulaEngine/Core/Time.cpp
    src/NebulaEngine/Core/Time.hpp
    src/NebulaEngine/Core/Types.hpp
    src/NebulaEngine/Core/Utils/Defer.hpp
    src/NebulaEngine/Core/Utils/Function.hpp
    src/NebulaEngine/Core/Window.cpp
    src/NebulaEngine/Core/Window.hpp

    # IO
    src/NebulaEngine/IO/File.hpp
    src/NebulaEngine/IO/File.cpp
    src/NebulaEngine/IO/Image.hpp
    src/NebulaEngine/IO/Image.cpp

    # Renderer
    src/NebulaEngine/Renderer/BaseRenderer.cpp
    src/NebulaEngine/Renderer/BaseRenderer.hpp
    src/NebulaEngine/Renderer/GraphicsContext.cpp
    src/NebulaEngine/Renderer/GraphicsContext.hpp
    src/NebulaEngine/Renderer/RenderSystem.cpp
    src/NebulaEngine/Renderer/RenderSystem.hpp
    src/NebulaEngine/Renderer/Renderer2D.cpp
    src/NebulaEngine/Renderer/Renderer2D.hpp
    src/NebulaEngine/Renderer/RendererAPI.cpp
    src/NebulaEngine/Renderer/RendererAPI.hpp
    src/NebulaEngine/Renderer/VoxelRenderer.cpp
    src/NebulaEngine/Renderer/VoxelRenderer.hpp

    src/NebulaEngine/Renderer/DescriptorSetLayout.hpp
    src/NebulaEngine/Renderer/DescriptorSets.hpp
    src/NebulaEngine/Renderer/GraphicsPipeline.hpp
    src/NebulaEngine/Renderer/IndexBuffer.hpp
    src/NebulaEngine/Renderer/TextureImage.hpp
    src/NebulaEngine/Renderer/TextureSampler.hpp
    src/NebulaEngine/Renderer/UniformBuffer.hpp
    src/NebulaEngine/Renderer/VertexBuffer.hpp

    # UI
    src/NebulaEngine/UI/ImGui/ImGuiBackend.hpp
    src/NebulaEngine/UI/ImGui/ImGuiBackend.cpp
    src/NebulaEngine/UI/ImGui/ImGuiLayer.hpp
    src/NebulaEngine/UI/ImGui/ImGuiLayer.cpp

    # Scene
    src/NebulaEngine/Scene/Scene.hpp
    src/NebulaEngine/Scene/Scene.cpp
    src/NebulaEngine/Scene/Scene2D.hpp
    src/NebulaEngine/Scene/Scene2D.cpp

    # Platform
    $<$<STREQUAL:${NEBULA_PLATFORM},Linux>:
        src/Platform/Linux/LinuxWindow.hpp
        src/Platform/Linux/LinuxWindow.cpp
    >
    $<$<STREQUAL:${NEBULA_PLATFORM},MacOS>:
        src/Platform/MacOS/MacOSWindow.hpp
        src/Platform/MacOS/MacOSWindow.cpp
    >
    $<$<STREQUAL:${NEBULA_PLATFORM},Windows>:
        src/Platform/Windows/WindowsWindow.hpp
        src/Platform/Windows/WindowsWindow.cpp
    >
    src/Platform/GLFW/GLFWWindowHelper.hpp
    src/Platform/GLFW/GLFWWindowHelper.cpp

    $<$<BOOL:${NEBULA_RENDERER_OPENGL}>:
    >

    $<$<BOOL:${NEBULA_RENDERER_METAL}>:
    >

    $<$<BOOL:${NEBULA_RENDERER_VULKAN}>:
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

        src/Platform/Vulkan/VK/Utils.hpp
        src/Platform/Vulkan/VK/Utils.cpp
        src/Platform/Vulkan/VK/VulkanExt.hpp
        src/Platform/Vulkan/VK/VulkanExt.cpp
        src/Platform/Vulkan/VK/DebugMessenger.hpp
        src/Platform/Vulkan/VK/DebugMessenger.cpp
        src/Platform/Vulkan/VK/Instance.hpp
        src/Platform/Vulkan/VK/Instance.cpp
        src/Platform/Vulkan/VK/Surface.hpp
        src/Platform/Vulkan/VK/Surface.cpp
        src/Platform/Vulkan/VK/PhysicalDevice.hpp
        src/Platform/Vulkan/VK/PhysicalDevice.cpp
        src/Platform/Vulkan/VK/LogicalDevice.hpp
        src/Platform/Vulkan/VK/LogicalDevice.cpp
        src/Platform/Vulkan/VK/SwapChain.hpp
        src/Platform/Vulkan/VK/SwapChain.cpp
        src/Platform/Vulkan/VK/CommandBuffer.hpp
        src/Platform/Vulkan/VK/CommandBuffer.cpp
        src/Platform/Vulkan/VK/CommandPool.hpp
        src/Platform/Vulkan/VK/CommandPool.cpp
        src/Platform/Vulkan/VK/ImageView.hpp
        src/Platform/Vulkan/VK/ImageView.cpp
        src/Platform/Vulkan/VK/RenderPass.hpp
        src/Platform/Vulkan/VK/RenderPass.cpp
        src/Platform/Vulkan/VK/GraphicsPipeline.hpp
        src/Platform/Vulkan/VK/GraphicsPipeline.cpp
        src/Platform/Vulkan/VK/FrameBuffer.hpp
        src/Platform/Vulkan/VK/FrameBuffer.cpp
        src/Platform/Vulkan/VK/Semaphore.hpp
        src/Platform/Vulkan/VK/Semaphore.cpp
        src/Platform/Vulkan/VK/Fence.hpp
        src/Platform/Vulkan/VK/Fence.cpp
        src/Platform/Vulkan/VK/VertexBuffer.hpp
        src/Platform/Vulkan/VK/VertexBuffer.cpp
        src/Platform/Vulkan/VK/IndexBuffer.hpp
        src/Platform/Vulkan/VK/IndexBuffer.cpp
        src/Platform/Vulkan/VK/DescriptorSetLayout.hpp
        src/Platform/Vulkan/VK/DescriptorSetLayout.cpp
        src/Platform/Vulkan/VK/UniformBuffer.hpp
        src/Platform/Vulkan/VK/UniformBuffer.cpp
        src/Platform/Vulkan/VK/DescriptorPool.hpp
        src/Platform/Vulkan/VK/DescriptorPool.cpp
        src/Platform/Vulkan/VK/DescriptorSets.hpp
        src/Platform/Vulkan/VK/DescriptorSets.cpp
        src/Platform/Vulkan/VK/TextureImage.hpp
        src/Platform/Vulkan/VK/TextureImage.cpp
        src/Platform/Vulkan/VK/TextureSampler.hpp
        src/Platform/Vulkan/VK/TextureSampler.cpp
    >
)

set(NEBULA_ENGINE_PCH_SOURCES
)


add_library(NebulaEngine_Engine STATIC
    ${NEBULA_ENGINE_SOURCES}
    ${NEBULA_ENGINE_PCH_SOURCES}
)

add_library(NebulaEngine_External STATIC
    src/External/StbImpl.cpp
)
target_link_libraries(NebulaEngine_External PUBLIC stb)

target_include_directories(NebulaEngine_Engine
PUBLIC
    src/
)

target_link_libraries(NebulaEngine_Engine PUBLIC
    NebulaEngine_External
    spdlog::spdlog
    glfw
    $<$<BOOL:${NEBULA_RENDERER_OPENGL}>:
        glad
    >
    $<$<BOOL:${NEBULA_RENDERER_METAL}>:
        "-framework Metal"
        "-framework Cocoa"
        "-framework QuartzCore"
    >
    $<$<BOOL:${NEBULA_RENDERER_VULKAN}>:
        Vulkan::Vulkan
    >
    glm
    ImGui
    QProfile
    EnTT::EnTT
    stb
)

target_compile_definitions(NebulaEngine_Engine
PUBLIC
    SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_${NEBULA_LOGLEVEL}
    $<$<STREQUAL:${NEBULA_BUILD_TYPE},Debug>:NEBULA_DEBUG>
    $<$<STREQUAL:${NEBULA_BUILD_TYPE},Release>:NEBULA_RELEASE>
    $<$<STREQUAL:${NEBULA_BUILD_TYPE},Dist>:NEBULA_DIST>
    $<$<BOOL:${NEBULA_PROFILE}>:NEBULA_PROFILE>
    $<$<BOOL:${NEBULA_RENDERER_OPENGL}>:NEBULA_RENDERER_OPENGL>
    $<$<BOOL:${NEBULA_RENDERER_METAL}>:NEBULA_RENDERER_METAL>
    $<$<BOOL:${NEBULA_RENDERER_VULKAN}>:NEBULA_RENDERER_VULKAN>
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

add_custom_target(NebulaEngine_Shaders
    COMMAND glslc ${CMAKE_SOURCE_DIR}/run/resources/shaders/Basic.vert -o ${CMAKE_SOURCE_DIR}/run/resources/shaders/Basic_vert.spv
    COMMAND glslc ${CMAKE_SOURCE_DIR}/run/resources/shaders/Basic.frag -o ${CMAKE_SOURCE_DIR}/run/resources/shaders/Basic_frag.spv
    COMMAND glslc ${CMAKE_SOURCE_DIR}/run/resources/shaders/Voxel.vert -o ${CMAKE_SOURCE_DIR}/run/resources/shaders/Voxel_vert.spv
    COMMAND glslc ${CMAKE_SOURCE_DIR}/run/resources/shaders/Voxel.frag -o ${CMAKE_SOURCE_DIR}/run/resources/shaders/Voxel_frag.spv
    COMMAND glslc ${CMAKE_SOURCE_DIR}/run/resources/shaders/Blit.vert -o ${CMAKE_SOURCE_DIR}/run/resources/shaders/Blit_vert.spv
    COMMAND glslc ${CMAKE_SOURCE_DIR}/run/resources/shaders/Blit.frag -o ${CMAKE_SOURCE_DIR}/run/resources/shaders/Blit_frag.spv
)
add_dependencies(NebulaEngine_Engine NebulaEngine_Shaders)

if (NEBULA_FORMAT)
    file(GLOB_RECURSE NEBULA_ENGINE_FORMAT_SOURCES
        src/NebulaEngine/*.hpp
        src/NebulaEngine/*.cpp
    )
    add_custom_target(NebulaEngine_Format
        COMMAND clang-format -style=file -i ${NEBULA_ENGINE_FORMAT_SOURCES}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
    add_dependencies(NebulaEngine_Engine NebulaEngine_Format)
endif()

add_subdirectory(tests)
