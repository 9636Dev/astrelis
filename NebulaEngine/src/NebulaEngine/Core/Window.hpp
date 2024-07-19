#pragma once

#include <functional>
#include <string>
#include <utility>

#include "NebulaEngine/Core/Bounds.hpp"
#include "NebulaEngine/Events/Event.hpp"
#include "NebulaEngine/Renderer/GraphicsContext.hpp"
#include "Pointer.hpp"
#include "Result.hpp"

namespace Nebula
{
    using WindowEventCallback = std::function<void(Event&)>;

    struct WindowProps
    {
        std::string Title;
        std::uint32_t Width;
        std::uint32_t Height;
        bool VSync;

        explicit WindowProps(const std::string& title = "Nebula Engine",
                             std::uint32_t width      = 1'280,
                             std::uint32_t height     = 720,
                             bool vsync               = true) :
            Title(title),
            Width(width),
            Height(height),
            VSync(vsync)
        {
        }
    };

    struct BaseWindowData
    {
        std::string Title;
        std::uint32_t Width;
        std::uint32_t Height;
        WindowEventCallback EventCallback;

        BaseWindowData(std::string title,
                       std::uint32_t width,
                       std::uint32_t height,
                       WindowEventCallback callback = [](Event&) {}) :
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
        virtual ~Window()                = default;
        Window(const Window&)            = delete;
        Window& operator=(const Window&) = delete;
        Window(Window&&)                 = delete;
        Window& operator=(Window&&)      = delete;

        virtual void BeginFrame() = 0;
        virtual void EndFrame()   = 0;
        virtual void OnUpdate()   = 0;

        virtual void WaitForEvents()                                       = 0;
        virtual void SetEventCallback(const WindowEventCallback& callback) = 0;
        virtual Bounds GetViewportBounds() const                           = 0;

        virtual std::uint32_t GetWidth() const  = 0;
        virtual std::uint32_t GetHeight() const = 0;

        virtual RefPtr<GraphicsContext> GetGraphicsContext() const = 0;
        virtual void* GetNativeWindow() const                      = 0;
        virtual void SetVSync(bool enabled)                        = 0;
        virtual bool IsVSync() const                               = 0;

        static Result<RefPtr<Window>, std::string> Create(const WindowProps& props = WindowProps());
    };

} // namespace Nebula
