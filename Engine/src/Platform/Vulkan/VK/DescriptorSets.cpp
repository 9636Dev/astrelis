#include "DescriptorSets.hpp"

#include "Astrelis/Core/Log.hpp"
#include "Astrelis/Renderer/GraphicsPipeline.hpp"

#include <array>

#include "GraphicsPipeline.hpp"
#include "Platform/Vulkan/VulkanGraphicsContext.hpp"
#include "TextureImage.hpp"
#include "TextureSampler.hpp"
#include "UniformBuffer.hpp"

namespace Astrelis::Vulkan {
    bool DescriptorSets::Init(LogicalDevice& device, DescriptorPool& pool,
        DescriptorSetLayout& layout, const std::vector<BindingDescriptor>& descriptors) {
        std::array<VkDescriptorSetLayout, 1> layouts = {layout.m_Layout};

        VkDescriptorSetAllocateInfo allocInfo {};
        allocInfo.sType              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool     = pool.GetHandle();
        allocInfo.descriptorSetCount = static_cast<std::uint32_t>(layouts.size());
        allocInfo.pSetLayouts        = layouts.data();

        if (vkAllocateDescriptorSets(device.GetHandle(), &allocInfo, &m_DescriptorSet)
            != VK_SUCCESS) {
            ASTRELIS_CORE_LOG_ERROR("Failed to allocate descriptor set!");
            return false;
        }

        std::vector<VkWriteDescriptorSet> descriptorWrites;
        // We need it to persist until the end of the scope
        std::vector<VkDescriptorBufferInfo> bufferInfos;
        std::vector<VkDescriptorImageInfo>  imageInfos;

        // We reserve enough for all bufferinfos and imageinfos, to prevent errors when resizing
        descriptorWrites.resize(descriptors.size());
        bufferInfos.resize(descriptors.size());
        imageInfos.resize(descriptors.size());

        for (std::size_t i = 0; i < descriptorWrites.size(); i++) {
            auto&       descriptorWrite     = descriptorWrites[i];
            const auto& descriptor          = descriptors[i];
            descriptorWrite.sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrite.dstSet          = m_DescriptorSet;
            descriptorWrite.dstBinding      = descriptors[i].Binding;
            descriptorWrite.dstArrayElement = 0;
            descriptorWrite.descriptorCount = descriptors[i].Count;

            switch (descriptor.DescriptorType) {
            case BindingDescriptor::Type::Uniform:
                {
                    VkDescriptorBufferInfo& bufferInfo = bufferInfos[i];
                    bufferInfo.buffer =
                        descriptor.Buffer.As<UniformBuffer>()->m_Buffers[0].m_Buffer;
                    bufferInfo.range  = descriptor.Size;
                    bufferInfo.offset = 0; // TODO: Add offset support

                    descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                    descriptorWrite.pBufferInfo    = &bufferInfo;
                    break;
                }
            case BindingDescriptor::Type::TextureSampler:
                {
                    VkDescriptorImageInfo& imageInfo = imageInfos[i];
                    imageInfo.imageLayout            = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                    imageInfo.imageView = descriptor.Texture.As<TextureImage>()->GetImageView();
                    imageInfo.sampler   = descriptor.Sampler.As<TextureSampler>()->m_Sampler;

                    descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                    descriptorWrite.pImageInfo     = &imageInfo;
                    break;
                }
            default:
                ASTRELIS_CORE_LOG_ERROR("Unknown descriptor type!");
                return false;
            }
        }

        vkUpdateDescriptorSets(device.GetHandle(),
            static_cast<std::uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0,
            nullptr);
        return true;
    }

    bool DescriptorSets::Init(RefPtr<GraphicsContext>& context,
        RefPtr<Astrelis::DescriptorSetLayout>&         layout,
        const std::vector<BindingDescriptor>&          descriptors) {
        auto ctx = context.As<VulkanGraphicsContext>();
        return Init(ctx->m_LogicalDevice, ctx->m_DescriptorPool, *layout.As<DescriptorSetLayout>(),
            descriptors);
    }

    void DescriptorSets::Bind(CommandBuffer& buffer, GraphicsPipeline& pipeline) const {
        vkCmdBindDescriptorSets(buffer.GetHandle(), VK_PIPELINE_BIND_POINT_GRAPHICS,
            pipeline.m_PipelineLayout, 0, 1, &m_DescriptorSet, 0, nullptr);
    }

    void DescriptorSets::Bind(
        RefPtr<GraphicsContext>& context, RefPtr<Astrelis::GraphicsPipeline>& pipeline) const {
        auto vkContext = context.As<VulkanGraphicsContext>();
        vkCmdBindDescriptorSets(vkContext->GetCurrentFrame().CommandBuffer.GetHandle(),
            VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.As<GraphicsPipeline>()->m_PipelineLayout, 0,
            1, &m_DescriptorSet, 0, nullptr);
    }

    void DescriptorSets::Destroy(RefPtr<GraphicsContext>& context) const {
        auto ctx = context.As<VulkanGraphicsContext>();
        vkFreeDescriptorSets(ctx->m_LogicalDevice.GetHandle(), ctx->m_DescriptorPool.GetHandle(), 1,
            &m_DescriptorSet);
    }
} // namespace Astrelis::Vulkan
