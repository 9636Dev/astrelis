#include "TextureSampler.hpp"

#include "Astrelis/Core/Log.hpp"

#include "Platform/Vulkan/VulkanGraphicsContext.hpp"

namespace Astrelis::Vulkan {

    bool TextureSampler::Init(LogicalDevice& device, PhysicalDevice& physicalDevice) {
        VkSamplerCreateInfo samplerInfo {};
        samplerInfo.sType            = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        samplerInfo.magFilter        = VK_FILTER_LINEAR;
        samplerInfo.minFilter        = VK_FILTER_LINEAR;
        samplerInfo.addressModeU     = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.addressModeV     = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.addressModeW     = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.anisotropyEnable = VK_TRUE;
        VkPhysicalDeviceProperties properties {};
        vkGetPhysicalDeviceProperties(physicalDevice.GetHandle(), &properties);
        samplerInfo.maxAnisotropy           = properties.limits.maxSamplerAnisotropy;
        samplerInfo.borderColor             = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
        samplerInfo.unnormalizedCoordinates = VK_FALSE;
        samplerInfo.compareEnable           = VK_FALSE;
        samplerInfo.compareOp               = VK_COMPARE_OP_ALWAYS;
        samplerInfo.mipmapMode              = VK_SAMPLER_MIPMAP_MODE_LINEAR;
        samplerInfo.mipLodBias              = 0.0F;
        samplerInfo.minLod                  = 0.0F;
        samplerInfo.maxLod                  = 0.0F;

        if (vkCreateSampler(device.GetHandle(), &samplerInfo, nullptr, &m_Sampler) != VK_SUCCESS) {
            ASTRELIS_CORE_LOG_ERROR("Failed to create texture sampler");
            return false;
        }

        return true;
    }

    bool TextureSampler::Init(RefPtr<GraphicsContext>& context) {
        auto ctx = context.As<VulkanGraphicsContext>();
        return Init(ctx->m_LogicalDevice, ctx->m_PhysicalDevice);
    }

    void TextureSampler::Destroy(RefPtr<GraphicsContext>& context) {
        vkDestroySampler(
            context.As<VulkanGraphicsContext>()->m_LogicalDevice.GetHandle(), m_Sampler, nullptr);
    }
} // namespace Astrelis::Vulkan
