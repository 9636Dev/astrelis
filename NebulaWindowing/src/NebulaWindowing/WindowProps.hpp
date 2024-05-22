#pragma once

#include <cstdint>
#include <string>

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

}  // namespace Nebula
