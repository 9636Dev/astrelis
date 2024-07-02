#pragma once

#include "NebulaEngine/Renderer/TextureImage.hpp"

#include "CommandPool.hpp"
#include "LogicalDevice.hpp"

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

namespace Nebula::Vulkan
{
    class TextureImage : public Nebula::TextureImage
    {
    public:
        TextureImage()                                     = default;
        ~TextureImage() override                           = default;
        TextureImage(const TextureImage& other)            = delete;
        TextureImage& operator=(const TextureImage& other) = delete;
        TextureImage(TextureImage&& other)                 = default;
        TextureImage& operator=(TextureImage&& other)      = default;

        // Internally used
        bool Init(LogicalDevice& device,
                  CommandPool& pool,
                  PhysicalDevice& physicalDevice,
                  VkExtent2D extent,
                  VkFormat format,
                  VkImageTiling tiling,
                  VkImageUsageFlags usage,
                  VkMemoryPropertyFlags properties);
        bool LoadTexture(RefPtr<GraphicsContext>& context, InMemoryImage& image) override;
        void Destroy(RefPtr<GraphicsContext>& context) override;
        void Destroy(LogicalDevice& device);

        VkImage m_Image              = VK_NULL_HANDLE;
        VkDeviceMemory m_ImageMemory = VK_NULL_HANDLE;
        VkImageView m_ImageView      = VK_NULL_HANDLE;
    };
} // namespace Nebula::Vulkan
