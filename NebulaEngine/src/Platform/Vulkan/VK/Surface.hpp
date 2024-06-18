#pragma once

#include "NebulaEngine/Core/Pointer.hpp"
#include "Platform/Vulkan/VK/Instance.hpp"

#include <vulkan/vulkan.h>

struct GLFWwindow;

namespace Nebula::Vulkan
{
    class Surface
    {
    public:
        Surface()                          = default;
        ~Surface()                         = default;
        Surface(const Surface&)            = delete;
        Surface& operator=(const Surface&) = delete;
        Surface(Surface&&)                 = delete;
        Surface& operator=(Surface&&)      = delete;

        [[nodiscard]] bool Init(Instance& instance, const RawRef<GLFWwindow*>& window);
        void Destroy(Instance& instance);

        [[nodiscard]] VkSurfaceKHR GetHandle() const { return m_Surface; }
    private:
        VkSurfaceKHR m_Surface = VK_NULL_HANDLE;
    };
} // namespace Nebula::Vulkan

