#include "VulkanImGuiBackend.hpp"
#include "Platform/Vulkan/VulkanGraphicsContext.hpp"

#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_vulkan.h>
#include <vulkan/vulkan_core.h>

#include <utility>

namespace Nebula
{
    VulkanImGuiBackend::VulkanImGuiBackend(RefPtr<Window> window, RefPtr<RendererAPI> api) :
        m_Window(std::move(window)),
        m_Context(m_Window->GetGraphicsContext().As<VulkanGraphicsContext>()),
        m_RendererAPI(std::move(api))
    {
        (void)window;
    }

    VulkanImGuiBackend::~VulkanImGuiBackend() = default;

    void VulkanImGuiBackend::Init()
    {
        if (m_Initialized)
        {
            return;
        }

        m_Initialized = true;
        ImGui_ImplGlfw_InitForVulkan(reinterpret_cast<GLFWwindow*>(m_Window->GetNativeWindow()), true);

        RendererAPI::CreateDetails details;
        details.VertexInput.Stride   = sizeof(ImDrawVert);
        details.VertexInput.Elements = {
            {offsetof(ImDrawVert, pos), 2, 0},
            {offsetof(ImDrawVert, uv),  2, 0},
            {offsetof(ImDrawVert, col), 4, 0}
        };

        m_ImageViews.Init(m_Context->m_LogicalDevice, m_Context->m_SwapChain);
        m_CommandPool.Init(m_Context->m_LogicalDevice);
        std::size_t imageCount = m_Context->m_SwapChain.GetImages().size();
        m_CommandBuffers.resize(imageCount);
        for (std::size_t i = 0; i < imageCount; ++i)
        {
            m_CommandBuffers[i] = RefPtr<Vulkan::CommandBuffer>::Create();
            m_CommandBuffers[i]->Init(m_Context->m_LogicalDevice, m_CommandPool);
        }

        Vulkan::RenderPassInfo renderPassInfo;
        renderPassInfo.loadOp          = VK_ATTACHMENT_LOAD_OP_LOAD;
        m_RenderPass.Init(m_Context->m_LogicalDevice, m_Context->m_SwapChain, renderPassInfo);
        m_SwapChainFrameBuffers.Init(m_Context->m_LogicalDevice, m_RenderPass, m_ImageViews, m_Context->m_SwapChain);

        m_InFlightFences.resize(m_MaxFramesInFlight);
        m_ImageAvailableSemaphores.resize(m_MaxFramesInFlight);
        m_RenderFinishedSemaphores.resize(m_MaxFramesInFlight);
        for (std::size_t i = 0; i < m_MaxFramesInFlight; ++i)
        {
            m_InFlightFences[i]           = RefPtr<Vulkan::Fence>::Create();
            m_ImageAvailableSemaphores[i] = RefPtr<Vulkan::Semaphore>::Create();
            m_RenderFinishedSemaphores[i] = RefPtr<Vulkan::Semaphore>::Create();

            m_InFlightFences[i]->Init(m_Context->m_LogicalDevice);
            m_ImageAvailableSemaphores[i]->Init(m_Context->m_LogicalDevice);
            m_RenderFinishedSemaphores[i]->Init(m_Context->m_LogicalDevice);
        }

        VkDescriptorPoolCreateInfo poolInfo = {};
        poolInfo.sType                      = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        std::array<VkDescriptorPoolSize, 1> poolSizes = {{{VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1}}};

        poolInfo.poolSizeCount = static_cast<std::uint32_t>(poolSizes.size());
        poolInfo.pPoolSizes    = poolSizes.data();
        poolInfo.maxSets       = 1;
        poolInfo.flags         = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
        if (vkCreateDescriptorPool(m_Context->m_LogicalDevice.GetHandle(), &poolInfo, nullptr, &m_DescriptorPool) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create descriptor pool");
        }


        // Descriptor pool
        ImGui_ImplVulkan_InitInfo initInfo = {};
        initInfo.Instance                  = m_Context->m_Instance;
        initInfo.PhysicalDevice            = m_Context->m_PhysicalDevice.GetHandle();
        initInfo.Device                    = m_Context->m_LogicalDevice.GetHandle();
        initInfo.QueueFamily               = m_Context->m_LogicalDevice.GetQueueFamilyIndices().graphicsFamily.value();
        initInfo.Queue                     = m_Context->m_LogicalDevice.GetGraphicsQueue();
        initInfo.PipelineCache             = VK_NULL_HANDLE;
        initInfo.DescriptorPool            = m_DescriptorPool;
        initInfo.Allocator                 = nullptr;
        initInfo.MinImageCount             = m_Context->m_SwapChain.GetImages().size();
        initInfo.ImageCount                = m_Context->m_SwapChain.GetImages().size();
        initInfo.MSAASamples               = VK_SAMPLE_COUNT_1_BIT;
        initInfo.RenderPass                = m_RenderPass.m_RenderPass;
        initInfo.Subpass                   = 0;

        ImGui_ImplVulkan_Init(&initInfo);
    }

