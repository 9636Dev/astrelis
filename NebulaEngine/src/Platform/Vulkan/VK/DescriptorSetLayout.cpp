#include "DescriptorSetLayout.hpp"

#include "NebulaEngine/Core/Log.hpp"

namespace Nebula::Vulkan
{
    bool DescriptorSetLayout::Init(LogicalDevice& device, std::vector<UniformDescriptor>& uniformDescriptors)
    {
        std::vector<VkDescriptorSetLayoutBinding> bindings;
        bindings.resize(uniformDescriptors.size());
        for (std::size_t i = 0; i < uniformDescriptors.size(); ++i)
        {
            bindings[i].binding            = uniformDescriptors[i].Binding;
            bindings[i].descriptorType     = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            bindings[i].descriptorCount    = uniformDescriptors[i].Count;
            bindings[i].stageFlags         = VK_SHADER_STAGE_VERTEX_BIT;
            bindings[i].pImmutableSamplers = nullptr;
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
