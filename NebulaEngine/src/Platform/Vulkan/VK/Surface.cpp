#include "Surface.hpp"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "NebulaEngine/Core/Log.hpp"

namespace Nebula::Vulkan
{
    bool Surface::Init(Instance& instance, const RawRef<GLFWwindow*>& window)
    {
        if (glfwCreateWindowSurface(instance.GetHandle(), window, nullptr, &m_Surface) != VK_SUCCESS)
        {
            NEBULA_CORE_LOG_ERROR("Failed to create window surface!");
            return false;
        }
        return true;
    }

    void Surface::Destroy(Instance& instance) { vkDestroySurfaceKHR(instance, m_Surface, nullptr); }
} // namespace Nebula::Vulkan
