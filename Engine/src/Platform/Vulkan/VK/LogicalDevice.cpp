#include "LogicalDevice.hpp"

#include "Astrelis/Core/Log.hpp"

#include <set>

namespace Astrelis::Vulkan
{
    QueueFamilyIndices LogicalDevice::FindQueueFamilies(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface)
    {
        QueueFamilyIndices indices;

        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

        int index = 0;
        for (const auto& queueFamily : queueFamilies)
        {
            if ((queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0)
            {
                indices.graphicsFamily = index;
            }

            VkBool32 presentSupport = VK_FALSE;
            vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, index, surface, &presentSupport);

            if (presentSupport != 0)
            {
                indices.presentFamily = index;
            }

            if (indices.IsComplete())
            {
                break;
            }

            index++;
        }

        return indices;
    }

    bool LogicalDevice::Init(PhysicalDevice& physicalDevice,
                             Surface& surface,
                             const std::vector<const char*>& deviceExtensions,
                             const std::vector<const char*>& validationLayers)
    {
        m_QueueFamilyIndices = FindQueueFamilies(physicalDevice.GetHandle(), surface.GetHandle());

        if (!m_QueueFamilyIndices.IsComplete())
        {
            ASTRELIS_CORE_LOG_ERROR("Failed to find required queue families!");
            return false;
        }

        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        std::set<uint32_t> uniqueQueueFamilies = {m_QueueFamilyIndices.graphicsFamily.value(),
                                                  m_QueueFamilyIndices.presentFamily.value()};

        float queuePriority = 1.0F;
        for (uint32_t queueFamily : uniqueQueueFamilies)
        {
            VkDeviceQueueCreateInfo queueCreateInfo {};
            queueCreateInfo.sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = queueFamily;
            queueCreateInfo.queueCount       = 1;
            queueCreateInfo.pQueuePriorities = &queuePriority;
            queueCreateInfos.push_back(queueCreateInfo);
        }

        VkPhysicalDeviceFeatures deviceFeatures {};
        deviceFeatures.samplerAnisotropy = VK_TRUE;

        VkDeviceCreateInfo createInfo {};
        createInfo.sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.queueCreateInfoCount    = static_cast<uint32_t>(queueCreateInfos.size());
        createInfo.pQueueCreateInfos       = queueCreateInfos.data();
        createInfo.pEnabledFeatures        = &deviceFeatures;
        createInfo.enabledExtensionCount   = static_cast<uint32_t>(deviceExtensions.size());
        createInfo.ppEnabledExtensionNames = deviceExtensions.data();
        createInfo.enabledLayerCount       = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames     = validationLayers.data();

        if (vkCreateDevice(physicalDevice.GetHandle(), &createInfo, nullptr, &m_Device) != VK_SUCCESS)
        {
            ASTRELIS_CORE_LOG_ERROR("Failed to create logical device!");
            return false;
        }

        vkGetDeviceQueue(m_Device, m_QueueFamilyIndices.graphicsFamily.value(), 0, &m_GraphicsQueue);
        vkGetDeviceQueue(m_Device, m_QueueFamilyIndices.presentFamily.value(), 0, &m_PresentQueue);

        return true;
    }

    void LogicalDevice::Destroy() { vkDestroyDevice(m_Device, nullptr); }

} // namespace Astrelis::Vulkan
