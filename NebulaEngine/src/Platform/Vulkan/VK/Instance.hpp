#pragma once

#include <cstdint>
#include <string_view>
#include <vector>
#include <vulkan/vulkan.h>

namespace Nebula::Vulkan
{
    struct APIVersion
    {
        std::uint32_t major = 1;
        std::uint32_t minor = 0;

        APIVersion() = default;

        APIVersion(std::uint32_t major, std::uint32_t minor) : major(major), minor(minor) {}
    };

    class Instance
    {
    public:
        Instance()                           = default;
        ~Instance()                          = default;
        Instance(const Instance&)            = delete;
        Instance& operator=(const Instance&) = delete;
        Instance(Instance&&)                 = delete;
        Instance& operator=(Instance&&)      = delete;

        [[nodiscard]] bool Init(std::string_view appName,
                                std::string_view engineName,
                                APIVersion apiVersion,
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
} // namespace Nebula::Vulkan
