#include "Instance.hpp"
#include <stdexcept>
#include <vulkan/vulkan.h>

namespace Nebula
{
    void VulkanInstance::Destroy()
    {
        if (m_Instance != VK_NULL_HANDLE)
        {
            vkDestroyInstance(m_Instance, nullptr);
        }
    }

    void VulkanInstance::Init(std::string_view appName,
                              std::string_view engineName,
                              const std::vector<const char*>& extensions,
                              const std::vector<const char*>& layers,
                              VkDebugUtilsMessengerCreateInfoEXT* debugCreateInfo)
    {
        VkApplicationInfo appInfo {};
        appInfo.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName   = appName.data();
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName        = engineName.data();
        appInfo.engineVersion      = VK_MAKE_VERSION(1, 0, 0);

        VkInstanceCreateInfo createInfo {};
        createInfo.sType            = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

        createInfo.enabledExtensionCount   = static_cast<uint32_t>(extensions.size());
        createInfo.ppEnabledExtensionNames = extensions.data();
        createInfo.enabledLayerCount       = static_cast<uint32_t>(layers.size());
        createInfo.ppEnabledLayerNames     = layers.data();
        createInfo.pNext                   = debugCreateInfo;
#ifdef __APPLE__
        createInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
#endif

        if (vkCreateInstance(&createInfo, nullptr, &m_Instance) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create Vulkan instance");
        }
    }
} // namespace Nebula
