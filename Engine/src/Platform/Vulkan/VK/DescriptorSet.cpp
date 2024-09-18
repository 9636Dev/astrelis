#include "DescriptorSet.hpp"

#include "Astrelis/Core/Log.hpp"

#include <array>

#include "GraphicsPipeline.hpp"
#include "Platform/Vulkan/VK/LogicalDevice.hpp"
#include "TextureImage.hpp"
#include "TextureSampler.hpp"
#include "UniformBuffer.hpp"

namespace Astrelis::Vulkan {
    bool DescriptorSet::Init(LogicalDevice& device, DescriptorPool& pool,
        DescriptorSetLayout& layout, const std::vector<DescriptorSetBinding>& descriptors,
        std::uint32_t setIndex) {
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
            const auto& descriptor      = descriptors[i];
            auto&       descriptorWrite = descriptorWrites[i];

            descriptorWrite.sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrite.dstSet          = m_DescriptorSet;
            descriptorWrite.dstBinding      = descriptor.Binding;
            descriptorWrite.dstArrayElement = 0;
            descriptorWrite.descriptorCount = descriptor.Count;

            if (!descriptor.Uniforms.empty()) {
                ASTRELIS_CORE_ASSERT(descriptor.Uniforms.size() > setIndex,
                    "Uniform buffer descriptor does not have enough elements!");
                VkDescriptorBufferInfo& bufferInfo = bufferInfos[i];
                bufferInfo.buffer                  = descriptor.Uniforms[setIndex]
                                        .Buffer.As<UniformBuffer*>()
                                        ->m_Buffers[0]
                                        .m_Buffer;
                bufferInfo.range  = descriptor.Size;
                bufferInfo.offset = 0; // TODO: Add offset support

                descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                descriptorWrite.pBufferInfo    = &bufferInfo;
            }
            else if (!descriptor.Textures.empty()) {
                ASTRELIS_CORE_ASSERT(descriptor.Textures.size() > setIndex,
                    "Texture descriptor does not have enough elements!");
                VkDescriptorImageInfo& imageInfo = imageInfos[i];
                imageInfo.imageLayout            = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                imageInfo.imageView =
                    descriptor.Textures[setIndex].Image.As<TextureImage*>()->GetImageView();
                imageInfo.sampler =
                    descriptor.Textures[setIndex].Sampler.As<TextureSampler*>()->m_Sampler;

                descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                descriptorWrite.pImageInfo     = &imageInfo;
            }
            else {
                ASTRELIS_CORE_LOG_ERROR("Unknown descriptor type!");
                return false;
            }
        }

        vkUpdateDescriptorSets(device.GetHandle(),
            static_cast<std::uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0,
            nullptr);
        return true;
    }

    void DescriptorSet::Bind(CommandBuffer& buffer, GraphicsPipeline& pipeline) const {
        vkCmdBindDescriptorSets(buffer.GetHandle(), VK_PIPELINE_BIND_POINT_GRAPHICS,
            pipeline.m_PipelineLayout, 0, 1, &m_DescriptorSet, 0, nullptr);
    }

    void DescriptorSet::Destroy(
        LogicalDevice& logicalDevice, DescriptorPool& descriptorPool) const {
        vkFreeDescriptorSets(
            logicalDevice.GetHandle(), descriptorPool.GetHandle(), 1, &m_DescriptorSet);
    }
} // namespace Astrelis::Vulkan
