#include "DescriptorSet.hpp"

namespace Nebula::Vulkan
{
    bool DescriptorSet::Init(LogicalDevice& device,
                            DescriptorPool& pool,
                             VkDescriptorSetLayout layout,
                             DescriptorSetInfo& info)
    {
        VkDescriptorSetAllocateInfo allocInfo {};
        allocInfo.sType              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool     = pool.GetHandle();
        allocInfo.descriptorSetCount = 1;
        allocInfo.pSetLayouts        = &layout;

        if (vkAllocateDescriptorSets(device.GetHandle(), &allocInfo, &m_DescriptorSet) != VK_SUCCESS)
        {
            return false;
        }

        VkDescriptorBufferInfo bufferInfo {};
        bufferInfo.buffer = info.UniformBuffer.GetBuffer();
        bufferInfo.offset = info.Offset;
        bufferInfo.range  = info.Size;

        // TODO: Image descriptors

        std::array<VkWriteDescriptorSet, 1> descriptorWrites {};
        descriptorWrites[0].sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[0].dstSet          = m_DescriptorSet;
        descriptorWrites[0].dstBinding      = info.Binding;
        descriptorWrites[0].dstArrayElement = 0;
        descriptorWrites[0].descriptorType  = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptorWrites[0].descriptorCount = 1;
        descriptorWrites[0].pBufferInfo     = &bufferInfo;


        vkUpdateDescriptorSets(device.GetHandle(), static_cast<std::uint32_t>(descriptorWrites.size()),
                               descriptorWrites.data(), 0, nullptr);
        return true;
    }

    void DescriptorSet::Destroy(LogicalDevice& device, DescriptorPool& pool) const
    {
        vkFreeDescriptorSets(device.GetHandle(), pool.GetHandle(), 1, &m_DescriptorSet);
    }
} // namespace Nebula::Vulkan
