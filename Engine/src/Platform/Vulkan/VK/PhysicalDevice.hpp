#pragma once

#include <cstdint>
#include <functional>
#include <utility>
#include <vulkan/vulkan.h>

namespace Astrelis::Vulkan {
    class PhysicalDevice {
    public:
        PhysicalDevice()                                 = default;
        ~PhysicalDevice()                                = default;
        PhysicalDevice(const PhysicalDevice&)            = delete;
        PhysicalDevice(PhysicalDevice&&)                 = delete;
        PhysicalDevice& operator=(const PhysicalDevice&) = delete;
        PhysicalDevice& operator=(PhysicalDevice&&)      = delete;


        /**
         * @brief Picks the best device based on the evaluator function.
         * @param instance The Vulkan instance.
         * @param surface The Vulkan surface.
         * @param support The device support.
         * @note The function will modify the support object to include only the present optional extensions.
         */
        void PickBestDevice(VkInstance instance, VkSurfaceKHR surface);

        void SetEvaluator(std::function<int(VkPhysicalDevice, VkSurfaceKHR)> evaluator) {
            m_Evaluator = std::move(evaluator);
        }

        VkPhysicalDevice GetHandle() const {
            return m_PhysicalDevice;
        }

        [[nodiscard]] bool IsFound() const {
            return m_PhysicalDevice != VK_NULL_HANDLE;
        }

        VkSampleCountFlagBits GetMaxUsableSampleCount();
    private:
        static std::int32_t RateDevice(VkPhysicalDevice device, VkSurfaceKHR surface);
        std::function<int(VkPhysicalDevice, VkSurfaceKHR)> m_Evaluator      = RateDevice;
        VkPhysicalDevice                                   m_PhysicalDevice = VK_NULL_HANDLE;
    };
} // namespace Astrelis::Vulkan
