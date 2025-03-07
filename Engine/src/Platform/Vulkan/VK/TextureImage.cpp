#include "TextureImage.hpp"

#include "Astrelis/Core/Base.hpp"

#include "Platform/Vulkan/VulkanGraphicsContext.hpp"
#include "Utils.hpp"

namespace Astrelis::Vulkan {
    bool TextureImage::Init(LogicalDevice& device, CommandPool& pool,
        PhysicalDevice& physicalDevice, VkExtent2D extent, VkFormat format, VkImageTiling tiling,
        VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkSampleCountFlagBits samples) {
        // TODO: Pool to transition image layout
        (void)pool;
        CreateImage(physicalDevice.GetHandle(), device.GetHandle(), extent.width, extent.height,
            format, tiling, usage, properties, samples, m_Image, m_ImageMemory);

        return m_ImageView.Init(device, m_Image, format);
    }

    bool TextureImage::LoadTexture(RefPtr<GraphicsContext>& context, InMemoryImage& image) {
        auto ctx = context.As<VulkanGraphicsContext>();

        // Destroy
        m_ImageView.Destroy(ctx->m_LogicalDevice);

        VkBuffer       stagingBuffer       = VK_NULL_HANDLE;
        VkDeviceMemory stagingBufferMemory = VK_NULL_HANDLE;

        ASTRELIS_REQUIRE(image.GetData().size() > 0, "Image data is empty!");
        ASTRELIS_REQUIRE(image.GetChannels() == 4, "Image channels are not 4!");
        VkDeviceSize imageSize = 4UL * image.GetWidth() * image.GetHeight();
        ASTRELIS_CORE_ASSERT(
            imageSize == image.GetData().size(), "Image size does not match data size!");

        CreateBuffer(ctx->m_PhysicalDevice.GetHandle(), ctx->m_LogicalDevice.GetHandle(), imageSize,
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            stagingBuffer, stagingBufferMemory);

        vkBindBufferMemory(ctx->m_LogicalDevice.GetHandle(), stagingBuffer, stagingBufferMemory, 0);
        {
            void* data = nullptr;
            vkMapMemory(
                ctx->m_LogicalDevice.GetHandle(), stagingBufferMemory, 0, imageSize, 0, &data);
            std::memcpy(data, image.GetData().data(), static_cast<std::size_t>(imageSize));
            vkUnmapMemory(ctx->m_LogicalDevice.GetHandle(), stagingBufferMemory);
        }

        CreateImage(ctx->m_PhysicalDevice.GetHandle(), ctx->m_LogicalDevice.GetHandle(),
            image.GetWidth(), image.GetHeight(), VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL,
            VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, ctx->m_MSAASamples, m_Image, m_ImageMemory);

        TransitionImageLayout(ctx->m_LogicalDevice.GetHandle(),
            ctx->m_LogicalDevice.GetGraphicsQueue(), ctx->m_CommandPool.GetHandle(), m_Image,
            VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
        CopyBufferToImage(ctx->m_LogicalDevice.GetHandle(), ctx->m_LogicalDevice.GetGraphicsQueue(),
            ctx->m_CommandPool.GetHandle(), stagingBuffer, m_Image, image.GetWidth(),
            image.GetHeight());
        TransitionImageLayout(ctx->m_LogicalDevice.GetHandle(),
            ctx->m_LogicalDevice.GetGraphicsQueue(), ctx->m_CommandPool.GetHandle(), m_Image,
            VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

        vkDestroyBuffer(ctx->m_LogicalDevice.GetHandle(), stagingBuffer, nullptr);
        vkFreeMemory(ctx->m_LogicalDevice.GetHandle(), stagingBufferMemory, nullptr);

        return m_ImageView.Init(ctx->m_LogicalDevice, m_Image, VK_FORMAT_R8G8B8A8_SRGB);
    }

    void TextureImage::Destroy(RefPtr<GraphicsContext>& context) {
        Destroy(context.As<VulkanGraphicsContext>()->m_LogicalDevice);
    }

    void TextureImage::Destroy(LogicalDevice& device) {
        m_ImageView.Destroy(device);
        vkDestroyImage(device.GetHandle(), m_Image, nullptr);
        vkFreeMemory(device.GetHandle(), m_ImageMemory, nullptr);
    }
} // namespace Astrelis::Vulkan
