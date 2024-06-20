#pragma once


#include "LogicalDevice.hpp"

#include <cstdint>
#include <vulkan/vulkan.h>

namespace Nebula::Vulkan
{
    class Fence
    {
    public:
        Fence() = default;
        ~Fence() = default;
        Fence(const Fence&) = delete;
        Fence& operator=(const Fence&) = delete;
        Fence(Fence&&) = default;
        Fence& operator=(Fence&&) = default;

        bool Init(LogicalDevice& device);
        void Destroy(LogicalDevice& device);

        void Wait(LogicalDevice& device, std::uint64_t timeout);
        void Reset(LogicalDevice& device);

        VkFence GetHandle() const { return m_Fence; }

        VkFence m_Fence = VK_NULL_HANDLE;
    };
} // namespace Nebula::Vulkan
