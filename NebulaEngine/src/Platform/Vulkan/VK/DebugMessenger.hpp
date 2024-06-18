#pragma once

#include <vulkan/vulkan.h>

namespace Nebula
{
class DebugMessenger
{
public:
    explicit DebugMessenger(VkInstance instance);
    ~DebugMessenger() = default;
    DebugMessenger(const DebugMessenger&) = delete;
    DebugMessenger& operator=(const DebugMessenger&) = delete;
    DebugMessenger(DebugMessenger&&) = delete;
    DebugMessenger& operator=(DebugMessenger&&) = delete;

    void Destroy(VkInstance instance);

private:
    VkDebugUtilsMessengerEXT m_DebugMessenger;
};

}  // namespace Nebula
