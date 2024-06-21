#include "TextureSampler.hpp"

#include "NebulaEngine/Core/Log.hpp"

#include "Platform/Vulkan/VulkanGraphicsContext.hpp"

namespace Nebula::Vulkan
{
    bool TextureSampler::Init(RefPtr<GraphicsContext>& context)
    {
        auto ctx = context.As<VulkanGraphicsContext>();
        VkSamplerCreateInfo samplerInfo{};
        samplerInfo.sType        = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        samplerInfo.magFilter    = VK_FILTER_LINEAR;
        samplerInfo.minFilter    = VK_FILTER_LINEAR;
        samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.anisotropyEnable = VK_TRUE;
        VkPhysicalDeviceProperties properties{};
        vkGetPhysicalDeviceProperties(ctx->m_PhysicalDevice.GetHandle(), &properties);
        samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
        samplerInfo.borderColor   = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
        samplerInfo.unnormalizedCoordinates = VK_FALSE;
        samplerInfo.compareEnable           = VK_FALSE;
        samplerInfo.compareOp               = VK_COMPARE_OP_ALWAYS;
        samplerInfo.mipmapMode              = VK_SAMPLER_MIPMAP_MODE_LINEAR;
        samplerInfo.mipLodBias              = 0.0F;
        samplerInfo.minLod                  = 0.0F;
        samplerInfo.maxLod                  = 0.0F;

        if (vkCreateSampler(ctx->m_LogicalDevice.GetHandle(), &samplerInfo, nullptr, &m_Sampler) != VK_SUCCESS)
        {
            NEBULA_CORE_LOG_ERROR("Failed to create texture sampler");
            return false;
        }

        return true;
    }

    void TextureSampler::Destroy(RefPtr<GraphicsContext>& context)
    {
        vkDestroySampler(context.As<VulkanGraphicsContext>()->m_LogicalDevice.GetHandle(), m_Sampler, nullptr);
    }
}  // namespace Nebula::Vulkan
