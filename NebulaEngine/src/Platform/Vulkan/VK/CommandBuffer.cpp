#include "CommandBuffer.hpp"

namespace Nebula::Vulkan
{
    bool CommandBuffer::Init(LogicalDevice& device, CommandPool& pool)
    {
        VkCommandBufferAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = pool.m_CommandPool;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = 1;

        return vkAllocateCommandBuffers(device.GetHandle(), &allocInfo, &m_CommandBuffer) == VK_SUCCESS;
    }

    void CommandBuffer::Destroy(LogicalDevice& device, CommandPool& pool)
    {
        vkFreeCommandBuffers(device.GetHandle(), pool.m_CommandPool, 1, &m_CommandBuffer);
    }

    bool CommandBuffer::Begin()
    {
        VkCommandBufferBeginInfo beginInfo = {};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        return vkBeginCommandBuffer(m_CommandBuffer, &beginInfo) == VK_SUCCESS;
    }

    bool CommandBuffer::End()
    {
        return vkEndCommandBuffer(m_CommandBuffer) == VK_SUCCESS;
    }
}  // namespace Nebula::Vulkan
