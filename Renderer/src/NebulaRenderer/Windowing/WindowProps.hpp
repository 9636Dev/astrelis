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
        std::int32_t Width = 1280;
        std::int32_t Height = 720;

        WindowProps() = default;

        WindowProps(std::string title, std::int32_t width, std::int32_t height) :
            Title(std::move(title)),
            Width(width),
            Height(height)
        {
        }
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
