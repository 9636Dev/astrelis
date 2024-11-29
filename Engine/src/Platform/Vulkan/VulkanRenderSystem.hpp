#pragma once

#include "Astrelis/Renderer/RenderSystem.hpp"

#include "VK/BindingDescriptorSet.hpp"
#include "VK/TextureSampler.hpp"
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

        Result<EmptyType, std::string> Init() override;
        void                           Shutdown() override;
        void                           StartGraphicsRenderPass() override;
        void                           EndGraphicsRenderPass() override;
        void                           BlitSwapchain() override;
        void                           EndFrame() override;
        std::future<InMemoryImage>     CaptureFrame(const FrameCaptureProps& props) override;

        Rect2Du GetGraphicsRenderArea() override {
            return Rect2Du(
                0, 0, m_Context->m_GraphicsExtent.width, m_Context->m_GraphicsExtent.height);
        }

        void SetGraphicsRenderArea(const Rect2Du& area) override {
            ASTRELIS_UNUSED(area);
            // We didn't implement this, but this would involve recreating the images, and framebuffers
        }

        void SetBlitSwapchain(bool blit) override {
            m_BlitSwapchain = blit;
        }

        // This is for ImGui to render, so we need the descriptor set for Vulkan
        void* GetGraphicsImage() override {
            return m_BindingDescriptors.m_DescriptorSets[m_Context->m_CurrentFrame].GetHandle();
        }

        Rect2Di GetRenderBounds() override {
            return Rect2Di {0, 0, static_cast<int32_t>(m_Context->m_Swapchain.GetExtent().width),
                static_cast<int32_t>(m_Context->m_Swapchain.GetExtent().height)};
        }

        static RefPtr<VulkanRenderSystem> Create(RefPtr<Window>& window) {
            auto ctx = window->GetGraphicsContext().As<VulkanGraphicsContext>();
            return RefPtr<VulkanRenderSystem>::Create(ctx);
        }
    private:
        RefPtr<VulkanGraphicsContext> m_Context;

        Vulkan::TextureSampler       m_GraphicsTextureSampler;
        Vulkan::BindingDescriptorSet m_BindingDescriptors;

        bool m_BlitSwapchain = true;
    };
} // namespace Astrelis
