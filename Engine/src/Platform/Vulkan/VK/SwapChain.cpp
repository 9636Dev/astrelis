#include "SwapChain.hpp"

#include "Utils.hpp"

#include <array>

#include "Astrelis/Core/GlobalConfig.hpp"
#include "Astrelis/Core/Log.hpp"

namespace Astrelis::Vulkan
{
    bool SwapChain::Init(const RawRef<GLFWwindow*>& window,
                         PhysicalDevice& physicalDevice,
                         LogicalDevice& logicalDevice,
                         Surface& surface,
                         std::uint32_t& imageCount,
                         bool vsync,
                         VkSwapchainKHR oldSwapchain)
    {
        SwapChainSupportDetails swapChainSupport =
            QuerySwapChainSupport(physicalDevice.GetHandle(), surface.GetHandle());

        VkSurfaceFormatKHR surfaceFormat = swapChainSupport.ChooseSwapSurfaceFormat();
        VkPresentModeKHR presentMode     = vsync ? swapChainSupport.ChooseSwapPresentMode() : VK_PRESENT_MODE_IMMEDIATE_KHR;
        VkExtent2D extent                = swapChainSupport.ChooseExtent(window);

        if (GlobalConfig::IsDebugMode())
        {
            ASTRELIS_CORE_LOG_INFO("Swap chain support details:");
            ASTRELIS_CORE_LOG_INFO("Surface formats:");
            for (const auto& format : swapChainSupport.formats)
            {
                ASTRELIS_CORE_LOG_INFO("\tFormat: {0}, Color space: {1}", Utils::VkFormatToString(format.format),
                                     Utils::VkColorSpaceToString(format.colorSpace));
            }

            ASTRELIS_CORE_LOG_INFO("Present modes:");
            for (const auto& mode : swapChainSupport.presentModes)
            {
                ASTRELIS_CORE_LOG_INFO("\t{0}", Utils::VkPresentModeToString(mode));
            }

            ASTRELIS_CORE_LOG_INFO("Surface format: {0}, Color space: {1}", Utils::VkFormatToString(surfaceFormat.format),
                                 Utils::VkColorSpaceToString(surfaceFormat.colorSpace));
            ASTRELIS_CORE_LOG_INFO("Present mode: {0}", Utils::VkPresentModeToString(presentMode));
            ASTRELIS_CORE_LOG_INFO("Extent: {0}x{1}", extent.width, extent.height);
        }

        if (imageCount < swapChainSupport.capabilities.minImageCount)
        {
            imageCount = swapChainSupport.capabilities.minImageCount;
        }
        else if (swapChainSupport.capabilities.maxImageCount != 0 &&
                 imageCount > swapChainSupport.capabilities.maxImageCount)
        {
            imageCount = swapChainSupport.capabilities.maxImageCount;
        }

        VkSwapchainCreateInfoKHR createInfo = {};
        createInfo.sType                    = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface                  = surface.GetHandle();
        createInfo.minImageCount            = imageCount;
        createInfo.imageFormat              = surfaceFormat.format;
        createInfo.imageColorSpace          = surfaceFormat.colorSpace;
        createInfo.imageExtent              = extent;
        createInfo.imageArrayLayers         = 1;
        createInfo.imageUsage               = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;

        const QueueFamilyIndices& indices               = logicalDevice.GetQueueFamilyIndices();
        std::array<std::uint32_t, 2> queueFamilyIndices = {indices.graphicsFamily.value(),
                                                           indices.presentFamily.value()};

        if (indices.graphicsFamily != indices.presentFamily)
        {
            createInfo.imageSharingMode      = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices   = queueFamilyIndices.data();
        }
        else
        {
            createInfo.imageSharingMode      = VK_SHARING_MODE_EXCLUSIVE;
            createInfo.queueFamilyIndexCount = 0;
            createInfo.pQueueFamilyIndices   = nullptr;
        }

        createInfo.preTransform   = swapChainSupport.capabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        createInfo.presentMode    = presentMode;
        createInfo.clipped        = VK_TRUE;
        createInfo.oldSwapchain   = oldSwapchain;

        if (vkCreateSwapchainKHR(logicalDevice.GetHandle(), &createInfo, nullptr, &m_SwapChain) != VK_SUCCESS)
        {
            ASTRELIS_CORE_LOG_ERROR("Failed to create swap chain!");
            return false;
        }

        vkGetSwapchainImagesKHR(logicalDevice.GetHandle(), m_SwapChain, &imageCount, nullptr);
        m_Images.resize(imageCount);
        vkGetSwapchainImagesKHR(logicalDevice.GetHandle(), m_SwapChain, &imageCount, m_Images.data());

        m_ImageFormat = surfaceFormat.format;
        m_Extent      = extent;

        return true;
    }

    void SwapChain::Destroy(LogicalDevice& logicalDevice)
    {
        vkDestroySwapchainKHR(logicalDevice.GetHandle(), m_SwapChain, nullptr);
    }
} // namespace Astrelis::Vulkan
