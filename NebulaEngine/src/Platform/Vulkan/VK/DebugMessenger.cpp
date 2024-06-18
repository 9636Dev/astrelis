#include "DebugMessenger.hpp"

#include <stdexcept>

#include "VulkanExt.hpp"

namespace Nebula
{
    DebugMessenger::DebugMessenger(VkInstance instance) : m_DebugMessenger(VK_NULL_HANDLE)
    {
        VkDebugUtilsMessengerCreateInfoEXT createInfo = {};
        PopulateDebugMessengerCreateInfo(createInfo);

        if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &m_DebugMessenger) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to set up debug messenger");
        }
    }

    void DebugMessenger::Destroy(VkInstance instance)
    {
        DestroyDebugUtilsMessengerEXT(instance, m_DebugMessenger, nullptr);
    }
} // namespace Nebula
