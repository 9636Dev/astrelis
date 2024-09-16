#pragma once

#include "Astrelis/Renderer/RenderSystem.hpp"

#include "VK/DescriptorSetLayout.hpp"
#include "VK/DescriptorSets.hpp"
#include "VK/GraphicsPipeline.hpp"
#include "VK/IndexBuffer.hpp"
#include "VK/TextureSampler.hpp"
#include "VK/VertexBuffer.hpp"
#include "VulkanGraphicsContext.hpp"

namespace Astrelis {
    // TODO: Make this the cross platform implementation, and not per platform
    class VulkanRenderSystem : public RenderSystem {
    public:
        explicit VulkanRenderSystem(RefPtr<VulkanGraphicsContext>& context) : m_Context(context) {
        }

        ~VulkanRenderSystem() override                           = default;
        VulkanRenderSystem(const VulkanRenderSystem&)            = delete;
        VulkanRenderSystem(VulkanRenderSystem&&)                 = delete;
        VulkanRenderSystem& operator=(const VulkanRenderSystem&) = delete;
        VulkanRenderSystem& operator=(VulkanRenderSystem&&)      = delete;

        bool                       Init() override;
        void                       Shutdown() override;
        void                       StartGraphicsRenderPass() override;
        void                       EndGraphicsRenderPass() override;
        void                       BlitSwapchain() override;
        void                       EndFrame() override;
        std::future<InMemoryImage> CaptureFrame(const FrameCaptureProps& props) override;

        void SetBlitSwapchain(bool blit) override {
#ifdef ASTRELIS_FEATURE_FRAMEBUFFER
            m_BlitSwapchain = blit;
#else
            ASTRELIS_UNUSED(blit);
#endif
        }

        // This is for ImGui to render, so we need the descriptor set for Vulkan
        void* GetGraphicsImage() override {
#ifdef ASTRELIS_FEATURE_FRAMEBUFFER
            return m_DescriptorSets.GetHandle();
#else
            ASTRELIS_ASSERT(false, "Framebuffer is not enabled");
            return nullptr;
#endif
        }

        Rect2Di GetRenderBounds() override {
            return Rect2Di {0, 0, static_cast<int32_t>(m_Context->m_SwapChain.GetExtent().width),
                static_cast<int32_t>(m_Context->m_SwapChain.GetExtent().height)};
        }

        static RefPtr<VulkanRenderSystem> Create(RefPtr<Window>& window) {
            auto ctx = window->GetGraphicsContext().As<VulkanGraphicsContext>();
            return RefPtr<VulkanRenderSystem>::Create(ctx);
        }
    private:
        RefPtr<VulkanGraphicsContext> m_Context;

#ifdef ASTRELIS_FEATURE_FRAMEBUFFER
        Vulkan::GraphicsPipeline       m_GraphicsPipeline;
        RefPtr<Vulkan::TextureSampler> m_GraphicsTextureSampler;
        Vulkan::VertexBuffer           m_VertexBuffer;
        Vulkan::IndexBuffer            m_IndexBuffer;
        Vulkan::DescriptorSets         m_DescriptorSets;
        Vulkan::DescriptorSetLayout    m_DescriptorSetLayout;

        bool m_BlitSwapchain = true;
#endif
    };
} // namespace Astrelis
