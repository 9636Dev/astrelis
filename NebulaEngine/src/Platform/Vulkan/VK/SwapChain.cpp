#include "SwapChain.hpp"

#include <array>

#include "NebulaEngine/Core/Log.hpp"
#include "Platform/Vulkan/VK/Utils.hpp"

namespace Nebula::Vulkan
{
    bool SwapChain::Init(const RawRef<GLFWwindow*>& window, PhysicalDevice& physicalDevice, LogicalDevice& logicalDevice, Surface& surface, VkSwapchainKHR oldSwapchain)
    {
        SwapChainSupportDetails swapChainSupport =
            QuerySwapChainSupport(physicalDevice.GetHandle(), surface.GetHandle());

        VkSurfaceFormatKHR surfaceFormat = swapChainSupport.ChooseSwapSurfaceFormat();
        VkPresentModeKHR presentMode = swapChainSupport.ChooseSwapPresentMode();
        VkExtent2D extent = swapChainSupport.ChooseExtent(window);

        std::uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
        if (swapChainSupport.capabilities.maxImageCount > 0 &&
            imageCount > swapChainSupport.capabilities.maxImageCount)
        {
            imageCount = swapChainSupport.capabilities.maxImageCount;
        }

        VkSwapchainCreateInfoKHR createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = surface.GetHandle();
        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = surfaceFormat.format;
        createInfo.imageColorSpace = surfaceFormat.colorSpace;
        createInfo.imageExtent = extent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        const QueueFamilyIndices& indices = logicalDevice.GetQueueFamilyIndices();
        std::array<std::uint32_t, 2> queueFamilyIndices = {indices.graphicsFamily.value(), indices.presentFamily.value()};

        if (indices.graphicsFamily != indices.presentFamily)
        {
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = queueFamilyIndices.data();
        }
        else
        {
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            createInfo.queueFamilyIndexCount = 0;
            createInfo.pQueueFamilyIndices = nullptr;
        }

        createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        createInfo.presentMode = presentMode;
        createInfo.clipped = VK_TRUE;
        createInfo.oldSwapchain = oldSwapchain;

        if (vkCreateSwapchainKHR(logicalDevice.GetHandle(), &createInfo, nullptr, &m_SwapChain) != VK_SUCCESS)
        {
            NEBULA_CORE_LOG_ERROR("Failed to create swap chain!");
            return false;
        }

        vkGetSwapchainImagesKHR(logicalDevice.GetHandle(), m_SwapChain, &imageCount, nullptr);
        m_Images.resize(imageCount);
        vkGetSwapchainImagesKHR(logicalDevice.GetHandle(), m_SwapChain, &imageCount, m_Images.data());

        m_ImageFormat = surfaceFormat.format;
        m_Extent = extent;

        return true;
    }

    void SwapChain::Destroy(LogicalDevice& logicalDevice)
    {
        vkDestroySwapchainKHR(logicalDevice.GetHandle(), m_SwapChain, nullptr);
    }
} // namespace Nebula::Vulkan
