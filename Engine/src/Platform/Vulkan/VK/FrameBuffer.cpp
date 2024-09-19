#include "FrameBuffer.hpp"

#include "Astrelis/Core/Log.hpp"

#include "LogicalDevice.hpp"

namespace Astrelis::Vulkan {
    bool FrameBuffer::Init(LogicalDevice& device, RenderPass& renderPass,
        const std::vector<VkImageView>& attachments, VkExtent2D extent) {
        VkFramebufferCreateInfo framebufferInfo {};
        framebufferInfo.sType           = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass      = renderPass.m_RenderPass;
        framebufferInfo.attachmentCount = static_cast<std::uint32_t>(attachments.size());
        framebufferInfo.pAttachments    = attachments.data();
        framebufferInfo.width           = extent.width;
        framebufferInfo.height          = extent.height;
        framebufferInfo.layers          = 1;

        if (vkCreateFramebuffer(device.GetHandle(), &framebufferInfo, nullptr, &m_Buffer)
            != VK_SUCCESS) {
            ASTRELIS_CORE_LOG_ERROR("Failed to create framebuffer!");
            return false;
        }

        return true;
    }

    void FrameBuffer::Destroy(LogicalDevice& device) {
        vkDestroyFramebuffer(device.GetHandle(), m_Buffer, nullptr);
        m_Buffer = VK_NULL_HANDLE;
    }
} // namespace Astrelis::Vulkan
