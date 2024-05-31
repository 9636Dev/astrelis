#pragma once

#include <GLFW/glfw3.h>

#include "NebulaCore/Util/Result.hpp"
#include "WindowProps.hpp"

namespace Nebula
{
    class WindowHelper
    {
    public:
        static Result<GLFWwindow*, WindowCreationError> CreateWindow(const WindowProps& props);
        static void TerminateWindow(GLFWwindow* window);

        template<typename T> static T& GetUserData(GLFWwindow* window)
        {
            return *static_cast<T*>(glfwGetWindowUserPointer(window));
        }

        // NOLINTNEXTLINE(readability-function-cognitive-complexity)
        static void SetEventCallbacks(GLFWwindow* window, WindowData& data);
    };
}; // namespace Nebula
