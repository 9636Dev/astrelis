#pragma once

#include "NebulaEngine/Renderer/TextureImage.hpp"

#include <vulkan/vulkan.h>

namespace Nebula::Vulkan
{
    class TextureImage : public Nebula::TextureImage
    {
    public:
        TextureImage() = default;
        ~TextureImage() override = default;
        TextureImage(const TextureImage& other) = delete;
        TextureImage& operator=(const TextureImage& other) = delete;
        TextureImage(TextureImage&& other) = delete;
        TextureImage& operator=(TextureImage&& other) = delete;

        bool LoadTexture(RefPtr<GraphicsContext>& context, InMemoryImage& image) override;
        void Destroy(RefPtr<GraphicsContext>& context) override;

        VkImage m_Image = VK_NULL_HANDLE;
        VkDeviceMemory m_ImageMemory = VK_NULL_HANDLE;
        VkImageView m_ImageView = VK_NULL_HANDLE;
    };
} // namespace Nebula::Vulkan
