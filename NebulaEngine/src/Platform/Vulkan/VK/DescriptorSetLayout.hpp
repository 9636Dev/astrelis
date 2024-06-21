#pragma once

#include "NebulaEngine/Renderer/DescriptorSetLayout.hpp"

#include "LogicalDevice.hpp"

#include <vulkan/vulkan.h>

namespace Nebula::Vulkan
{
    class DescriptorSetLayout : public Nebula::DescriptorSetLayout
    {
    public:
        DescriptorSetLayout()                                      = default;
        ~DescriptorSetLayout() override                            = default;
        DescriptorSetLayout(const DescriptorSetLayout&)            = delete;
        DescriptorSetLayout& operator=(const DescriptorSetLayout&) = delete;
        DescriptorSetLayout(DescriptorSetLayout&&)                 = delete;
        DescriptorSetLayout& operator=(DescriptorSetLayout&&)      = delete;

        [[nodiscard]] bool Init(LogicalDevice& device, std::vector<UniformDescriptor>& uniformDescriptors, std::vector<SamplerDescriptor>& samplerDescriptors);
        void Destroy(LogicalDevice& device) const;

        VkDescriptorSetLayout m_Layout = VK_NULL_HANDLE;
    };
} // namespace Nebula::Vulkan
