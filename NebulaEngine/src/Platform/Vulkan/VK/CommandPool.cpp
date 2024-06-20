#include "CommandPool.hpp"

#include "NebulaEngine/Core/Log.hpp"

namespace Nebula::Vulkan
{
    bool CommandPool::Init(LogicalDevice& device)
    {
        VkCommandPoolCreateInfo poolInfo {};
        poolInfo.sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.queueFamilyIndex = device.GetQueueFamilyIndices().graphicsFamily.value();
        poolInfo.flags            = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

        if (vkCreateCommandPool(device.GetHandle(), &poolInfo, nullptr, &m_CommandPool) != VK_SUCCESS)
        {
            NEBULA_CORE_LOG_ERROR("Failed to create command pool!");
            return false;
        }
        return true;
    }

    void CommandPool::Destroy(LogicalDevice& device)
    {
        vkDestroyCommandPool(device.GetHandle(), m_CommandPool, nullptr);
    }
} // namespace Nebula::Vulkan
