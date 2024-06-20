#include "DescriptorPool.hpp"

#include <array>

namespace Nebula::Vulkan
{
    bool DescriptorPool::Init(LogicalDevice& device, std::uint32_t size)
    {
        // TODO: Make this more flexible
        std::array<VkDescriptorPoolSize, 2> poolSizes{};
        poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        poolSizes[0].descriptorCount = size;
        poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        poolSizes[1].descriptorCount = size;

        VkDescriptorPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.poolSizeCount = static_cast<std::uint32_t>(poolSizes.size());
        poolInfo.pPoolSizes = poolSizes.data();
        poolInfo.maxSets = size;
        poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;

        return static_cast<bool>(vkCreateDescriptorPool(device.GetHandle(), &poolInfo, nullptr, &m_DescriptorPool) == VK_SUCCESS);
    }

    void DescriptorPool::Destroy(LogicalDevice& device)
    {
        vkDestroyDescriptorPool(device.GetHandle(), m_DescriptorPool, nullptr);
    }
}  // namespace Nebula::Vulkan
