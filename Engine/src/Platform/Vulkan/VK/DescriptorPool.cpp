#include "DescriptorPool.hpp"
#include "Astrelis/Core/Base.hpp"

namespace Astrelis::Vulkan
{
    bool DescriptorPool::Init(LogicalDevice& device, const DescriptorPoolCreateInfo& createInfo)
    {
        std::vector<VkDescriptorPoolSize> poolSizes(createInfo.poolSizes.size());
        for (std::size_t i = 0; i < createInfo.poolSizes.size(); ++i)
        {
            poolSizes[i].type            = createInfo.poolSizes[i].type;
            poolSizes[i].descriptorCount = createInfo.poolSizes[i].descriptorCount;
        }

        VkDescriptorPoolCreateInfo poolInfo {};
        poolInfo.sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.poolSizeCount = static_cast<std::uint32_t>(poolSizes.size());
        poolInfo.pPoolSizes    = poolSizes.data();
        poolInfo.maxSets       = createInfo.maxSets;
        poolInfo.flags         = createInfo.flags;

        if (vkCreateDescriptorPool(device.GetHandle(), &poolInfo, nullptr, &m_DescriptorPool) != VK_SUCCESS)
        {
            ASTRELIS_CORE_LOG_ERROR("Failed to create descriptor pool!");
            return false;
        }
        return true;
    }

    void DescriptorPool::Destroy(LogicalDevice& device)
    {
        vkDestroyDescriptorPool(device.GetHandle(), m_DescriptorPool, nullptr);
    }
} // namespace Astrelis::Vulkan
