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

set(ASTRELIS_BUILD_TYPE "Debug" CACHE STRING "The build type for Astrelis")
set_property(CACHE ASTRELIS_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "Dist")

if(ASTRELIS_BUILD_TYPE STREQUAL "Debug")
    set(ASTRELIS_LOGLEVEL ${ASTRELIS_DEBUG_LOGLEVEL})
    if (NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "The build type for Astrelis" FORCE)
    elseif(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
        message(WARNING "CMAKE_BUILD_TYPE is set to ${CMAKE_BUILD_TYPE}, but ASTRELIS_BUILD_TYPE is set to Debug. This may cause unexpected behavior.")
        set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "The build type for Astrelis" FORCE)
    endif()
elseif(ASTRELIS_BUILD_TYPE STREQUAL "Release")
    set(ASTRELIS_LOGLEVEL ${ASTRELIS_RELEASE_LOGLEVEL})
    if (NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE "Release" CACHE STRING "The build type for Astrelis" FORCE)
    elseif(NOT CMAKE_BUILD_TYPE STREQUAL "Release")
        message(WARNING "CMAKE_BUILD_TYPE is set to ${CMAKE_BUILD_TYPE}, but ASTRELIS_BUILD_TYPE is set to Release. This may cause unexpected behavior.")
        set(CMAKE_BUILD_TYPE "Release" CACHE STRING "The build type for Astrelis" FORCE)
    endif()
elseif(ASTRELIS_BUILD_TYPE STREQUAL "Dist")
    set(ASTRELIS_LOGLEVEL ${ASTRELIS_DIST_LOGLEVEL})
    if (NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE "Release" CACHE STRING "The build type for Astrelis" FORCE)
    elseif(NOT CMAKE_BUILD_TYPE STREQUAL "Release")
        message(WARNING "CMAKE_BUILD_TYPE is set to ${CMAKE_BUILD_TYPE}, but ASTRELIS_BUILD_TYPE is set to Dist. This may cause unexpected behavior (should be Release).")
        set(CMAKE_BUILD_TYPE "Release" CACHE STRING "The build type for Astrelis" FORCE)
    endif()
endif()

option(ASTRELIS_PEDANTIC_WARNINGS "Enable pedantic warnings" ON)
option(ASTRELIS_PROFILE "Enable profiling" ON)

# Feature Options
option(ASTRELIS_FEATURE_FRAMEBUFFER "Enable framebuffer support" OFF)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


if (ASTRELIS_PLATFORM STREQUAL "Linux")
    set(ASTRELIS_RENDERER_OPENGL OFF)
    set(ASTRELIS_RENDERER_METAL OFF)
    set(ASTRELIS_RENDERER_VULKAN ON)
elseif (ASTRELIS_PLATFORM STREQUAL "MacOS")
    # TODO: Add Metal renderer, and disable OpenGL
    set(ASTRELIS_RENDERER_OPENGL OFF)
    set(ASTRELIS_RENDERER_METAL OFF)
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
    # Core
    src/Astrelis/Core/Base.hpp
    src/Astrelis/Core/Application.cpp
    src/Astrelis/Core/Application.hpp
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
    src/Astrelis/Core/Utils/Profiling.hpp
    src/Astrelis/Core/Utils/Profiling.cpp
    src/Astrelis/Core/Utils/PlatformDetection.hpp

    # IO
    src/Astrelis/IO/File.hpp
    src/Astrelis/IO/File.cpp
    src/Astrelis/IO/Image.hpp
    src/Astrelis/IO/Image.cpp

    # Renderer
    src/Astrelis/Renderer/BaseRenderer.cpp
    src/Astrelis/Renderer/BaseRenderer.hpp
    src/Astrelis/Renderer/GraphicsContext.cpp
    src/Astrelis/Renderer/GraphicsContext.hpp
    src/Astrelis/Renderer/RenderSystem.cpp
    src/Astrelis/Renderer/RenderSystem.hpp
    src/Astrelis/Renderer/Renderer2D.cpp
    src/Astrelis/Renderer/Renderer2D.hpp
    src/Astrelis/Renderer/RendererAPI.cpp
    src/Astrelis/Renderer/RendererAPI.hpp
    src/Astrelis/Renderer/VoxelRenderer.cpp
    src/Astrelis/Renderer/VoxelRenderer.hpp

    src/Astrelis/Renderer/DescriptorSetLayout.hpp
    src/Astrelis/Renderer/DescriptorSets.hpp
    src/Astrelis/Renderer/GraphicsPipeline.hpp
    src/Astrelis/Renderer/IndexBuffer.hpp
    src/Astrelis/Renderer/TextureImage.hpp
    src/Astrelis/Renderer/TextureSampler.hpp
    src/Astrelis/Renderer/UniformBuffer.hpp
    src/Astrelis/Renderer/VertexBuffer.hpp

    # UI
    src/Astrelis/UI/ImGui/ImGuiBackend.hpp
    src/Astrelis/UI/ImGui/ImGuiBackend.cpp
    src/Astrelis/UI/ImGui/ImGuiLayer.hpp
    src/Astrelis/UI/ImGui/ImGuiLayer.cpp

    # Scene
    src/Astrelis/Scene/Scene.hpp
    src/Astrelis/Scene/Scene.cpp
    src/Astrelis/Scene/Scene2D.hpp
    src/Astrelis/Scene/Scene2D.cpp

    # Platform
    $<$<STREQUAL:${ASTRELIS_PLATFORM},Linux>:
        src/Platform/Linux/LinuxWindow.hpp
        src/Platform/Linux/LinuxWindow.cpp
    >
    $<$<STREQUAL:${ASTRELIS_PLATFORM},MacOS>:
        src/Platform/MacOS/MacOSWindow.hpp
        src/Platform/MacOS/MacOSWindow.cpp
    >
    $<$<STREQUAL:${ASTRELIS_PLATFORM},Windows>:
        src/Platform/Windows/WindowsWindow.hpp
        src/Platform/Windows/WindowsWindow.cpp
    >
    src/Platform/GLFW/GLFWWindowHelper.hpp
    src/Platform/GLFW/GLFWWindowHelper.cpp

    $<$<BOOL:${ASTRELIS_RENDERER_OPENGL}>:
    >

    $<$<BOOL:${ASTRELIS_RENDERER_METAL}>:
    >

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

