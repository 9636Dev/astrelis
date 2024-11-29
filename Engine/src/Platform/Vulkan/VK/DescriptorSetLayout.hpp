#pragma once

#include "Astrelis/Renderer/BindingDescriptor.hpp"

#include <vulkan/vulkan.h>

#include "LogicalDevice.hpp"

namespace Astrelis::Vulkan {
    class DescriptorSetLayout {
    public:
        DescriptorSetLayout()                                      = default;
        ~DescriptorSetLayout()                                     = default;
        DescriptorSetLayout(const DescriptorSetLayout&)            = default;
        DescriptorSetLayout& operator=(const DescriptorSetLayout&) = default;
        DescriptorSetLayout(DescriptorSetLayout&&)                 = default;
        DescriptorSetLayout& operator=(DescriptorSetLayout&&)      = default;

        [[nodiscard]] bool Init(
            LogicalDevice& device, const std::vector<DescriptorSetBinding>& descriptors);
        void Destroy(LogicalDevice& device) const;

        VkDescriptorSetLayout m_Layout = VK_NULL_HANDLE;
    };
} // namespace Astrelis::Vulkan
