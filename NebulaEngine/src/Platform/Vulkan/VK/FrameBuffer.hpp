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

        bool Init(LogicalDevice& device, RenderPass& renderPass, ImageView& imageView, std::uint32_t width, std::uint32_t height);
        void Destroy(LogicalDevice& device);

        VkFramebuffer GetHandle() const { return m_Buffer; }
    private:
        VkFramebuffer m_Buffer;
    };
} // namespace Nebula::Vulkan
