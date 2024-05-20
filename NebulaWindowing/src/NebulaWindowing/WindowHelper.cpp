#include "WindowHelper.hpp"

namespace Nebula
{
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
        }

        GLFWwindow* window = glfwCreateWindow(props.Width, props.Height, props.Title.c_str(), nullptr, nullptr);
        if (window == nullptr)
        {
            glfwTerminate();
            return WindowCreationError::WindowCreationFailed;
        }

        return window;
    }
}  // namespace Nebula
