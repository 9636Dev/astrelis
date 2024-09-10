#pragma once

#include <GLFW/glfw3.h>

#include "Astrelis/Core/Window.hpp"

namespace Astrelis
{
    class GLFWWindowHelper
    {
    public:
        static Result<GLFWwindow*, std::string> CreateGLWindow(const WindowProps& props);
        static Result<GLFWwindow*, std::string> CreateNonAPIWindow(const WindowProps& props);
        static void DestroyWindow(OwnedPtr<GLFWwindow*> window);

        static void SetEventCallbacks(RawRef<GLFWwindow*> window, BaseWindowData& data);
    private:
        template<typename T> static T& GetUserData(GLFWwindow* window)
        {
            return *static_cast<T*>(glfwGetWindowUserPointer(window));
        }

        static bool InitGLFW();
        static void TerminateGLFW();

        static std::size_t s_WindowCount;
        static bool s_GLFWInitialized;
    };
} // namespace Astrelis
