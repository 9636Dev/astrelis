#include "VulkanRenderSystem.hpp"

#include "Astrelis/Core/Base.hpp"

#include "Astrelis/Core/GlobalConfig.hpp"
#include "Astrelis/Renderer/TextureImage.hpp"

#include <vulkan/vulkan.h>

#include "Platform/Vulkan/VK/TextureSampler.hpp"
#include "Platform/Vulkan/VK/Utils.hpp"
#include "Platform/Vulkan/VK/VulkanExt.hpp"

namespace Astrelis {
    bool VulkanRenderSystem::Init() {
        ASTRELIS_PROFILE_SCOPE("Astrelis::VulkanRenderSystem::Init");
        ASTRELIS_CORE_LOG_TRACE("Initializing VulkanRenderSystem");
#ifdef ASTRELIS_FEATURE_FRAMEBUFFER
        m_GraphicsTextureSampler.Init(m_Context->m_LogicalDevice, m_Context->m_PhysicalDevice);

        auto ctx = static_cast<RefPtr<GraphicsContext>>(m_Context);

        std::vector<DescriptorSetBinding::TextureSampler> samplers;
        samplers.reserve(m_Context->m_Frames.size());
        for (auto& frame : m_Context->m_Frames) {
            samplers.emplace_back(RawRef<TextureImage*>(&frame.GraphicsTextureImage),
                RawRef<TextureSampler*>(m_GraphicsTextureSampler));
        }

        std::vector<DescriptorSetBinding> bindings = {
            DescriptorSetBinding("GraphicsTexture", DescriptorType::TextureSampler, 0,
                DescriptorSetBinding::StageFlags::Fragment, samplers)};

        if (!m_BindingDescriptors.Init(m_Context->m_LogicalDevice, m_Context->m_DescriptorPool,
                static_cast<std::uint32_t>(m_Context->m_Frames.size()), bindings)) {
            ASTRELIS_CORE_LOG_ERROR("Failed to create binding descriptor set.");
            return false;
        }
#endif
        return true;
    }

    void VulkanRenderSystem::Shutdown() {
        ASTRELIS_PROFILE_SCOPE("Astrelis::VulkanRenderSystem::Shutdown");
        vkDeviceWaitIdle(m_Context->m_LogicalDevice.GetHandle());
#ifdef ASTRELIS_FEATURE_FRAMEBUFFER
        auto ctx = static_cast<RefPtr<GraphicsContext>>(m_Context);
        m_BindingDescriptors.Destroy(m_Context->m_LogicalDevice, m_Context->m_DescriptorPool);
        m_GraphicsTextureSampler.Destroy(ctx);
#endif
    }

    void VulkanRenderSystem::StartGraphicsRenderPass() {
#ifdef ASTRELIS_FEATURE_FRAMEBUFFER
        auto& frame = m_Context->GetCurrentFrame();
    #ifdef ASTRELIS_DEBUG
        if (GlobalConfig::IsDebugMode()) {
            Vulkan::Ext::BeginDebugLabel(
                frame.CommandBuffer.GetHandle(), "GraphicsRender", {0.0F, 1.0F, 0.0F, 1.0F});
        }
    #endif
        m_Context->m_GraphicsRenderPass.Begin(
            frame.CommandBuffer, frame.GraphicsFrameBuffer, m_Context->m_GraphicsExtent);
#else
        m_Context->m_RenderPass.Begin(
            frame.CommandBuffer, frame.GraphicsFrameBuffer, m_Context->m_SwapChain.GetExtent());
#endif
    }

    void VulkanRenderSystem::EndGraphicsRenderPass() {
#ifdef ASTRELIS_FEATURE_FRAMEBUFFER
        m_Context->m_GraphicsRenderPass.End(m_Context->GetCurrentFrame().CommandBuffer);
    #ifdef ASTRELIS_DEBUG
        if (GlobalConfig::IsDebugMode()) {
            Vulkan::Ext::EndDebugLabel(m_Context->GetCurrentFrame().CommandBuffer.GetHandle());
        }
    #endif
#endif
    }

