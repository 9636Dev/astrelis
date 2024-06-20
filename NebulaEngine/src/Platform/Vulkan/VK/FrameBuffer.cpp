#include "FrameBuffer.hpp"

#include "NebulaEngine/Core/Log.hpp"
#include "Platform/Vulkan/VK/LogicalDevice.hpp"

namespace Nebula::Vulkan
{
    bool FrameBuffer::Init(LogicalDevice& device, RenderPass& renderPass, ImageView& imageView, std::uint32_t width, std::uint32_t height)
    {
        std::array<VkImageView, 1> attachments = {imageView.GetHandle()};

        VkFramebufferCreateInfo framebufferInfo {};
        framebufferInfo.sType           = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass      = renderPass.m_RenderPass;
        framebufferInfo.attachmentCount = static_cast<std::uint32_t>(attachments.size());
        framebufferInfo.pAttachments    = attachments.data();
        framebufferInfo.width           = width;
        framebufferInfo.height          = height;
        framebufferInfo.layers          = 1;

        if (vkCreateFramebuffer(device.GetHandle(), &framebufferInfo, nullptr, &m_Buffer) != VK_SUCCESS)
        {
            NEBULA_CORE_LOG_ERROR("Failed to create framebuffer!");
            return false;
        }

        return true;
    }

    void FrameBuffer::Destroy(LogicalDevice& device) { vkDestroyFramebuffer(device.GetHandle(), m_Buffer, nullptr); }
} // namespace Nebula::Vulkan

