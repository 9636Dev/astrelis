#pragma once

#include <GLFW/glfw3.h>

#include <cstdint>
#include <string>

#include "NebulaCore/Util/Result.hpp"

namespace Nebula
{
    struct WindowProps
    {
        std::string Title;
        std::int32_t Width;
        std::int32_t Height;
    };

    enum class WindowCreationError
    {
        GLFWInitFailed,
        WindowCreationFailed
    };

    class WindowHelper
    {
    public:
        static Result<GLFWwindow*, WindowCreationError> CreateWindow(const WindowProps& props);
    };
}; // namespace Nebula
