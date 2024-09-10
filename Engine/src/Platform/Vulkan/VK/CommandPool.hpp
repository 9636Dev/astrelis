#pragma once

#include <vulkan/vulkan.h>

#include "LogicalDevice.hpp"

namespace Astrelis::Vulkan
{
    class CommandPool
    {
    public:
        CommandPool() = default;
        ~CommandPool() = default;
        CommandPool(const CommandPool&) = delete;
        CommandPool& operator=(const CommandPool&) = delete;
        CommandPool(CommandPool&&) = default;
        CommandPool& operator=(CommandPool&&) = default;

        [[nodiscard]] bool Init(LogicalDevice& device);
        void Destroy(LogicalDevice& device);

        VkCommandPool GetHandle() const { return m_CommandPool; }
    private:
        VkCommandPool m_CommandPool = VK_NULL_HANDLE;
    };
} // namespace Astrelis::Vulkan
