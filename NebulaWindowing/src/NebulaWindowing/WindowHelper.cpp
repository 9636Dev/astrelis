#include "WindowHelper.hpp"

namespace Nebula
{
    Result<GLFWwindow*, WindowCreationError> WindowHelper::CreateWindow(const WindowProps& props)
    {
        if (glfwInit() != GLFW_TRUE)
        {
            return WindowCreationError::GLFWInitFailed;
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
