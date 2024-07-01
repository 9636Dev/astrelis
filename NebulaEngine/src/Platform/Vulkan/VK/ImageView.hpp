#pragma once

#include "LogicalDevice.hpp"

#include <vulkan/vulkan.h>

namespace Nebula::Vulkan
{
    class ImageView
    {
    public:
        ImageView()                             = default;
        ~ImageView()                            = default;
        ImageView(const ImageView&)            = delete;
        ImageView& operator=(const ImageView&) = delete;
        ImageView(ImageView&&)                 = default;
        ImageView& operator=(ImageView&&)      = default;

        [[nodiscard]] bool Init(LogicalDevice& device, VkImage image, VkFormat format);
        void Destroy(LogicalDevice& device);

        VkImageView GetHandle() const { return m_ImageView; }

        VkImageView m_ImageView = VK_NULL_HANDLE;
    };
} // namespace Nebula::Vulkan
