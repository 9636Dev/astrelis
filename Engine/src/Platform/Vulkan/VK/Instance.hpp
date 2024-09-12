#pragma once

#include "Astrelis/Core/Types.hpp"

#include <cstdint>
#include <vector>
#include <vulkan/vulkan.h>

namespace Astrelis::Vulkan
{
    struct Version
    {
        std::uint32_t VulkanVersion = VK_VERSION_1_0;

        [[nodiscard]] bool IsValid() const { return VulkanVersion != 0; }
        Version(std::uint32_t major, std::uint32_t minor, std::uint32_t patch)
            : VulkanVersion(VK_MAKE_VERSION(major, minor, patch)) {}

        std::uint32_t GetMajor() const { return VK_VERSION_MAJOR(VulkanVersion); }
        std::uint32_t GetMinor() const { return VK_VERSION_MINOR(VulkanVersion); }
        std::uint32_t GetPatch() const { return VK_VERSION_PATCH(VulkanVersion); }
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

        [[nodiscard]] bool Init(CString appName,
                                Version appVersion,
                                Version apiVersion,
                                const std::vector<CString>& extensions,
                                const std::vector<CString>& layers);

        void Destroy();

        // NOLINTNEXTLINE(google-explicit-constructor, hicpp-explicit-conversions)
        operator VkInstance() const { return m_Instance; }

        VkInstance GetHandle() const { return m_Instance; }
    private:
        VkInstance m_Instance = VK_NULL_HANDLE;
    };
} // namespace Astrelis::Vulkan
