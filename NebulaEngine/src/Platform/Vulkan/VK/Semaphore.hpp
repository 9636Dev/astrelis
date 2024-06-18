#pragma once

#include "NebulaEngine/Renderer/Semaphore.hpp"
#include "Platform/Vulkan/VK/LogicalDevice.hpp"

#include <vulkan/vulkan.h>

namespace Nebula::Vulkan
{
    class Semaphore : public Nebula::Semaphore
    {
    public:
        Semaphore() = default;
        ~Semaphore() override = default;
        Semaphore(const Semaphore&) = delete;
        Semaphore& operator=(const Semaphore&) = delete;
        Semaphore(Semaphore&&) = delete;
        Semaphore& operator=(Semaphore&&) = delete;

        bool Init(LogicalDevice& device);
        void Destroy(LogicalDevice& device);

        VkSemaphore m_Semaphore = VK_NULL_HANDLE;
    };
} // namespace Nebula::Vulkan
