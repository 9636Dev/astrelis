#include "DescriptorSetLayout.hpp"

#include "NebulaEngine/Core/Log.hpp"

namespace Nebula::Vulkan
{
    bool DescriptorSetLayout::Init(LogicalDevice& device, std::vector<UniformDescriptor>& uniformDescriptors, std::vector<SamplerDescriptor>& samplerDescriptors)
    {
        std::vector<VkDescriptorSetLayoutBinding> bindings;
        bindings.resize(uniformDescriptors.size() + samplerDescriptors.size());
        for (std::size_t i = 0; i < uniformDescriptors.size(); ++i)
        {
            bindings[i].binding            = uniformDescriptors[i].Binding;
            bindings[i].descriptorType     = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            bindings[i].descriptorCount    = uniformDescriptors[i].Count;
            bindings[i].stageFlags         = VK_SHADER_STAGE_VERTEX_BIT;
            bindings[i].pImmutableSamplers = nullptr;
        }

        for (std::size_t i = 0; i < samplerDescriptors.size(); ++i)
        {
            bindings[uniformDescriptors.size() + i].binding            = samplerDescriptors[i].Binding;
            bindings[uniformDescriptors.size() + i].descriptorType     = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            bindings[uniformDescriptors.size() + i].descriptorCount    = samplerDescriptors[i].Count;
            bindings[uniformDescriptors.size() + i].stageFlags         = VK_SHADER_STAGE_FRAGMENT_BIT;
            bindings[uniformDescriptors.size() + i].pImmutableSamplers = nullptr;
        }

        VkDescriptorSetLayoutCreateInfo layoutInfo {};
        layoutInfo.sType        = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutInfo.bindingCount = static_cast<std::uint32_t>(bindings.size());
        layoutInfo.pBindings    = bindings.data();

        if (vkCreateDescriptorSetLayout(device.GetHandle(), &layoutInfo, nullptr, &m_Layout) != VK_SUCCESS)
        {
            NEBULA_CORE_LOG_ERROR("Failed to create descriptor set layout!");
            return false;
        }
        return true;
    }

    void DescriptorSetLayout::Destroy(LogicalDevice& device) const
    {
        vkDestroyDescriptorSetLayout(device.GetHandle(), m_Layout, nullptr);
    }
} // namespace Nebula::Vulkan
