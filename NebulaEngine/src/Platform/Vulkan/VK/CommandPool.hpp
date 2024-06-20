#pragma once

#include "LogicalDevice.hpp"

#include <vulkan/vulkan.h>

namespace Nebula::Vulkan
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
} // namespace Nebula::Vulkan
