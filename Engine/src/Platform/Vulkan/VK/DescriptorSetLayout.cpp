#include "DescriptorSetLayout.hpp"

#include "Astrelis/Core/Base.hpp"

#include "Astrelis/Core/GlobalConfig.hpp"
#include "Astrelis/Core/Log.hpp"

namespace Astrelis::Vulkan {
    static VkDescriptorType GetVkDescriptorType(DescriptorType type) {
        switch (type) {
        case DescriptorType::Uniform:
            return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        case DescriptorType::TextureSampler:
            return VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        default:
            ASTRELIS_CORE_LOG_ERROR("Unknown descriptor type!");
            return VK_DESCRIPTOR_TYPE_MAX_ENUM;
        }
    }

    static VkShaderStageFlags GetVkShaderStageFlags(DescriptorSetBinding::StageFlags flags) {
        VkShaderStageFlags stageFlags = 0;
        if ((flags & DescriptorSetBinding::StageFlags::Vertex)
            != DescriptorSetBinding::StageFlags::None) {
            stageFlags |= VK_SHADER_STAGE_VERTEX_BIT;
        }
        if ((flags & DescriptorSetBinding::StageFlags::Fragment)
            != DescriptorSetBinding::StageFlags::None) {
            stageFlags |= VK_SHADER_STAGE_FRAGMENT_BIT;
        }
        return stageFlags;
    }

    bool DescriptorSetLayout::Init(
        LogicalDevice& device, const std::vector<DescriptorSetBinding>& descriptors) {
        std::vector<VkDescriptorSetLayoutBinding> bindings;
        bindings.resize(descriptors.size());
#ifdef ASTRELIS_DEBUG
        std::set<std::uint32_t> bindingsSet;
#endif
        for (std::size_t i = 0; i < descriptors.size(); ++i) {
            VkDescriptorType type = GetVkDescriptorType(descriptors[i].Type);
            [[unlikely]] if (type == VK_DESCRIPTOR_TYPE_MAX_ENUM) {
                ASTRELIS_CORE_LOG_ERROR("Failed to get descriptor type!");
                return false;
            }
            VkShaderStageFlags stageFlags = GetVkShaderStageFlags(descriptors[i].Flags);
            [[unlikely]] if (stageFlags == 0) {
                ASTRELIS_CORE_LOG_ERROR("Failed to get shader stage flags!");
                return false;
            }

#ifdef ASTRELIS_DEBUG
            if (bindingsSet.find(descriptors[i].Binding) != bindingsSet.end()) {
                ASTRELIS_CORE_LOG_ERROR("Binding already exists!");
                return false;
            }
            bindingsSet.insert(descriptors[i].Binding);
#endif
            bindings[i].binding            = descriptors[i].Binding;
            bindings[i].descriptorType     = type;
            bindings[i].descriptorCount    = descriptors[i].Count;
            bindings[i].stageFlags         = stageFlags;
            bindings[i].pImmutableSamplers = nullptr;

            if (GlobalConfig::IsDebugMode()) {
                ASTRELIS_CORE_LOG_DEBUG(
                    "DescriptorSetLayout::Init: Binding: {0}, DescriptorType: {1}, " "DescriptorCount: {2}, StageFlags: {3}",
                    bindings[i].binding,
                    static_cast<std::underlying_type_t<VkDescriptorType>>(
                        bindings[i].descriptorType),
                    bindings[i].descriptorCount, bindings[i].stageFlags);
            }
        }


        VkDescriptorSetLayoutCreateInfo layoutInfo {};
        layoutInfo.sType        = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutInfo.bindingCount = static_cast<std::uint32_t>(bindings.size());
        layoutInfo.pBindings    = bindings.data();

        if (vkCreateDescriptorSetLayout(device.GetHandle(), &layoutInfo, nullptr, &m_Layout)
            != VK_SUCCESS) {
            ASTRELIS_CORE_LOG_ERROR("Failed to create descriptor set layout!");
            return false;
        }
        return true;
    }

    void DescriptorSetLayout::Destroy(LogicalDevice& device) const {
        vkDestroyDescriptorSetLayout(device.GetHandle(), m_Layout, nullptr);
    }
} // namespace Astrelis::Vulkan
