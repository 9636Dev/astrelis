#include "ImageViews.hpp"

namespace Nebula::Vulkan
{
    bool ImageViews::Init(LogicalDevice& device, SwapChain& swapChain)
    {
        m_ImageViews.resize(swapChain.GetImages().size());
        for (size_t i = 0; i < m_ImageViews.size(); i++)
        {
            VkImageViewCreateInfo createInfo {};
            createInfo.sType                           = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            createInfo.image                           = swapChain.GetImages()[i];
            createInfo.viewType                        = VK_IMAGE_VIEW_TYPE_2D;
            createInfo.format                          = swapChain.GetImageFormat();
            createInfo.components.r                    = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.g                    = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.b                    = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.a                    = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
            createInfo.subresourceRange.baseMipLevel   = 0;
            createInfo.subresourceRange.levelCount     = 1;
            createInfo.subresourceRange.baseArrayLayer = 0;
            createInfo.subresourceRange.layerCount     = 1;

            if (vkCreateImageView(device.GetHandle(), &createInfo, nullptr, &m_ImageViews[i]) != VK_SUCCESS)
            {
                return false;
            }
        }
        return true;
    }

    void ImageViews::Destroy(LogicalDevice& device)
    {
        for (auto* imageView : m_ImageViews)
        {
            vkDestroyImageView(device.GetHandle(), imageView, nullptr);
        }
    }
} // namespace Nebula::Vulkan
