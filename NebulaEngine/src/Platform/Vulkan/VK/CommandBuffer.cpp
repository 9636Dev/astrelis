#include "CommandBuffer.hpp"

#include <array>

#include "Fence.hpp"
#include "Platform/Vulkan/VulkanGraphicsContext.hpp"
#include "Semaphore.hpp"

namespace Nebula::Vulkan
{
    bool CommandBuffer::Init(LogicalDevice& device, CommandPool& pool)
    {
        VkCommandBufferAllocateInfo allocInfo = {};
        allocInfo.sType                       = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool                 = pool.m_CommandPool;
        allocInfo.level                       = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount          = 1;

        return vkAllocateCommandBuffers(device.GetHandle(), &allocInfo, &m_CommandBuffer) == VK_SUCCESS;
    }

    void CommandBuffer::Destroy(LogicalDevice& device, CommandPool& pool)
    {
        vkFreeCommandBuffers(device.GetHandle(), pool.m_CommandPool, 1, &m_CommandBuffer);
    }

    bool CommandBuffer::Begin()
    {
        VkCommandBufferBeginInfo beginInfo = {};
        beginInfo.sType                    = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags                    = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        return vkBeginCommandBuffer(m_CommandBuffer, &beginInfo) == VK_SUCCESS;
    }

    bool CommandBuffer::End() { return vkEndCommandBuffer(m_CommandBuffer) == VK_SUCCESS; }

    void CommandBuffer::Reset()
    {
        vkResetCommandBuffer(m_CommandBuffer, VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT);
    }

    bool CommandBuffer::Submit(RefPtr<GraphicsContext>& context,
                               RefPtr<Nebula::Semaphore>& imageAvailableSemaphore,
                               RefPtr<Nebula::Semaphore>& finishedRenderingSemaphore,
                               RefPtr<Nebula::Fence>& inFlightFence)
    {
        VkSubmitInfo submitInfo = {};
        submitInfo.sType        = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        std::array<VkSemaphore, 1> waitSemaphores      = {imageAvailableSemaphore.As<Semaphore>()->m_Semaphore};
        std::array<VkPipelineStageFlags, 1> waitStages = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};

        submitInfo.waitSemaphoreCount = static_cast<uint32_t>(waitSemaphores.size());
        submitInfo.pWaitSemaphores    = waitSemaphores.data();
        submitInfo.pWaitDstStageMask  = waitStages.data();
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers    = &m_CommandBuffer;

        std::array<VkSemaphore, 1> signal = {finishedRenderingSemaphore.As<Semaphore>()->m_Semaphore};
        submitInfo.signalSemaphoreCount   = static_cast<uint32_t>(signal.size());
        submitInfo.pSignalSemaphores      = signal.data();

        auto ctx = context.As<VulkanGraphicsContext>();
        return vkQueueSubmit(ctx->m_LogicalDevice.GetGraphicsQueue(), 1, &submitInfo,
                             inFlightFence.As<Fence>()->m_Fence) == VK_SUCCESS;
    }
} // namespace Nebula::Vulkan
