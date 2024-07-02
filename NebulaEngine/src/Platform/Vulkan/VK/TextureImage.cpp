#include "TextureImage.hpp"

#include "NebulaEngine/Core/Assert.hpp"

#include "Platform/Vulkan/VulkanGraphicsContext.hpp"
#include "Utils.hpp"

namespace Nebula::Vulkan
{
        bool TextureImage::Init(LogicalDevice& device,
                  CommandPool& pool,
                  PhysicalDevice& physicalDevice,
                  VkExtent2D extent,
                  VkFormat format,
                  VkImageTiling tiling,
                  VkImageUsageFlags usage,
                  VkMemoryPropertyFlags properties)
    {
        // TODO: Pool to transition image layout
        (void)pool;
        CreateImage(physicalDevice.GetHandle(), device.GetHandle(), extent.width, extent.height, format, tiling, usage, properties, m_Image, m_ImageMemory);

        VkImageViewCreateInfo viewInfo = {};
        viewInfo.sType                 = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewInfo.image                 = m_Image;
        viewInfo.viewType              = VK_IMAGE_VIEW_TYPE_2D;
        viewInfo.format                = format;
        viewInfo.subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
        viewInfo.subresourceRange.baseMipLevel   = 0;
        viewInfo.subresourceRange.levelCount     = 1;
        viewInfo.subresourceRange.baseArrayLayer = 0;
        viewInfo.subresourceRange.layerCount     = 1;

        if (vkCreateImageView(device.GetHandle(), &viewInfo, nullptr, &m_ImageView) != VK_SUCCESS)
        {
            NEBULA_CORE_LOG_ERROR("Failed to create texture image view!");
            return false;
        }

        return true;
    }

    bool TextureImage::LoadTexture(RefPtr<GraphicsContext>& context, InMemoryImage& image)
    {
        auto ctx = context.As<VulkanGraphicsContext>();

        VkBuffer stagingBuffer             = VK_NULL_HANDLE;
        VkDeviceMemory stagingBufferMemory = VK_NULL_HANDLE;

        // TODO: Maybe check to ensure that channels are 4
        VkDeviceSize imageSize = 4UL * image.GetWidth() * image.GetHeight();
        NEBULA_CORE_ASSERT(imageSize == image.GetData().size(), "Image size does not match data size!");

        CreateBuffer(ctx->m_PhysicalDevice.GetHandle(), ctx->m_LogicalDevice.GetHandle(), imageSize,
                     VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                     VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer,
                     stagingBufferMemory);

        vkBindBufferMemory(ctx->m_LogicalDevice.GetHandle(), stagingBuffer, stagingBufferMemory, 0);
        {
            void* data = nullptr;
            vkMapMemory(ctx->m_LogicalDevice.GetHandle(), stagingBufferMemory, 0, imageSize, 0, &data);
            std::memcpy(data, image.GetData().data(), static_cast<std::size_t>(imageSize));
            vkUnmapMemory(ctx->m_LogicalDevice.GetHandle(), stagingBufferMemory);
        }

        CreateImage(ctx->m_PhysicalDevice.GetHandle(), ctx->m_LogicalDevice.GetHandle(), image.GetWidth(),
                    image.GetHeight(), VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL,
                    VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                    m_Image, m_ImageMemory);

        TransitionImageLayout(ctx->m_LogicalDevice.GetHandle(), ctx->m_LogicalDevice.GetGraphicsQueue(), ctx->m_CommandPool.GetHandle(), m_Image,
                              VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
        CopyBufferToImage(ctx->m_LogicalDevice.GetHandle(), ctx->m_LogicalDevice.GetGraphicsQueue(), ctx->m_CommandPool.GetHandle(), stagingBuffer, m_Image, image.GetWidth(), image.GetHeight());
        TransitionImageLayout(ctx->m_LogicalDevice.GetHandle(), ctx->m_LogicalDevice.GetGraphicsQueue(), ctx->m_CommandPool.GetHandle(), m_Image,
                              VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

        vkDestroyBuffer(ctx->m_LogicalDevice.GetHandle(), stagingBuffer, nullptr);
        vkFreeMemory(ctx->m_LogicalDevice.GetHandle(), stagingBufferMemory, nullptr);

        VkImageViewCreateInfo viewInfo = {};
        viewInfo.sType                 = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewInfo.image                 = m_Image;
        viewInfo.viewType              = VK_IMAGE_VIEW_TYPE_2D;
        viewInfo.format                = VK_FORMAT_R8G8B8A8_SRGB;
        viewInfo.subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
        viewInfo.subresourceRange.baseMipLevel   = 0;
        viewInfo.subresourceRange.levelCount     = 1;
        viewInfo.subresourceRange.baseArrayLayer = 0;
        viewInfo.subresourceRange.layerCount     = 1;

        if (vkCreateImageView(ctx->m_LogicalDevice.GetHandle(), &viewInfo, nullptr, &m_ImageView) != VK_SUCCESS)
        {
            NEBULA_CORE_LOG_ERROR("Failed to create texture image view!");
            return false;
        }

        return true;
    }

    void TextureImage::Destroy(RefPtr<GraphicsContext>& context)
    {
        Destroy(context.As<VulkanGraphicsContext>()->m_LogicalDevice);
    }

    void TextureImage::Destroy(LogicalDevice& device)
    {
        vkDestroyImageView(device.GetHandle(), m_ImageView, nullptr);
        vkDestroyImage(device.GetHandle(), m_Image, nullptr);
        vkFreeMemory(device.GetHandle(), m_ImageMemory, nullptr);
    }
} // namespace Nebula::Vulkan

