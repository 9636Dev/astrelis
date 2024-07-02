#include "DescriptorSetLayout.hpp"

#include "NebulaEngine/Core/Log.hpp"
#include "Platform/Vulkan/VulkanGraphicsContext.hpp"

namespace Nebula::Vulkan
{
    bool DescriptorSetLayout::Init(LogicalDevice& device, const std::vector<BindingDescriptor>& descriptors)
    {
        std::vector<VkDescriptorSetLayoutBinding> bindings;
        bindings.resize(descriptors.size());
        for (std::size_t i = 0; i < descriptors.size(); ++i)
        {
            VkDescriptorType type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            VkShaderStageFlags stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

            if (descriptors[i].DescriptorType == BindingDescriptor::Type::Uniform)
            {
                type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
            }
            else if (descriptors[i].DescriptorType == BindingDescriptor::Type::TextureSampler)
            {
                type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
            }
            else
            {
                NEBULA_CORE_LOG_ERROR("Unknown descriptor type!");
                return false;
            }

            bindings[i].binding            = descriptors[i].Binding;
            bindings[i].descriptorType     = type;
            bindings[i].descriptorCount    = descriptors[i].Count;
            bindings[i].stageFlags         = stageFlags;
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

    bool DescriptorSetLayout::Init(RefPtr<GraphicsContext>& context, const std::vector<BindingDescriptor>& descriptors)
    {
        return Init(context.As<VulkanGraphicsContext>()->m_LogicalDevice, descriptors);
    }

    void DescriptorSetLayout::Destroy(LogicalDevice& device) const
    {
        vkDestroyDescriptorSetLayout(device.GetHandle(), m_Layout, nullptr);
    }

    void DescriptorSetLayout::Destroy(RefPtr<GraphicsContext>& context) const
    {
        Destroy(context.As<VulkanGraphicsContext>()->m_LogicalDevice);
    }
} // namespace Nebula::Vulkan
