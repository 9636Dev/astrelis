#include "PhysicalDevice.hpp"

#include "Astrelis/Core/Log.hpp"

#include <algorithm>

#include "Utils.hpp"

namespace Astrelis::Vulkan
{
    std::int32_t PhysicalDevice::RateDevice(VkPhysicalDevice device, VkSurfaceKHR surface)
    {
        VkPhysicalDeviceProperties deviceProperties;
        vkGetPhysicalDeviceProperties(device, &deviceProperties);

        VkPhysicalDeviceFeatures deviceFeatures;
        vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

        int score = 0;

        SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(device, surface);
        bool swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();


        VkPhysicalDeviceFeatures supportedFeatures;
        vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

        // TODO: Max anisotropy optional
        if (supportedFeatures.samplerAnisotropy != VK_TRUE || !swapChainAdequate)
        {
            return 0;
        }

        score += static_cast<std::int32_t>(deviceProperties.limits.maxImageDimension2D);

        if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
        {
            score += 1'000;
        }

        return score;
    }

    void PhysicalDevice::PickBestDevice(VkInstance instance, VkSurfaceKHR surface)
    {
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

        if (deviceCount == 0)
        {
            ASTRELIS_CORE_LOG_ERROR("Failed to find GPUs with Vulkan support!");
            return;
        }

        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());
        std::vector<std::pair<VkPhysicalDevice, int>> deviceScores;
        deviceScores.reserve(deviceCount);
        for (VkPhysicalDevice device : devices)
        {
            deviceScores.emplace_back(device, RateDevice(device, surface));
        }

        std::sort(deviceScores.begin(), deviceScores.end(),
                  [](const auto& deviceA, const auto& deviceB) { return deviceA.second > deviceB.second; });

        m_PhysicalDevice = deviceScores[0].first;
    }
} // namespace Astrelis::Vulkan
