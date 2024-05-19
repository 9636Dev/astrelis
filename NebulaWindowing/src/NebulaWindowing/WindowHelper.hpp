#pragma once

#include <GLFW/glfw3.h>

namespace Nebula
{
    class WindowHelper
    {
    public:
        static void SetWindowHints();
        static void SetWindowCallbacks(GLFWwindow* window);
    };
}; // namespace Nebula
