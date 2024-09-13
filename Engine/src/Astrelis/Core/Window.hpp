#pragma once

#include "Astrelis/Events/Event.hpp"
#include "Astrelis/Renderer/GraphicsContext.hpp"

#include <functional>
#include <string>
#include <utility>

#include "Geometry.hpp"
#include "Pointer.hpp"
#include "Result.hpp"

namespace Astrelis
{
    using WindowEventCallback = std::function<void(Event&)>;

    struct WindowProps
    {
        std::string Title;
        Dimension2Du Dimensions;
        bool VSync;

        explicit WindowProps(const std::string& title = "Astrelis Engine",
                             Dimension2Du dimensions  = {1'280, 720},
                             bool vsync               = true) :
            Title(title),
            Dimensions(dimensions),
            VSync(vsync)
        {
        }
    };

    struct BaseWindowData
    {
        std::string Title;
        Dimension2Du Dimensions;
        WindowEventCallback EventCallback;

        BaseWindowData(std::string title, Dimension2Du dimensions, WindowEventCallback callback = [](Event&) {}) :
            Title(std::move(title)),
            Dimensions(dimensions),
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
        virtual Rect2Di GetViewportBounds() const                          = 0;

        virtual std::uint32_t GetWidth() const  = 0;
        virtual std::uint32_t GetHeight() const = 0;

        virtual RefPtr<GraphicsContext> GetGraphicsContext() const = 0;
        virtual void* GetNativeWindow() const                      = 0;
        virtual void SetVSync(bool enabled)                        = 0;
        virtual bool IsVSync() const                               = 0;

        static Result<RefPtr<Window>, std::string> Create(const WindowProps& props = WindowProps());
    };

} // namespace Astrelis
