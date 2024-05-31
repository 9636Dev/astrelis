#pragma once

#include "Event/Event.hpp"

#include <cstdint>
#include <functional>
#include <string>

namespace Nebula
{
    using EventCallbackFn = std::function<void(class Event&)>;

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

    struct WindowData
    {
        EventCallbackFn EventCallback;
    };

} // namespace Nebula
