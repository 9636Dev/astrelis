#pragma once

#include "NebulaEngine/Renderer/CommandBuffer.hpp"

#include "CommandPool.hpp"
#include "LogicalDevice.hpp"

namespace Nebula::Vulkan
{
    class CommandBuffer : public Nebula::CommandBuffer
    {
    public:
        CommandBuffer()                                = default;
        ~CommandBuffer() override                      = default;
        CommandBuffer(const CommandBuffer&)            = delete;
        CommandBuffer& operator=(const CommandBuffer&) = delete;
        CommandBuffer(CommandBuffer&&)                 = delete;
        CommandBuffer& operator=(CommandBuffer&&)      = delete;

        bool Init(LogicalDevice& device, CommandPool& commandPool);
        void Destroy(LogicalDevice& device, CommandPool& pool);

        bool Begin() override;
        bool End() override;

        VkCommandBuffer m_CommandBuffer = VK_NULL_HANDLE;
    };
} // namespace Nebula::Vulkan
