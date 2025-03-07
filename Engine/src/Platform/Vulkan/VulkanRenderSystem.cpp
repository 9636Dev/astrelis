#include "VulkanRenderSystem.hpp"

#include "Astrelis/Core/Base.hpp"

#include "Astrelis/Core/GlobalConfig.hpp"
#include "Astrelis/Renderer/TextureImage.hpp"

#include <vulkan/vulkan.h>

#include "Platform/Vulkan/VK/TextureSampler.hpp"
#include "Platform/Vulkan/VK/Utils.hpp"
#include "Platform/Vulkan/VK/VulkanExt.hpp"

namespace Astrelis {
    Result<EmptyType, std::string> VulkanRenderSystem::Init() {
        ASTRELIS_PROFILE_SCOPE("Astrelis::VulkanRenderSystem::Init");
        ASTRELIS_CORE_LOG_TRACE("Initializing VulkanRenderSystem");
        if (!m_GraphicsTextureSampler.Init(
                m_Context->m_LogicalDevice, m_Context->m_PhysicalDevice)) {
            return "Failed to initialize graphics texture sampler";
        }

        std::vector<DescriptorSetBinding::TextureSampler> samplers;
        samplers.reserve(m_Context->m_Frames.size());
        for (auto& frame : m_Context->m_Frames) {
            samplers.emplace_back(RawRef<TextureImage*>(&frame.GraphicsTextureImage),
                RawRef<TextureSampler*>(&m_GraphicsTextureSampler));
        }

        std::vector<DescriptorSetBinding> bindings = {
            DescriptorSetBinding("GraphicsTexture", DescriptorType::TextureSampler, 0,
                DescriptorSetBinding::StageFlags::Fragment, samplers)};

        if (!m_BindingDescriptors.Init(m_Context->m_LogicalDevice, m_Context->m_DescriptorPool,
                static_cast<std::uint32_t>(m_Context->m_Frames.size()), bindings)) {
            return "Failed to initialize binding descriptors";
        }
        return EmptyType {};
    }

    void VulkanRenderSystem::Shutdown() {
        ASTRELIS_PROFILE_SCOPE("Astrelis::VulkanRenderSystem::Shutdown");
        vkDeviceWaitIdle(m_Context->m_LogicalDevice.GetHandle());
        m_BindingDescriptors.Destroy(m_Context->m_LogicalDevice, m_Context->m_DescriptorPool);
        m_GraphicsTextureSampler.Destroy(m_Context->m_LogicalDevice);
    }

    void VulkanRenderSystem::StartGraphicsRenderPass() {
        auto& frame = m_Context->GetCurrentFrame();
#ifdef ASTRELIS_DEBUG
        if (GlobalConfig::IsDebugMode()) {
            Vulkan::Ext::BeginDebugLabel(
                frame.CommandBuffer.GetHandle(), "GraphicsRender", {0.0F, 1.0F, 0.0F, 1.0F});
        }
#endif
        std::vector<VkClearValue> clearValues(2);
        clearValues[0].color = {
            {0.0F, 0.0F, 0.0F, 1.0F}
        };
        clearValues[1].depthStencil = {1.0F, 0};
        m_Context->m_GraphicsRenderPass.Begin(frame.CommandBuffer, frame.GraphicsFrameBuffer,
            m_Context->m_GraphicsExtent, clearValues);
    }

    void VulkanRenderSystem::EndGraphicsRenderPass() {
        m_Context->m_GraphicsRenderPass.End(m_Context->GetCurrentFrame().CommandBuffer);
#ifdef ASTRELIS_DEBUG
        if (GlobalConfig::IsDebugMode()) {
            Vulkan::Ext::EndDebugLabel(m_Context->GetCurrentFrame().CommandBuffer.GetHandle());
        }
#endif
    }

    void VulkanRenderSystem::BlitSwapchain() {
        auto& frame = m_Context->GetCurrentFrame();
#ifdef ASTRELIS_DEBUG
        if (GlobalConfig::IsDebugMode()) {
            Vulkan::Ext::BeginDebugLabel(
                frame.CommandBuffer.GetHandle(), "BlitSwapchain", {0.0F, 1.0F, 0.0F, 1.0F});
        }
#endif

        std::vector<VkClearValue> clearValues(2);
        clearValues[0].color = {
            {0.0F, 0.0F, 0.0F, 1.0F}
        };
        clearValues[1].depthStencil = {1.0F, 0};

        auto format = m_Context->m_Swapchain.ImageFormat();
        Vulkan::TransitionImageLayout(frame.CommandBuffer.GetHandle(),
            m_Context->m_Swapchain[m_Context->m_ImageIndex], format,
            VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);


        if (!m_BlitSwapchain) {
            // If we are not blitting we just clear it
            VkClearColorValue clearColor = {};
            clearColor.float32[0]        = 0.0F;
            clearColor.float32[1]        = 0.0F;
            clearColor.float32[2]        = 0.0F;
            clearColor.float32[3]        = 1.0F;

            VkImageSubresourceRange subresourceRange = {};
            subresourceRange.aspectMask              = VK_IMAGE_ASPECT_COLOR_BIT;
            subresourceRange.baseMipLevel            = 0;
            subresourceRange.levelCount              = 1;
            subresourceRange.baseArrayLayer          = 0;
            subresourceRange.layerCount              = 1;

            vkCmdClearColorImage(frame.CommandBuffer.GetHandle(),
                m_Context->m_Swapchain[m_Context->m_ImageIndex],
                VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, &clearColor, 1, &subresourceRange);

            Vulkan::TransitionImageLayout(frame.CommandBuffer.GetHandle(),
                m_Context->m_Swapchain[m_Context->m_ImageIndex], format,
                VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);

            m_Context->m_RenderPass.Begin(frame.CommandBuffer,
                m_Context->m_SwapChainFrames[m_Context->m_ImageIndex].FrameBuffer,
                m_Context->m_Swapchain.GetExtent(), clearValues);
#ifdef ASTRELIS_DEBUG
            if (GlobalConfig::IsDebugMode()) {
                Vulkan::Ext::EndDebugLabel(frame.CommandBuffer.GetHandle());
            }
#endif

            return;
        }

        Vulkan::TransitionImageLayout(frame.CommandBuffer.GetHandle(),
            frame.GraphicsTextureImage.GetImage(), format, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
            VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);

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
            m_Context->m_Swapchain.GetExtent(), clearValues);
#ifdef ASTRELIS_DEBUG
        if (GlobalConfig::IsDebugMode()) {
            Vulkan::Ext::EndDebugLabel(frame.CommandBuffer.GetHandle());
        }
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
