#include "CommandPool.hpp"

namespace Nebula::Vulkan
{
    CommandPool::~CommandPool() = default;

    bool CommandPool::Init(LogicalDevice& device)
    {
        VkCommandPoolCreateInfo poolInfo {};
        poolInfo.sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.queueFamilyIndex = device.GetQueueFamilyIndices().graphicsFamily.value();
        poolInfo.flags            = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

        return vkCreateCommandPool(device.GetHandle(), &poolInfo, nullptr, &m_CommandPool) == VK_SUCCESS;
    }

    void CommandPool::Destroy(LogicalDevice& device)
    {
        vkDestroyCommandPool(device.GetHandle(), m_CommandPool, nullptr);
    }
} // namespace Nebula::Vulkan
