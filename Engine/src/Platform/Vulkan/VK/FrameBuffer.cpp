#include "FrameBuffer.hpp"

#include "Astrelis/Core/Log.hpp"

#include <array>

#include "LogicalDevice.hpp"

namespace Astrelis::Vulkan
{
    bool FrameBuffer::Init(LogicalDevice& device, RenderPass& renderPass, VkImageView imageView, VkExtent2D extent)
    {
        std::array<VkImageView, 1> attachments = {imageView};

        VkFramebufferCreateInfo framebufferInfo {};
        framebufferInfo.sType           = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass      = renderPass.m_RenderPass;
        framebufferInfo.attachmentCount = static_cast<std::uint32_t>(attachments.size());
        framebufferInfo.pAttachments    = attachments.data();
        framebufferInfo.width           = extent.width;
        framebufferInfo.height          = extent.height;
        framebufferInfo.layers          = 1;

        if (vkCreateFramebuffer(device.GetHandle(), &framebufferInfo, nullptr, &m_Buffer) != VK_SUCCESS)
        {
            ASTRELIS_CORE_LOG_ERROR("Failed to create framebuffer!");
            return false;
        }

        return true;
    }

    void FrameBuffer::Destroy(LogicalDevice& device) const
    {
        vkDestroyFramebuffer(device.GetHandle(), m_Buffer, nullptr);
    }
} // namespace Astrelis::Vulkan

