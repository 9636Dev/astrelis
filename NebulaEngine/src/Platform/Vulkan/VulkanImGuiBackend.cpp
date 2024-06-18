#include "VulkanImGuiBackend.hpp"
#include "Platform/Vulkan/VulkanGraphicsContext.hpp"

#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_vulkan.h>

namespace Nebula
{
    VulkanImGuiBackend::VulkanImGuiBackend(RefPtr<Window> window) : m_Window(std::move(window)), m_Context(m_Window->GetGraphicsContext().As<VulkanGraphicsContext>()) {}

    VulkanImGuiBackend::~VulkanImGuiBackend() = default;

    void VulkanImGuiBackend::Init()
    {
        ImGui_ImplGlfw_InitForVulkan(reinterpret_cast<GLFWwindow*>(m_Window->GetNativeWindow()), true);

        ImGui_ImplVulkan_InitInfo init_info = {};
        init_info.Instance = m_Context->m_Instance;
        init_info.PhysicalDevice = m_Context->m_PhysicalDevice.GetHandle();
        init_info.Device = m_Context->m_LogicalDevice.GetHandle();
        init_info.QueueFamily = m_Context->m_LogicalDevice.GetQueueFamilyIndices().graphicsFamily.value();
        init_info.Queue = m_Context->m_LogicalDevice.GetGraphicsQueue();
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
}  // namespace Nebula
