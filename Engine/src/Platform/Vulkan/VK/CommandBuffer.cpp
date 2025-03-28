#include "CommandBuffer.hpp"

#include "Astrelis/Core/Base.hpp"

#include "Fence.hpp"
#include "Semaphore.hpp"

namespace Astrelis::Vulkan {
    bool CommandBuffer::Init(LogicalDevice& device, CommandPool& pool) {
        ASTRELIS_CORE_ASSERT(
            m_CommandBuffer == VK_NULL_HANDLE, "Command buffer is not null handle");
        VkCommandBufferAllocateInfo allocInfo = {};
        allocInfo.sType                       = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool                 = pool.GetHandle();
        allocInfo.level                       = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount          = 1;

        if (vkAllocateCommandBuffers(device.GetHandle(), &allocInfo, &m_CommandBuffer)
            != VK_SUCCESS) {
            ASTRELIS_CORE_LOG_ERROR("Failed to allocate command buffer!");
        }

        return true;
    }

    void CommandBuffer::Destroy(LogicalDevice& device, CommandPool& pool) {
        ASTRELIS_CORE_ASSERT(m_CommandBuffer != VK_NULL_HANDLE, "Command buffer is null handle");
        vkFreeCommandBuffers(device.GetHandle(), pool.GetHandle(), 1, &m_CommandBuffer);
    }

    bool CommandBuffer::Begin() const {
        VkCommandBufferBeginInfo beginInfo = {};
        beginInfo.sType                    = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags                    = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        return vkBeginCommandBuffer(m_CommandBuffer, &beginInfo) == VK_SUCCESS;
    }

    bool CommandBuffer::End() const {
        return vkEndCommandBuffer(m_CommandBuffer) == VK_SUCCESS;
    }

    void CommandBuffer::Reset() {
        vkResetCommandBuffer(m_CommandBuffer, VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT);
    }

    bool CommandBuffer::Submit(LogicalDevice& device, VkQueue queue, Semaphore& waitSemaphore,
        Semaphore& signalSemaphore, Fence& fence) {
        ASTRELIS_UNUSED(device);
        VkSubmitInfo submitInfo = {};
        submitInfo.sType        = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        std::array<VkSemaphore, 1> waitSemaphores = {waitSemaphore.GetHandle()};
        // TODO: Make this generic, since there could be stencil buffer, depth buffer, etc.
        std::array<VkPipelineStageFlags, 1> waitStages = {
            VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};

        submitInfo.waitSemaphoreCount = static_cast<uint32_t>(waitSemaphores.size());
        submitInfo.pWaitSemaphores    = waitSemaphores.data();
        submitInfo.pWaitDstStageMask  = waitStages.data();
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers    = &m_CommandBuffer;

        std::array<VkSemaphore, 1> signal = {signalSemaphore.GetHandle()};
        submitInfo.signalSemaphoreCount   = static_cast<uint32_t>(signal.size());
        submitInfo.pSignalSemaphores      = signal.data();

        return vkQueueSubmit(queue, 1, &submitInfo, fence.GetHandle()) == VK_SUCCESS;
    }
} // namespace Astrelis::Vulkan