    void VulkanImGuiBackend::Shutdown()
    {
        if (!m_Initialized)
        {
            return;
        }

        m_Initialized = false;

        ImGui_ImplVulkan_Shutdown();
        vkDestroyDescriptorPool(m_Context->m_LogicalDevice.GetHandle(), m_DescriptorPool, nullptr);

        for (std::size_t i = 0; i < m_MaxFramesInFlight; ++i)
        {
            m_ImageAvailableSemaphores[i]->Destroy(m_Context->m_LogicalDevice);
            m_InFlightFences[i]->Destroy(m_Context->m_LogicalDevice);
            m_RenderFinishedSemaphores[i]->Destroy(m_Context->m_LogicalDevice);
        }

        m_SwapChainFrameBuffers.Destroy(m_Context->m_LogicalDevice);
        m_RenderPass.Destroy(m_Context->m_LogicalDevice);
        for (std::size_t i = 0; i < m_CommandBuffers.size(); ++i)
        {
            m_CommandBuffers[i]->Destroy(m_Context->m_LogicalDevice, m_CommandPool);
        }
        m_CommandPool.Destroy(m_Context->m_LogicalDevice);
        m_ImageViews.Destroy(m_Context->m_LogicalDevice);

        ImGui_ImplGlfw_Shutdown();
    }

    void VulkanImGuiBackend::Begin()
    {
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
    }

    void VulkanImGuiBackend::End()
    {
        ImGui::Render();
        auto ctx       = static_cast<RefPtr<GraphicsContext>>(m_Context);
        auto inFlightFence = m_InFlightFences[m_CurrentFrame];
        auto imageAvailableSemaphore = m_ImageAvailableSemaphores[m_CurrentFrame];
        auto semaphore = static_cast<RefPtr<Semaphore>>(imageAvailableSemaphore);

        inFlightFence->Wait(ctx, std::numeric_limits<std::uint64_t>::max());
        m_RendererAPI->AcquireNextImage(ctx, semaphore, m_CurrentImageIndex);
        inFlightFence->Reset(ctx);

        auto commandBuffer = m_CommandBuffers[m_CurrentFrame];
        commandBuffer->Reset();
        commandBuffer->Begin();

        // Begin render pass
        VkRenderPassBeginInfo renderPassInfo = {};
        renderPassInfo.sType                 = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass            = m_RenderPass.m_RenderPass;
        renderPassInfo.framebuffer           = m_SwapChainFrameBuffers.m_SwapChainFrameBuffers[m_CurrentImageIndex];
        renderPassInfo.renderArea.offset     = {0, 0};
        renderPassInfo.renderArea.extent     = m_Context->m_SwapChain.GetExtent();

        vkCmdBeginRenderPass(commandBuffer->m_CommandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        ImDrawData* drawData = ImGui::GetDrawData();
        ImGui_ImplVulkan_RenderDrawData(drawData, m_CommandBuffers[m_CurrentFrame]->m_CommandBuffer);

        vkCmdEndRenderPass(commandBuffer->m_CommandBuffer);

        commandBuffer->End();
        auto finishedRenderingSemaphore = static_cast<RefPtr<Semaphore>>(m_RenderFinishedSemaphores[m_CurrentFrame]);
        auto fence = static_cast<RefPtr<Fence>>(inFlightFence);
        commandBuffer->Submit(ctx, semaphore, finishedRenderingSemaphore, fence);

        m_RendererAPI->Present(m_CurrentImageIndex, finishedRenderingSemaphore);

        m_CurrentFrame = (m_CurrentFrame + 1) % m_MaxFramesInFlight;
    }
} // namespace Nebula

