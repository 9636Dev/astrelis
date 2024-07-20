#pragma once

#include <vulkan/vulkan.h>

#include "Instance.hpp"

#include "NebulaEngine/Core/Pointer.hpp"

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

