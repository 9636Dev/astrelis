#include "DescriptorSets.hpp"

#include "NebulaEngine/Core/Log.hpp"
#include "NebulaEngine/Renderer/GraphicsPipeline.hpp"
#include "Platform/Vulkan/VulkanGraphicsContext.hpp"

#include "GraphicsPipeline.hpp"
#include "TextureImage.hpp"
#include "TextureSampler.hpp"
#include "UniformBuffer.hpp"

#include <array>

namespace Nebula::Vulkan
{
    bool DescriptorSets::Init(LogicalDevice& device,
                              DescriptorPool& pool,
                              DescriptorSetLayout& layout,
                              const std::vector<BindingDescriptor>& descriptors)
    {
        std::array<VkDescriptorSetLayout, 1> layouts = {layout.m_Layout};

        VkDescriptorSetAllocateInfo allocInfo {};
        allocInfo.sType              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool     = pool.GetHandle();
        allocInfo.descriptorSetCount = static_cast<std::uint32_t>(layouts.size());
        allocInfo.pSetLayouts        = layouts.data();

        if (vkAllocateDescriptorSets(device.GetHandle(), &allocInfo, &m_DescriptorSet) != VK_SUCCESS)
        {
            NEBULA_CORE_LOG_ERROR("Failed to allocate descriptor set!");
            return false;
        }

        std::vector<VkWriteDescriptorSet> descriptorWrites;
        descriptorWrites.resize(descriptors.size());

        for (std::size_t i = 0; i < descriptors.size(); ++i)
        {
            descriptorWrites[i].sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrites[i].dstSet          = m_DescriptorSet;
            descriptorWrites[i].dstBinding      = descriptors[i].Binding;
            descriptorWrites[i].dstArrayElement = 0;
            descriptorWrites[i].descriptorCount = descriptors[i].Count;

            if (descriptors[i].DescriptorType == BindingDescriptor::Type::Uniform)
            {
                descriptorWrites[i].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                VkDescriptorBufferInfo bufferInfo {};
                bufferInfo.buffer = descriptors[i].Buffer.As<UniformBuffer>()->m_Buffers[0].m_Buffer;
                bufferInfo.range  = descriptors[i].Size;
                bufferInfo.offset = 0; // TODO: Add offset support

                descriptorWrites[i].pBufferInfo = &bufferInfo;
            }
            else if (descriptors[i].DescriptorType == BindingDescriptor::Type::TextureSampler)
            {
                descriptorWrites[i].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                VkDescriptorImageInfo imageInfo {};
                imageInfo.imageLayout          = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                imageInfo.imageView            = descriptors[i].Texture.As<TextureImage>()->m_ImageView;
                imageInfo.sampler              = descriptors[i].Sampler.As<TextureSampler>()->m_Sampler;
                descriptorWrites[i].pImageInfo = &imageInfo;
            }
            else
            {
                NEBULA_CORE_LOG_ERROR("Unknown descriptor type!");
                return false;
            }
        }

        vkUpdateDescriptorSets(device.GetHandle(), static_cast<std::uint32_t>(descriptorWrites.size()),
                               descriptorWrites.data(), 0, nullptr);
        return true;
    }

    bool DescriptorSets::Init(RefPtr<GraphicsContext>& context,
                              RefPtr<Nebula::DescriptorSetLayout>& layout,
                              const std::vector<BindingDescriptor>& descriptors)
    {
        auto ctx = context.As<VulkanGraphicsContext>();
        return Init(ctx->m_LogicalDevice, ctx->m_DescriptorPool, *layout.As<DescriptorSetLayout>(), descriptors);
    }

    void DescriptorSets::Bind(CommandBuffer& buffer, GraphicsPipeline& pipeline) const
    {
        vkCmdBindDescriptorSets(buffer.GetHandle(), VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.m_PipelineLayout, 0, 1,
                                &m_DescriptorSet, 0, nullptr);
    }

    void DescriptorSets::Bind(RefPtr<GraphicsContext>& context, RefPtr<Nebula::GraphicsPipeline>& pipeline) const
    {
        auto vkContext = context.As<VulkanGraphicsContext>();
        vkCmdBindDescriptorSets(vkContext->GetCurrentFrame().CommandBuffer.GetHandle(), VK_PIPELINE_BIND_POINT_GRAPHICS,
                                pipeline.As<GraphicsPipeline>()->m_PipelineLayout, 0, 1, &m_DescriptorSet, 0, nullptr);
    }

    void DescriptorSets::Destroy(RefPtr<GraphicsContext>& context) const
    {
        auto ctx = context.As<VulkanGraphicsContext>();
        vkFreeDescriptorSets(ctx->m_LogicalDevice.GetHandle(), ctx->m_DescriptorPool.GetHandle(), 1, &m_DescriptorSet);
    }
} // namespace Nebula::Vulkan
