#pragma once

#include "Astrelis/Core/Pointer.hpp"
#include "Astrelis/Core/Utils/PlatformDetection.hpp"

#include <cstdint>
#include <string>
#include <vector>
#include <vulkan/vulkan.h>

struct GLFWwindow;

namespace Astrelis::Vulkan
{
    inline const std::vector<const char*>& GetValidationLayers()
    {
        static const std::vector<const char*> layers = {
            "VK_LAYER_KHRONOS_validation",
        };

        return layers;
    }

    inline const std::vector<const char*>& GetDeviceExtensions()
    {
        static const std::vector<const char*> extensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME,
#ifdef ASTRELIS_PLATFORM_MACOS
                                                            "VK_KHR_portability_subset"
#endif
        };

        return extensions;
    }

    bool CheckValidationLayerSupport();
    std::vector<const char*> GetRequiredExtensions(bool enableValidationLayers);

    VkCommandBuffer BeginSingleTimeCommands(VkDevice logicalDevice, VkCommandPool commandPool);
    void EndSingleTimeCommands(VkDevice logicalDevice,
                               VkQueue queue,
                               VkCommandPool commandPool,
                               VkCommandBuffer commandBuffer);
    void TransitionImageLayout(VkCommandBuffer commandBuffer,
                               VkImage image,
                               VkFormat format,
                               VkImageLayout oldLayout,
                               VkImageLayout newLayout);
    void TransitionImageLayout(VkDevice logicalDevice,
                               VkQueue queue,
                               VkCommandPool commandPool,
                               VkImage image,
                               VkFormat format,
                               VkImageLayout oldLayout,
                               VkImageLayout newLayout);

    bool CreateBuffer(VkPhysicalDevice physicalDevice,
                      VkDevice logicalDevice,
                      VkDeviceSize size,
                      VkBufferUsageFlags usage,
                      VkMemoryPropertyFlags properties,
                      VkBuffer& buffer,
                      VkDeviceMemory& bufferMemory);
    bool CopyBuffer(VkDevice logicalDevice,
                    VkQueue queue,
                    VkCommandPool commandPool,
                    VkBuffer srcBuffer,
                    VkBuffer dstBuffer,
                    VkDeviceSize size);
    std::uint32_t
        FindMemoryType(VkPhysicalDevice physicalDevice, std::uint32_t typeFilter, VkMemoryPropertyFlags properties);

    bool CreateImage(VkPhysicalDevice physicalDevice,
                     VkDevice logicalDevice,
                     std::uint32_t width,
                     std::uint32_t height,
                     VkFormat format,
                     VkImageTiling tiling,
                     VkImageUsageFlags usage,
                     VkMemoryPropertyFlags properties,
                     VkImage& image,
                     VkDeviceMemory& imageMemory);

    void CopyBufferToImage(VkDevice logicalDevice,
                           VkQueue queue,
                           VkCommandPool commandPool,
                           VkBuffer buffer,
                           VkImage image,
                           std::uint32_t width,
                           std::uint32_t height);

    struct SwapChainSupportDetails
    {
        VkSurfaceCapabilitiesKHR capabilities = {};
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;

        VkSurfaceFormatKHR ChooseSwapSurfaceFormat()
        {
            for (const auto& availableFormat : formats)
            {
                if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
                    availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
                {
                    return availableFormat;
                }
            }

            return formats[0];
        }

        VkPresentModeKHR ChooseSwapPresentMode()
        {
            for (const auto& availablePresentMode : presentModes)
            {
                if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
                {
                    return availablePresentMode;
                }
            }

            return VK_PRESENT_MODE_FIFO_KHR;
        }

        VkExtent2D ChooseExtent(const RawRef<GLFWwindow*>& window) const;
    };

    SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);

    class Utils
    {
    public:
        Utils() = delete;

        static std::string VkFormatToString(VkFormat format);
        static std::string VkColorSpaceToString(VkColorSpaceKHR colorSpace);
        static std::string VkPresentModeToString(VkPresentModeKHR presentMode);
    };
} // namespace Astrelis::Vulkan
