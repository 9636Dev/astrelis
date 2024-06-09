#pragma once

#include <functional>
#include <string>
#include <utility>

#include "NebulaEngine/Events/Event.hpp"
#include "Pointer.hpp"
#include "Result.hpp"

namespace Nebula
{
    using WindowEventCallback = std::function<void(Event&)>;

                                struct WindowProps
    {
        std::string Title;
        unsigned int Width;
        unsigned int Height;

        explicit WindowProps(const std::string& title = "Nebula Engine",
                             unsigned int width       = 1'280,
                             unsigned int height      = 720) :
            Title(title),
            Width(width),
            Height(height)
        {
        }
    };

    struct BaseWindowData
    {
        std::string Title;
        std::uint32_t Width;
        std::uint32_t Height;
        WindowEventCallback EventCallback;

        BaseWindowData(std::string title, std::uint32_t width, std::uint32_t height, WindowEventCallback callback = [](Event&) {}) :
            Title(std::move(title)),
            Width(width),
            Height(height),
            EventCallback(std::move(callback))
        {
        }
    };

    class Window
    {
    public:
        Window()                         = default;
        virtual ~Window()                        = default;
        Window(const Window&)            = delete;
        Window& operator=(const Window&) = delete;
        Window(Window&&)                 = delete;
        Window& operator=(Window&&)      = delete;

        virtual void OnUpdate() = 0;
        virtual void SetEventCallback(const WindowEventCallback& callback) = 0;

        virtual void* GetNativeWindow() const = 0;
        virtual std::uint32_t GetWidth() const = 0;
        virtual std::uint32_t GetHeight() const = 0;

        static Result<Ptr<Window>, std::string> Create(const WindowProps& props = WindowProps());
    };

} // namespace Nebula
