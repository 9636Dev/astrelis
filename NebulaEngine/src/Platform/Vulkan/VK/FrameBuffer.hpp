#pragma once

#include "ImageView.hpp"
#include "LogicalDevice.hpp"
#include "RenderPass.hpp"

#include <vulkan/vulkan.h>

namespace Nebula::Vulkan
{
    class FrameBuffer
    {
    public:
        FrameBuffer()                                        = default;
        ~FrameBuffer()                                       = default;
        FrameBuffer(const FrameBuffer&)            = delete;
        FrameBuffer& operator=(const FrameBuffer&) = delete;
        FrameBuffer(FrameBuffer&&)                 = default;
        FrameBuffer& operator=(FrameBuffer&&)      = default;

        [[nodiscard]] bool Init(LogicalDevice& device, RenderPass& renderPass, ImageView& imageView, VkExtent2D extent);
        void Destroy(LogicalDevice& device);

        VkFramebuffer GetHandle() const { return m_Buffer; }

        VkFramebuffer m_Buffer;
    };
} // namespace Nebula::Vulkan
