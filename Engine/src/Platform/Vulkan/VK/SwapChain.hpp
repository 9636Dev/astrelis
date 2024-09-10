#pragma once

#include <vulkan/vulkan.h>

#include "LogicalDevice.hpp"
#include "PhysicalDevice.hpp"
#include "Surface.hpp"

namespace Astrelis::Vulkan
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

        [[nodiscard]] bool Init(const RawRef<GLFWwindow*>& window,
                                PhysicalDevice& physicalDevice,
                                LogicalDevice& logicalDevice,
                                Surface& surface,
                                std::uint32_t& imageCount,
                                bool vsync,
                                VkSwapchainKHR oldSwapchain = VK_NULL_HANDLE);
        void Destroy(LogicalDevice& device);

        [[nodiscard]] VkSwapchainKHR GetHandle() const { return m_SwapChain; }

        [[nodiscard]] VkFormat GetImageFormat() const { return m_ImageFormat; }

        [[nodiscard]] VkExtent2D GetExtent() const { return m_Extent; }

        [[nodiscard]] const std::vector<VkImage>& GetImages() const { return m_Images; }

        std::uint32_t GetImageCount() const { return GetImages().size(); }
    private:
        VkSwapchainKHR m_SwapChain = VK_NULL_HANDLE;
        VkFormat m_ImageFormat     = VK_FORMAT_UNDEFINED;
        VkExtent2D m_Extent        = {0, 0};
        std::vector<VkImage> m_Images;
    };
} // namespace Astrelis::Vulkan
