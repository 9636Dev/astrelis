#include "DebugMessenger.hpp"

#include <stdexcept>

#include "NebulaEngine/Core/Log.hpp"
#include "VulkanExt.hpp"

namespace Nebula::Vulkan
{
    bool DebugMessenger::Init(Instance& instance)
    {
        VkDebugUtilsMessengerCreateInfoEXT createInfo = {};
        PopulateDebugMessengerCreateInfo(createInfo);

        if (CreateDebugUtilsMessengerEXT(instance.GetHandle(), &createInfo, nullptr, &m_DebugMessenger) != VK_SUCCESS)
        {
            NEBULA_CORE_LOG_ERROR("Failed to set up debug messenger");
            return false;
        }
        return true;
    }

    void DebugMessenger::Destroy(Instance& instance)
    {
        DestroyDebugUtilsMessengerEXT(instance.GetHandle(), m_DebugMessenger, nullptr);
    }
} // namespace Nebula::Vulkan
