#include "VulkanImGuiBackend.hpp"
#include "Platform/Vulkan/VulkanGraphicsContext.hpp"

#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_vulkan.h>
#include <vulkan/vulkan_core.h>

#include <utility>

namespace Nebula
{
    VulkanImGuiBackend::VulkanImGuiBackend(RefPtr<Window> window, RefPtr<RendererAPI> api)
    {
        (void)window;
        (void)api;
    }

    VulkanImGuiBackend::~VulkanImGuiBackend() = default;

    void VulkanImGuiBackend::Init()
    {
    }

    void VulkanImGuiBackend::Shutdown()
    {
    }

    void VulkanImGuiBackend::Begin()
    {
    }

    void VulkanImGuiBackend::End()
    {
    }
} // namespace Nebula

