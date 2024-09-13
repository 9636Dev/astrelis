#include "DebugMessenger.hpp"

#include "Astrelis/Core/Log.hpp"

#include "VulkanExt.hpp"

namespace Astrelis::Vulkan
{
    bool DebugMessenger::Init(Instance& instance)
    {
        VkDebugUtilsMessengerCreateInfoEXT createInfo = {};
        Ext::PopulateDebugMessengerCreateInfo(createInfo);

        if (Ext::CreateDebugUtilsMessengerEXT(instance.GetHandle(), &createInfo, nullptr, &m_DebugMessenger) !=
            VK_SUCCESS)
        {
            ASTRELIS_CORE_LOG_ERROR("Failed to set up debug messenger");
            return false;
        }
        return true;
    }

    void DebugMessenger::Destroy(Instance& instance)
    {
        Ext::DestroyDebugUtilsMessengerEXT(instance.GetHandle(), m_DebugMessenger, nullptr);
    }
} // namespace Astrelis::Vulkan
