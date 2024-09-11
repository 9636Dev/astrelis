#pragma once

#include "LogicalDevice.hpp"

#include <vulkan/vulkan.h>

namespace Astrelis::Vulkan
{
    struct DescriptorPoolSize
    {
        VkDescriptorType type;
        std::uint32_t descriptorCount;
    };

    struct DescriptorPoolCreateInfo
    {
        std::vector<DescriptorPoolSize> poolSizes;
        std::uint32_t maxSets = 0;
        VkDescriptorPoolCreateFlags flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
    };

    class DescriptorPool
    {
    public:
        DescriptorPool() = default;
        ~DescriptorPool() = default;
        DescriptorPool(const DescriptorPool &other) = delete;
        DescriptorPool &operator=(const DescriptorPool &other) = delete;
        DescriptorPool(DescriptorPool &&other) = delete;
        DescriptorPool &operator=(DescriptorPool &&other) = delete;

        [[nodiscard]] bool Init(LogicalDevice& device, const DescriptorPoolCreateInfo& createInfo);
        void Destroy(LogicalDevice& device);

        [[nodiscard]] VkDescriptorPool GetHandle() const { return m_DescriptorPool; }
    private:
        VkDescriptorPool m_DescriptorPool = VK_NULL_HANDLE;
    };
}  // namespace Astrelis::Vulkan
