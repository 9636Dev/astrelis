#pragma once

#include <vector>
#include <vulkan/vulkan.h>

#include "LogicalDevice.hpp"
#include "RenderPass.hpp"

namespace Astrelis::Vulkan {
    class FrameBuffer {
    public:
        FrameBuffer()                              = default;
        ~FrameBuffer()                             = default;
        FrameBuffer(const FrameBuffer&)            = default;
        FrameBuffer& operator=(const FrameBuffer&) = default;
        FrameBuffer(FrameBuffer&&)                 = default;
        FrameBuffer& operator=(FrameBuffer&&)      = default;

        [[nodiscard]] bool Init(LogicalDevice& device, RenderPass& renderPass,
            const std::vector<VkImageView>& attachments, VkExtent2D extent);
        void               Destroy(LogicalDevice& device);

        VkFramebuffer GetHandle() const {
            return m_Buffer;
        }

        VkFramebuffer m_Buffer = VK_NULL_HANDLE;
    };
} // namespace Astrelis::Vulkan
