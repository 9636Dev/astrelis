#pragma once

#include "NebulaEngine/Renderer/TextureImage.hpp"

#include "CommandPool.hpp"
#include "LogicalDevice.hpp"
#include "ImageView.hpp"

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
        TextureImage(TextureImage&& other)                 = delete;
        TextureImage& operator=(TextureImage&& other)      = delete;

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

        [[nodiscard]] const VkImage& GetImage() const { return m_Image; }
        [[nodiscard]] const VkImageView& GetImageView() const { return m_ImageView.GetHandle(); }
    private:
        VkImage m_Image              = VK_NULL_HANDLE;
        VkDeviceMemory m_ImageMemory = VK_NULL_HANDLE;
        ImageView m_ImageView;
    };
} // namespace Nebula::Vulkan
