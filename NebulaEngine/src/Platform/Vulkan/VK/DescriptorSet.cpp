#include "DescriptorSet.hpp"

#include "NebulaEngine/Core/Log.hpp"
#include "NebulaEngine/Renderer/GraphicsPipeline.hpp"
#include "Platform/Vulkan/VK/GraphicsPipeline.hpp"
#include "Platform/Vulkan/VulkanGraphicsContext.hpp"

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
            NEBULA_CORE_LOG_ERROR("Failed to allocate descriptor set!");
            return false;
        }

        VkDescriptorBufferInfo bufferInfo {};
        bufferInfo.buffer = info.Buffer;
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

    void DescriptorSet::Bind(RefPtr<GraphicsContext>& context, RefPtr<Nebula::GraphicsPipeline>& pipeline) const
    {
        auto vkContext = context.As<VulkanGraphicsContext>();
        vkCmdBindDescriptorSets(vkContext->GetCurrentFrame().CommandBuffer.GetHandle(), VK_PIPELINE_BIND_POINT_GRAPHICS,
                                pipeline.As<GraphicsPipeline>()->m_PipelineLayout, 0, 1, &m_DescriptorSet, 0, nullptr);
    }

    void DescriptorSet::Destroy(LogicalDevice& device, DescriptorPool& pool) const
    {
        vkFreeDescriptorSets(device.GetHandle(), pool.GetHandle(), 1, &m_DescriptorSet);
    }
} // namespace Nebula::Vulkan
