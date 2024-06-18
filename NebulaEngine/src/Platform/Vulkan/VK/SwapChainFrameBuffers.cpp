#include "SwapChainFrameBuffers.hpp"

#include "ImageViews.hpp"

#include "NebulaEngine/Core/Log.hpp"
#include "Platform/Vulkan/VK/LogicalDevice.hpp"

namespace Nebula::Vulkan
{
    bool SwapChainFrameBuffers::Init(LogicalDevice& device,
                                     RenderPass& renderPass,
                                     ImageViews& imageViews,
                                     SwapChain& chain)
    {
        m_SwapChainFrameBuffers.resize(imageViews.GetImageViews().size());
        for (std::size_t i = 0; i < imageViews.GetImageViews().size(); i++)
        {
            VkImageView attachments[] = {imageViews.GetImageViews()[i]};

            VkFramebufferCreateInfo framebufferInfo {};
            framebufferInfo.sType           = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferInfo.renderPass      = renderPass.m_RenderPass;
            framebufferInfo.attachmentCount = 1;
            framebufferInfo.pAttachments    = attachments;
            framebufferInfo.width           = chain.GetExtent().width;
            framebufferInfo.height          = chain.GetExtent().height;
            framebufferInfo.layers          = 1;

            if (vkCreateFramebuffer(device.GetHandle(), &framebufferInfo, nullptr, &m_SwapChainFrameBuffers[i]) !=
                VK_SUCCESS)
            {
                NEBULA_CORE_LOG_ERROR("Failed to create framebuffer!");
                return false;
            }
        }

        return true;
    }

    void SwapChainFrameBuffers::Destroy(LogicalDevice& device)
    {
        for (auto* framebuffer : m_SwapChainFrameBuffers)
        {
            vkDestroyFramebuffer(device.GetHandle(), framebuffer, nullptr);
        }
    }
} // namespace Nebula::Vulkan

