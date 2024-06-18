#include "PhysicalDevice.hpp"
#include "NebulaEngine/Core/Log.hpp"

#include <algorithm>

namespace Nebula::Vulkan
{
    std::int32_t PhysicalDevice::RateDevice(VkPhysicalDevice device)
    {
        VkPhysicalDeviceProperties deviceProperties;
        vkGetPhysicalDeviceProperties(device, &deviceProperties);

        VkPhysicalDeviceFeatures deviceFeatures;
        vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

        int score = 0;

        score += static_cast<std::int32_t>(deviceProperties.limits.maxImageDimension2D);

        if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
        {
            score += 1'000;
        }

        return score;
    }

    void PhysicalDevice::PickBestDevice(VkInstance instance)
    {
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

        if (deviceCount == 0)
        {
            NEBULA_CORE_LOG_ERROR("Failed to find GPUs with Vulkan support!");
            return;
        }

        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());
        std::vector<std::pair<VkPhysicalDevice, int>> deviceScores;
        deviceScores.reserve(deviceCount);
        for (VkPhysicalDevice device : devices)
        {
            deviceScores.emplace_back(device, RateDevice(device));
        }

        std::sort(deviceScores.begin(), deviceScores.end(),
                  [](const auto& deviceA, const auto& deviceB) { return deviceA.second > deviceB.second; });

        m_PhysicalDevice = deviceScores[0].first;
    }
} // namespace Nebula::Vulkan
