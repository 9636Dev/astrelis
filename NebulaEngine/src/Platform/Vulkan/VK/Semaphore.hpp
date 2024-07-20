#pragma once

#include <vulkan/vulkan.h>

#include "LogicalDevice.hpp"

namespace Nebula::Vulkan
{
    class Semaphore
    {
    public:
        Semaphore()                            = default;
        ~Semaphore()                           = default;
        Semaphore(const Semaphore&)            = delete;
        Semaphore& operator=(const Semaphore&) = delete;
        Semaphore(Semaphore&&)                 = default;
        Semaphore& operator=(Semaphore&&)      = default;

        [[nodiscard]] bool Init(LogicalDevice& device);
        void Destroy(LogicalDevice& device);

        VkSemaphore GetHandle() const { return m_Semaphore; }
    private:
        VkSemaphore m_Semaphore = VK_NULL_HANDLE;
    };
} // namespace Nebula::Vulkan
