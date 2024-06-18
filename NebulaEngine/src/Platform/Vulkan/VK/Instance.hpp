#pragma once

#include <string_view>
#include <vector>
#include <vulkan/vulkan.h>

namespace Nebula
{
    class VulkanInstance
    {
    public:
        VulkanInstance()                                 = default;
        ~VulkanInstance()                                = default;
        VulkanInstance(const VulkanInstance&)            = delete;
        VulkanInstance& operator=(const VulkanInstance&) = delete;
        VulkanInstance(VulkanInstance&&)                 = delete;
        VulkanInstance& operator=(VulkanInstance&&)      = delete;

        void Init(std::string_view appName,
                  std::string_view engineName,
                  const std::vector<const char*>& extensions,
                  const std::vector<const char*>& layers,
                  VkDebugUtilsMessengerCreateInfoEXT* debugCreateInfo = nullptr);

        void Destroy();

        // NOLINTNEXTLINE(google-explicit-constructor, hicpp-explicit-conversions)
        operator VkInstance() const { return m_Instance; }

        VkInstance GetHandle() const { return m_Instance; }
    private:
        VkInstance m_Instance = VK_NULL_HANDLE;
    };
} // namespace Nebula
