#pragma once

#include <vulkan/vulkan.h>

#include "CommandPool.hpp"
#include "Fence.hpp"
#include "LogicalDevice.hpp"
#include "Semaphore.hpp"

namespace Astrelis::Vulkan
{
    class CommandBuffer
    {
    public:
        CommandBuffer()                                = default;
        ~CommandBuffer()                               = default;
        CommandBuffer(const CommandBuffer&)            = delete;
        CommandBuffer& operator=(const CommandBuffer&) = delete;
        CommandBuffer(CommandBuffer&&)                 = default;
        CommandBuffer& operator=(CommandBuffer&&)      = default;

        [[nodiscard]] bool Init(LogicalDevice& device, CommandPool& commandPool);
        void Destroy(LogicalDevice& device, CommandPool& pool);

        bool Begin() const;
        bool End() const;
        void Reset();
        bool Submit(LogicalDevice& device, VkQueue queue, Semaphore& waitSemaphore, Semaphore& signalSemaphore, Fence& fence);

        VkCommandBuffer GetHandle() const { return m_CommandBuffer; }
    private:
        VkCommandBuffer m_CommandBuffer = VK_NULL_HANDLE;
    };
} // namespace Astrelis::Vulkan
