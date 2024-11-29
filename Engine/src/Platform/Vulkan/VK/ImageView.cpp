#include "ImageView.hpp"

#include "Astrelis/Core/Log.hpp"

#include "Utils.hpp"

namespace Astrelis::Vulkan {
    bool ImageView::Init(LogicalDevice& device, VkImage image, VkFormat format) {
        VkImageViewCreateInfo createInfo {};
        createInfo.sType    = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image    = image;
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format   = format;
        if (Utils::HasDepthComponent(format)) {
            createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
            if (Utils::HasStencilComponent(format)) {
                createInfo.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
            }
        }
        else {
            createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        }
        createInfo.subresourceRange.baseMipLevel   = 0;
        createInfo.subresourceRange.levelCount     = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount     = 1;

        if (vkCreateImageView(device.GetHandle(), &createInfo, nullptr, &m_ImageView)
            != VK_SUCCESS) {
            ASTRELIS_CORE_LOG_ERROR("Failed to create image view!");
            return false;
        }
        return true;
    }

    void ImageView::Destroy(LogicalDevice& device) {
        vkDestroyImageView(device.GetHandle(), m_ImageView, nullptr);
        m_ImageView = VK_NULL_HANDLE;
    }
} // namespace Astrelis::Vulkan
