#pragma once

#include "NebulaEngine/Renderer/CommandPool.hpp"
#include "Platform/Vulkan/VK/LogicalDevice.hpp"

#include <vulkan/vulkan.h>

namespace Nebula::Vulkan
{
    class CommandPool : public Nebula::CommandPool
    {
    public:
        CommandPool() = default;
        ~CommandPool() override;
        CommandPool(const CommandPool&) = delete;
        CommandPool& operator=(const CommandPool&) = delete;
        CommandPool(CommandPool&&) = delete;
        CommandPool& operator=(CommandPool&&) = delete;

        bool Init(LogicalDevice& device);
        void Destroy(LogicalDevice& device);

        VkCommandPool m_CommandPool = VK_NULL_HANDLE;
    };
} // namespace Nebula::Vulkan
