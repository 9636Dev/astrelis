#pragma once

#include "NebulaEngine/Renderer/RendererAPI.hpp"
#include "NebulaEngine/UI/ImGui/ImGuiBackend.hpp"
#include "Platform/Vulkan/VK/Fence.hpp"
#include "Platform/Vulkan/VK/ImageViews.hpp"
#include "Platform/Vulkan/VK/RenderPass.hpp"
#include "Platform/Vulkan/VK/Semaphore.hpp"
#include "Platform/Vulkan/VK/SwapChainFrameBuffers.hpp"
#include "VK/CommandBuffer.hpp"
#include "VK/CommandPool.hpp"
#include "VulkanGraphicsContext.hpp"
#include <vulkan/vulkan_core.h>

namespace Nebula
{
    class VulkanImGuiBackend : public ImGuiBackend
    {
    public:
        explicit VulkanImGuiBackend(RefPtr<Window> window, RefPtr<RendererAPI> api);
        ~VulkanImGuiBackend() override;
        VulkanImGuiBackend(const VulkanImGuiBackend&)            = delete;
        VulkanImGuiBackend& operator=(const VulkanImGuiBackend&) = delete;
        VulkanImGuiBackend(VulkanImGuiBackend&&)                 = delete;
        VulkanImGuiBackend& operator=(VulkanImGuiBackend&&)      = delete;

        void Init() override;
        void Shutdown() override;
        void Begin() override;
        void End() override;
    private:
        RefPtr<Window> m_Window;
        RefPtr<VulkanGraphicsContext> m_Context;
        RefPtr<RendererAPI> m_RendererAPI;
        bool m_Initialized = false;

        Vulkan::CommandPool m_CommandPool;
        std::vector<RefPtr<Vulkan::CommandBuffer>> m_CommandBuffers;
        Vulkan::ImageViews m_ImageViews;
        Vulkan::RenderPass m_RenderPass;
        Vulkan::SwapChainFrameBuffers m_SwapChainFrameBuffers;
        std::vector<RefPtr<Vulkan::Fence>> m_InFlightFences;
        std::vector<RefPtr<Vulkan::Semaphore>> m_ImageAvailableSemaphores;
        std::vector<RefPtr<Vulkan::Semaphore>> m_RenderFinishedSemaphores;
        std::size_t m_CurrentFrame = 0;
        std::uint32_t m_CurrentImageIndex = 0;

        VkDescriptorPool m_DescriptorPool = VK_NULL_HANDLE;

        constexpr static std::uint32_t m_MaxFramesInFlight = 2;

    };
} // namespace Nebula

