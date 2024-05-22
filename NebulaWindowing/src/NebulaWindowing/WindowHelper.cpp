#include "WindowHelper.hpp"
#include "GLFW/glfw3.h"

#include "NebulaCore/Log/Log.hpp"

namespace Nebula
{
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
    static std::size_t s_WindowCount = 0;

    Result<GLFWwindow*, WindowCreationError> WindowHelper::CreateWindow(const WindowProps& props)
    {
        static bool s_GLFWInitialized = false;
        if (!s_GLFWInitialized)
        {
            if (glfwInit() != GLFW_TRUE)
            {
                glfwTerminate();
                return WindowCreationError::GLFWInitFailed;
            }
            s_GLFWInitialized = true;
            NEB_CORE_LOG_DEBUG("Windowing: GLFW initialized");
        }

        GLFWwindow* window = glfwCreateWindow(props.Width, props.Height, props.Title.c_str(), nullptr, nullptr);
        if (window == nullptr)
        {
            glfwTerminate();
            return WindowCreationError::WindowCreationFailed;
        }

        s_WindowCount++;
        return window;
    }

    void WindowHelper::TerminateWindow(GLFWwindow* window)
    {
        glfwSetWindowUserPointer(window, nullptr);
        glfwDestroyWindow(window);

        if (--s_WindowCount == 0)
        {
            glfwTerminate();
            NEB_CORE_LOG_DEBUG("Windowing: GLFW terminated");
        }
    }
} // namespace Nebula
