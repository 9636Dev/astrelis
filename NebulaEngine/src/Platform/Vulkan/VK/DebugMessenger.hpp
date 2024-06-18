#pragma once

#include <vulkan/vulkan.h>

#include "Instance.hpp"

namespace Nebula::Vulkan
{
    class DebugMessenger
    {
    public:
        DebugMessenger() = default;
        ~DebugMessenger()                                = default;
        DebugMessenger(const DebugMessenger&)            = delete;
        DebugMessenger& operator=(const DebugMessenger&) = delete;
        DebugMessenger(DebugMessenger&&)                 = delete;
        DebugMessenger& operator=(DebugMessenger&&)      = delete;

        [[nodiscard]] bool Init(Instance& instance);
        void Destroy(Instance& instance);
    private:
        VkDebugUtilsMessengerEXT m_DebugMessenger = VK_NULL_HANDLE;
    };

} // namespace Nebula::Vulkan
