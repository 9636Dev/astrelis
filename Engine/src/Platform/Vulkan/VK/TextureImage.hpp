#pragma once

#include "Astrelis/Renderer/TextureImage.hpp"

#include <vulkan/vulkan.h>

#include "CommandPool.hpp"
#include "ImageView.hpp"
#include "LogicalDevice.hpp"

namespace Astrelis::Vulkan {
    class TextureImage : public Astrelis::TextureImage {
    public:
        TextureImage()                                     = default;
        ~TextureImage() override                           = default;
        TextureImage(const TextureImage& other)            = default;
        TextureImage& operator=(const TextureImage& other) = default;
        TextureImage(TextureImage&& other)                 = default;
        TextureImage& operator=(TextureImage&& other)      = default;

        // Internally used
        [[nodiscard]] bool Init(LogicalDevice& device, CommandPool& pool,
            PhysicalDevice& physicalDevice, VkExtent2D extent, VkFormat format,
            VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties);
        bool LoadTexture(RefPtr<GraphicsContext>& context, InMemoryImage& image) override;
        void Destroy(RefPtr<GraphicsContext>& context) override;
        void Destroy(LogicalDevice& device);

        [[nodiscard]] const VkImage& GetImage() const {
            return m_Image;
        }

        [[nodiscard]] const VkImageView& GetImageView() const {
            return m_ImageView.GetHandle();
        }
    private:
        VkImage        m_Image       = VK_NULL_HANDLE;
        VkDeviceMemory m_ImageMemory = VK_NULL_HANDLE;
        ImageView      m_ImageView;
    };
} // namespace Astrelis::Vulkan
