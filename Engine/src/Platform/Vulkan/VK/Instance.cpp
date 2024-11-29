#include "Instance.hpp"

#include "Astrelis/Core/Base.hpp"

#include "Astrelis/Core/Log.hpp"

namespace Astrelis::Vulkan {
    void Instance::Destroy() {
        if (m_Instance != VK_NULL_HANDLE) {
            vkDestroyInstance(m_Instance, nullptr);
        }
    }

    bool Instance::Init(CString appName, Version appVersion, Version apiVersion,
        const std::vector<CString>& extensions, const std::vector<CString>& layers) {
        VkApplicationInfo appInfo {};
        appInfo.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName   = appName;
        appInfo.applicationVersion = appVersion.VulkanVersion;
        appInfo.pEngineName        = "Astrelis";
        appInfo.engineVersion      = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion         = apiVersion.VulkanVersion;

        VkInstanceCreateInfo createInfo {};
        createInfo.sType            = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

        createInfo.enabledExtensionCount   = static_cast<uint32_t>(extensions.size());
        createInfo.ppEnabledExtensionNames = extensions.data();
        createInfo.enabledLayerCount       = static_cast<uint32_t>(layers.size());
        createInfo.ppEnabledLayerNames     = layers.data();
#ifdef __APPLE__
        createInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
#endif

        if (vkCreateInstance(&createInfo, nullptr, &m_Instance) != VK_SUCCESS) {
            ASTRELIS_CORE_LOG_ERROR("Failed to create Vulkan instance!");
            return false;
        }
        return true;
    }
} // namespace Astrelis::Vulkan
