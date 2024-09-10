#pragma once

#include <cstdint>
#include <optional>

#include <vulkan/vulkan.h>

#include "PhysicalDevice.hpp"
#include "Surface.hpp"

namespace Astrelis::Vulkan
{
    struct QueueFamilyIndices
    {
        std::optional<std::uint32_t> graphicsFamily;
        std::optional<std::uint32_t> presentFamily;

        [[nodiscard]] bool IsComplete() const { return graphicsFamily.has_value() && presentFamily.has_value(); }
    };

    class LogicalDevice
    {
    public:
        LogicalDevice()                                = default;
        ~LogicalDevice()                               = default;
        LogicalDevice(const LogicalDevice&)            = delete;
        LogicalDevice(LogicalDevice&&)                 = delete;
        LogicalDevice& operator=(const LogicalDevice&) = delete;
        LogicalDevice& operator=(LogicalDevice&&)      = delete;

        [[nodiscard]] bool Init(PhysicalDevice& physicalDevice,
                  Surface& surface,
                  const std::vector<const char*>& deviceExtensions,
                  const std::vector<const char*>& validationLayers);
        void Destroy();

        [[nodiscard]] VkDevice GetHandle() const { return m_Device; }

        [[nodiscard]] VkQueue GetGraphicsQueue() const { return m_GraphicsQueue; }

        [[nodiscard]] VkQueue GetPresentQueue() const { return m_PresentQueue; }

        [[nodiscard]] const QueueFamilyIndices& GetQueueFamilyIndices() const { return m_QueueFamilyIndices; }
    private:
        VkDevice m_Device       = VK_NULL_HANDLE;
        VkQueue m_GraphicsQueue = VK_NULL_HANDLE;
        VkQueue m_PresentQueue  = VK_NULL_HANDLE;
        QueueFamilyIndices m_QueueFamilyIndices;

        static QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);
    };
} // namespace Astrelis::Vulkan
