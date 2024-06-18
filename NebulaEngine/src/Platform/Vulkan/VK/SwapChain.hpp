#pragma once

#include "LogicalDevice.hpp"
#include "PhysicalDevice.hpp"
#include "Surface.hpp"

#include <vulkan/vulkan.h>

namespace Nebula::Vulkan
{
    class SwapChain
    {
    public:
        SwapChain()                            = default;
        virtual ~SwapChain()                   = default;
        SwapChain(const SwapChain&)            = delete;
        SwapChain& operator=(const SwapChain&) = delete;
        SwapChain(SwapChain&&)                 = delete;
        SwapChain& operator=(SwapChain&&)      = delete;

        bool Init(PhysicalDevice& physicalDevice, LogicalDevice& logicalDevice, Surface& surface);
        void Destroy(LogicalDevice& device);

        [[nodiscard]] VkSwapchainKHR GetHandle() const { return m_SwapChain; }
        [[nodiscard]] VkFormat GetImageFormat() const { return m_ImageFormat; }
        [[nodiscard]] VkExtent2D GetExtent() const { return m_Extent; }
        [[nodiscard]] const std::vector<VkImage>& GetImages() const { return m_Images; }
    private:
        VkSwapchainKHR m_SwapChain = VK_NULL_HANDLE;
        VkFormat m_ImageFormat = VK_FORMAT_UNDEFINED;
        VkExtent2D m_Extent = {0, 0};
        std::vector<VkImage> m_Images;
    };
} // namespace Nebula::Vulkan
