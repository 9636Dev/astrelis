#pragma once


#include "LogicalDevice.hpp"

#include <vulkan/vulkan.h>

namespace Nebula::Vulkan
{
    class DescriptorPool
    {
    public:
        DescriptorPool() = default;
        ~DescriptorPool() = default;
        DescriptorPool(const DescriptorPool &other) = delete;
        DescriptorPool &operator=(const DescriptorPool &other) = delete;
        DescriptorPool(DescriptorPool &&other) = delete;
        DescriptorPool &operator=(DescriptorPool &&other) = delete;

        bool Init(LogicalDevice& device, std::uint32_t size);
        void Destroy(LogicalDevice& device);

        [[nodiscard]] VkDescriptorPool GetHandle() const { return m_DescriptorPool; }
    private:
        VkDescriptorPool m_DescriptorPool = VK_NULL_HANDLE;
    };
}  // namespace Nebula::Vulkan
