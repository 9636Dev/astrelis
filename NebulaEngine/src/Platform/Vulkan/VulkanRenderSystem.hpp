#pragma once

#include "NebulaEngine/Renderer/RenderSystem.hpp"

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
        ~VulkanRenderSystem() override = default;
        VulkanRenderSystem(const VulkanRenderSystem&) = delete;
        VulkanRenderSystem(VulkanRenderSystem&&) = delete;
        VulkanRenderSystem& operator=(const VulkanRenderSystem&) = delete;
        VulkanRenderSystem& operator=(VulkanRenderSystem&&) = delete;

        bool Init() override;
        void Shutdown() override;
        void StartGraphicsRenderPass() override;
        void EndGraphicsRenderPass() override;
        void StartOverlayRenderPass() override;
        void EndOverlayRenderPass() override;
        void BlitSwapchain() override;

        static RefPtr<VulkanRenderSystem> Create(RefPtr<Window>& window)
        {
            auto ctx = window->GetGraphicsContext().As<VulkanGraphicsContext>();
            return RefPtr<VulkanRenderSystem>::Create(ctx);
        }
    private:
        RefPtr<VulkanGraphicsContext> m_Context;

        RefPtr<Vulkan::TextureSampler> m_GraphicsTextureSampler;
        RefPtr<Vulkan::TextureSampler> m_UITextureSampler;

        Vulkan::GraphicsPipeline m_GraphicsPipeline;
        Vulkan::VertexBuffer m_VertexBuffer;
        Vulkan::IndexBuffer m_IndexBuffer;
    };
} // namespace Nebula
