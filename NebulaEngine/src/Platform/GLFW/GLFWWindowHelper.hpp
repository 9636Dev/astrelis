#pragma once

#include <GLFW/glfw3.h>

#include "NebulaEngine/Core/Window.hpp"

namespace Nebula
{
    class GLFWWindowHelper
    {
    public:
        static Result<GLFWwindow*, std::string> CreateWindow(const WindowProps &props);
        // This creates 4.1 for MacOS
        static Result<GLFWwindow*, std::string> CreateLegacyWindow(const WindowProps &props);
        static void DestroyWindow(GLFWwindow *window);

        static void SetEventCallbacks(GLFWwindow* window, BaseWindowData& data);

    private:
        template<typename T>
        static T& GetUserData(GLFWwindow* window)
        {
            return *static_cast<T*>(glfwGetWindowUserPointer(window));
        }

        static bool InitGLFW();
        static void TerminateGLFW();

        static std::size_t s_WindowCount;
        static bool s_GLFWInitialized;
    };
} // namespace Nebula
