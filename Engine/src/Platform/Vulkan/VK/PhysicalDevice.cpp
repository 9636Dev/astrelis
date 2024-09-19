#include "PhysicalDevice.hpp"

#include "Astrelis/Core/GlobalConfig.hpp"
#include "Astrelis/Core/Log.hpp"

#include <algorithm>

#include "Utils.hpp"

namespace Astrelis::Vulkan {
    std::int32_t PhysicalDevice::RateDevice(VkPhysicalDevice device, VkSurfaceKHR surface) {
        VkPhysicalDeviceProperties deviceProperties;
        vkGetPhysicalDeviceProperties(device, &deviceProperties);

        VkPhysicalDeviceFeatures deviceFeatures;
        vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

        int score = 0;

        SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(device, surface);
        if (swapChainSupport.formats.empty() || swapChainSupport.presentModes.empty()) {
            return 0;
        }


        VkPhysicalDeviceFeatures supportedFeatures;
        vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

        if (supportedFeatures.samplerAnisotropy == VK_FALSE) {
            // TODO(Feature): We provide information to the context which features are missing and the context will decide what to do.
            return 0;
        }

        score += static_cast<std::int32_t>(deviceProperties.limits.maxImageDimension2D);

        if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
            score += 1'000;
        }

        ASTRELIS_CORE_LOG_INFO("Device: {} - {}", deviceProperties.deviceName, score);
        return score;
    }

    void PhysicalDevice::PickBestDevice(VkInstance instance, VkSurfaceKHR surface) {
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

        if (deviceCount == 0) {
            ASTRELIS_CORE_LOG_ERROR("Failed to find GPUs with Vulkan support!");
            return;
        }

        std::vector<VkPhysicalDevice> devices(deviceCount);
        if (GlobalConfig::IsDebugMode()) {
            ASTRELIS_CORE_LOG_INFO("Found {} devices with Vulkan support", deviceCount);
        }

        vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());
        std::vector<std::pair<VkPhysicalDevice, int>> deviceScores;
        deviceScores.reserve(deviceCount);
        for (VkPhysicalDevice device : devices) {
            deviceScores.emplace_back(device, RateDevice(device, surface));
        }

        std::sort(
            deviceScores.begin(), deviceScores.end(), [](const auto& deviceA, const auto& deviceB) {
                return deviceA.second > deviceB.second;
            });

        m_PhysicalDevice = deviceScores[0].first;
    }

    VkSampleCountFlagBits PhysicalDevice::GetMaxUsableSampleCount() {
        VkPhysicalDeviceProperties physicalDeviceProperties;
        vkGetPhysicalDeviceProperties(m_PhysicalDevice, &physicalDeviceProperties);

        VkSampleCountFlags counts = physicalDeviceProperties.limits.framebufferColorSampleCounts
            & physicalDeviceProperties.limits.framebufferDepthSampleCounts;

        if ((counts & VK_SAMPLE_COUNT_64_BIT) != 0) {
            return VK_SAMPLE_COUNT_64_BIT;
        }
        if ((counts & VK_SAMPLE_COUNT_32_BIT) != 0) {
            return VK_SAMPLE_COUNT_32_BIT;
        }
        if ((counts & VK_SAMPLE_COUNT_16_BIT) != 0) {
            return VK_SAMPLE_COUNT_16_BIT;
        }
        if ((counts & VK_SAMPLE_COUNT_8_BIT) != 0) {
            return VK_SAMPLE_COUNT_8_BIT;
        }
        if ((counts & VK_SAMPLE_COUNT_4_BIT) != 0) {
            return VK_SAMPLE_COUNT_4_BIT;
        }
        if ((counts & VK_SAMPLE_COUNT_2_BIT) != 0) {
            return VK_SAMPLE_COUNT_2_BIT;
        }

        return VK_SAMPLE_COUNT_1_BIT;
    }

} // namespace Astrelis::Vulkan