    void VulkanRenderSystem::BlitSwapchain() {
#ifdef ASTRELIS_FEATURE_FRAMEBUFFER
        auto& frame = m_Context->GetCurrentFrame();
    #ifdef ASTRELIS_DEBUG
        if (GlobalConfig::IsDebugMode()) {
            Vulkan::Ext::BeginDebugLabel(
                frame.CommandBuffer.GetHandle(), "BlitSwapchain", {0.0F, 1.0F, 0.0F, 1.0F});
        }
    #endif

        if (!m_BlitSwapchain) {
            m_Context->m_RenderPass.Begin(frame.CommandBuffer,
                m_Context->m_SwapChainFrames[m_Context->m_ImageIndex].FrameBuffer,
                m_Context->m_Swapchain.GetExtent());
    #ifdef ASTRELIS_DEBUG
            if (GlobalConfig::IsDebugMode()) {
                Vulkan::Ext::EndDebugLabel(frame.CommandBuffer.GetHandle());
            }
    #endif

            return;
        }

        auto format = m_Context->m_Swapchain.ImageFormat();
        Vulkan::TransitionImageLayout(frame.CommandBuffer.GetHandle(),
            frame.GraphicsTextureImage.GetImage(), format, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
            VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);

        Vulkan::TransitionImageLayout(frame.CommandBuffer.GetHandle(),
            m_Context->m_Swapchain[m_Context->m_ImageIndex], format,
            VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

        VkImageBlit blitRegion                   = {};
        blitRegion.srcSubresource.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
        blitRegion.srcSubresource.mipLevel       = 0;
        blitRegion.srcSubresource.baseArrayLayer = 0;
        blitRegion.srcSubresource.layerCount     = 1;
        blitRegion.srcOffsets[0]                 = {0, 0, 0};
        blitRegion.srcOffsets[1] = {static_cast<std::int32_t>(m_Context->m_GraphicsExtent.width),
            static_cast<std::int32_t>(m_Context->m_GraphicsExtent.height), 1};
        blitRegion.dstSubresource.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
        blitRegion.dstSubresource.mipLevel       = 0;
        blitRegion.dstSubresource.baseArrayLayer = 0;
        blitRegion.dstSubresource.layerCount     = 1;
        blitRegion.dstOffsets[0]                 = {0, 0, 0};
        blitRegion.dstOffsets[1]                 = {
            static_cast<std::int32_t>(m_Context->m_Swapchain.GetExtent().width),
            static_cast<std::int32_t>(m_Context->m_Swapchain.GetExtent().height), 1};

        vkCmdBlitImage(frame.CommandBuffer.GetHandle(), frame.GraphicsTextureImage.GetImage(),
            VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, m_Context->m_Swapchain[m_Context->m_ImageIndex],
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &blitRegion, VK_FILTER_LINEAR);

        Vulkan::TransitionImageLayout(frame.CommandBuffer.GetHandle(),
            frame.GraphicsTextureImage.GetImage(), format, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
            VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

        Vulkan::TransitionImageLayout(frame.CommandBuffer.GetHandle(),
            m_Context->m_Swapchain[m_Context->m_ImageIndex], format,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);

        m_Context->m_RenderPass.Begin(frame.CommandBuffer,
            m_Context->m_SwapChainFrames[m_Context->m_ImageIndex].FrameBuffer,
            m_Context->m_Swapchain.GetExtent());
    #ifdef ASTRELIS_DEBUG
        if (GlobalConfig::IsDebugMode()) {
            Vulkan::Ext::EndDebugLabel(frame.CommandBuffer.GetHandle());
        }
    #endif
#endif
    }

    void VulkanRenderSystem::EndFrame() {
        m_Context->m_RenderPass.End(m_Context->GetCurrentFrame().CommandBuffer);
    }

    std::future<InMemoryImage> VulkanRenderSystem::CaptureFrame(const FrameCaptureProps& props) {
        // Reset capture promise
        m_Context->m_CapturePromise             = std::promise<InMemoryImage>();
        m_Context->m_CaptureNextFrame           = true;
        m_Context->m_CaptureOutputExtent.width  = props.Width;
        m_Context->m_CaptureOutputExtent.height = props.Height;
        return m_Context->m_CapturePromise.get_future();
    }
} // namespace Astrelis