target_precompile_headers(Astrelis_Engine PRIVATE
    src/Astrelis/Core/Base.hpp
)

target_include_directories(Astrelis_Engine
PUBLIC
    src/
    vendor/tracy/Public
)


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
)

target_compile_definitions(Astrelis_Engine
PUBLIC
    SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_${ASTRELIS_LOGLEVEL}
    $<$<STREQUAL:${ASTRELIS_BUILD_TYPE},Debug>:ASTRELIS_DEBUG>
    $<$<STREQUAL:${ASTRELIS_BUILD_TYPE},Release>:ASTRELIS_RELEASE>
    $<$<STREQUAL:${ASTRELIS_BUILD_TYPE},Dist>:ASTRELIS_DIST>
    $<$<BOOL:${ASTRELIS_PROFILE}>:ASTRELIS_PROFILE>
    $<$<BOOL:${ASTRELIS_RENDERER_OPENGL}>:ASTRELIS_RENDERER_OPENGL>
    $<$<BOOL:${ASTRELIS_RENDERER_METAL}>:ASTRELIS_RENDERER_METAL>
    $<$<BOOL:${ASTRELIS_RENDERER_VULKAN}>:ASTRELIS_RENDERER_VULKAN>

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

add_custom_target(Astrelis_Shaders
    COMMAND glslc ${CMAKE_SOURCE_DIR}/run/resources/shaders/Basic.vert -o ${CMAKE_SOURCE_DIR}/run/resources/shaders/Basic_vert.spv
    COMMAND glslc ${CMAKE_SOURCE_DIR}/run/resources/shaders/Basic.frag -o ${CMAKE_SOURCE_DIR}/run/resources/shaders/Basic_frag.spv
    COMMAND glslc ${CMAKE_SOURCE_DIR}/run/resources/shaders/Voxel.vert -o ${CMAKE_SOURCE_DIR}/run/resources/shaders/Voxel_vert.spv
    COMMAND glslc ${CMAKE_SOURCE_DIR}/run/resources/shaders/Voxel.frag -o ${CMAKE_SOURCE_DIR}/run/resources/shaders/Voxel_frag.spv
    COMMAND glslc ${CMAKE_SOURCE_DIR}/run/resources/shaders/Blit.vert -o ${CMAKE_SOURCE_DIR}/run/resources/shaders/Blit_vert.spv
    COMMAND glslc ${CMAKE_SOURCE_DIR}/run/resources/shaders/Blit.frag -o ${CMAKE_SOURCE_DIR}/run/resources/shaders/Blit_frag.spv
)
add_dependencies(Astrelis_Engine Astrelis_Shaders)

if (ASTRELIS_FORMAT)
    file(GLOB_RECURSE ASTRELIS_ENGINE_FORMAT_SOURCES
        src/Astrelis/*.hpp
        src/Astrelis/*.cpp
    )
    add_custom_target(Astrelis_Format
        COMMAND clang-format -style=file -i ${ASTRELIS_ENGINE_FORMAT_SOURCES}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
    add_dependencies(Astrelis_Engine Astrelis_Format)
endif()

add_subdirectory(tests)
