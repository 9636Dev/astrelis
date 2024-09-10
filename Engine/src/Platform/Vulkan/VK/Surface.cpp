#include "Surface.hpp"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "Astrelis/Core/Log.hpp"

namespace Astrelis::Vulkan
{
    bool Surface::Init(Instance& instance, const RawRef<GLFWwindow*>& window)
    {
        if (glfwCreateWindowSurface(instance.GetHandle(), window, nullptr, &m_Surface) != VK_SUCCESS)
        {
            ASTRELIS_CORE_LOG_ERROR("Failed to create window surface!");
            return false;
        }
        return true;
    }

    void Surface::Destroy(Instance& instance) { vkDestroySurfaceKHR(instance, m_Surface, nullptr); }
} // namespace Astrelis::Vulkan
