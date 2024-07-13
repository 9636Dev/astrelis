#pragma once

#include "NebulaEngine/Renderer/RenderSystem.hpp"

#include "Platform/Vulkan/VK/DescriptorSetLayout.hpp"
#include "Platform/Vulkan/VK/DescriptorSets.hpp"
#include "Platform/Vulkan/VK/GraphicsPipeline.hpp"
#include "Platform/Vulkan/VK/IndexBuffer.hpp"
#include "Platform/Vulkan/VK/TextureSampler.hpp"
#include "Platform/Vulkan/VK/VertexBuffer.hpp"
#include "VulkanGraphicsContext.hpp"

namespace Nebula
{
    // TODO: Make this the cross platform implementation, and not per platform
    class VulkanRenderSystem : public RenderSystem
    {
    public:
        explicit VulkanRenderSystem(RefPtr<VulkanGraphicsContext>& context) : m_Context(context) {}

        ~VulkanRenderSystem() override                           = default;
        VulkanRenderSystem(const VulkanRenderSystem&)            = delete;
        VulkanRenderSystem(VulkanRenderSystem&&)                 = delete;
        VulkanRenderSystem& operator=(const VulkanRenderSystem&) = delete;
        VulkanRenderSystem& operator=(VulkanRenderSystem&&)      = delete;

        bool Init() override;
        void Shutdown() override;
        void StartGraphicsRenderPass() override;
        void EndGraphicsRenderPass() override;
        void BlitSwapchain() override;
        void EndFrame() override;
        std::future<InMemoryImage> CaptureFrame() override;

        void SetBlitSwapchain(bool blit) override { m_BlitSwapchain = blit; }

        // This is for ImGui to render, so we need the descriptor set for Vulkan
        void* GetGraphicsImage() override { return m_DescriptorSets.GetHandle(); }

        static RefPtr<VulkanRenderSystem> Create(RefPtr<Window>& window)
        {
            auto ctx = window->GetGraphicsContext().As<VulkanGraphicsContext>();
            return RefPtr<VulkanRenderSystem>::Create(ctx);
        }
    private:
        RefPtr<VulkanGraphicsContext> m_Context;

        RefPtr<Vulkan::TextureSampler> m_GraphicsTextureSampler;

        Vulkan::GraphicsPipeline m_GraphicsPipeline;
        Vulkan::VertexBuffer m_VertexBuffer;
        Vulkan::IndexBuffer m_IndexBuffer;
        Vulkan::DescriptorSets m_DescriptorSets;
        Vulkan::DescriptorSetLayout m_DescriptorSetLayout;

        bool m_BlitSwapchain = true;
    };
} // namespace Nebula
