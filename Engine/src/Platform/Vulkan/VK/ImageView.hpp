#pragma once

#include <vulkan/vulkan.h>

#include "LogicalDevice.hpp"

namespace Astrelis::Vulkan {
    class ImageView {
    public:
        ImageView()                            = default;
        ~ImageView()                           = default;
        ImageView(const ImageView&)            = default;
        ImageView& operator=(const ImageView&) = default;
        ImageView(ImageView&&)                 = default;
        ImageView& operator=(ImageView&&)      = default;

        [[nodiscard]] bool Init(LogicalDevice& device, VkImage image, VkFormat format);
        void               Destroy(LogicalDevice& device);

        const VkImageView& GetHandle() const {
            return m_ImageView;
        }

        VkImageView m_ImageView = VK_NULL_HANDLE;
    };
} // namespace Astrelis::Vulkan
