#include "VulkanImGuiBackend.hpp"

#include "Astrelis/Core/Base.hpp"

#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_vulkan.h>
#include <vulkan/vulkan_core.h>

#include "Platform/Vulkan/VK/CommandBuffer.hpp"
#include "Platform/Vulkan/Vulkan2DRendererAPI.hpp"
#include "Platform/Vulkan/VulkanGraphicsContext.hpp"

namespace Astrelis {
    VulkanImGuiBackend::VulkanImGuiBackend(RefPtr<Window> window, RefPtr<RendererAPI> api)
        : m_Window(std::move(window)),
          m_Context(m_Window->GetGraphicsContext().As<VulkanGraphicsContext>()),
          m_API(api.As<Vulkan2DRendererAPI>()) {
    }

    VulkanImGuiBackend::~VulkanImGuiBackend() = default;

    void VulkanImGuiBackend::Init() {
        ASTRELIS_PROFILE_FUNCTION();
        ImGui_ImplGlfw_InitForVulkan(
            reinterpret_cast<GLFWwindow*>(m_Window->GetNativeWindow()), true);

        ImGui_ImplVulkan_InitInfo initInfo = {};
        initInfo.Instance                  = m_Context->m_Instance;
        initInfo.PhysicalDevice            = m_Context->m_PhysicalDevice.GetHandle();
        initInfo.Device                    = m_Context->m_LogicalDevice.GetHandle();
        initInfo.QueueFamily =
            m_Context->m_LogicalDevice.GetQueueFamilyIndices().graphicsFamily.value();
        initInfo.Queue           = m_Context->m_LogicalDevice.GetGraphicsQueue();
        initInfo.PipelineCache   = VK_NULL_HANDLE;
        initInfo.DescriptorPool  = m_Context->m_DescriptorPool.GetHandle();
        initInfo.Allocator       = nullptr;
        initInfo.MinImageCount   = 2;
        initInfo.ImageCount      = m_Context->m_Swapchain.ImageCount();
        initInfo.CheckVkResultFn = nullptr;
        initInfo.RenderPass      = m_Context->m_RenderPass.GetHandle();
        initInfo.MSAASamples     = m_Context->m_MSAASamples;

        ImGui_ImplVulkan_Init(&initInfo);
    }

    void VulkanImGuiBackend::Shutdown() {
        ASTRELIS_PROFILE_FUNCTION();
        m_API->WaitDeviceIdle();
        ImGui_ImplVulkan_Shutdown();
        ImGui_ImplGlfw_Shutdown();
    }

    void VulkanImGuiBackend::Begin() {
        ASTRELIS_PROFILE_FUNCTION();
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
    }

    void VulkanImGuiBackend::End() {
        ASTRELIS_PROFILE_FUNCTION();
        ImDrawData* drawData = ImGui::GetDrawData();

        Vulkan::CommandBuffer& commandBuffer = m_Context->GetCurrentFrame().CommandBuffer;
        ImGui_ImplVulkan_RenderDrawData(drawData, commandBuffer.GetHandle());
    }

    void VulkanImGuiBackend::Resize(std::int32_t width, std::int32_t height) {
        ASTRELIS_PROFILE_FUNCTION();
        ASTRELIS_UNUSED(width, height);
        m_API->ResizeViewport();
    }
} // namespace Astrelis
