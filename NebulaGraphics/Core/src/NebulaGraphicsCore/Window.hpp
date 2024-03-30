#pragma once

namespace Nebula
{
    class Window
    {
    public:
        Window() = default;
        virtual ~Window() = default;
        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;
        Window(Window&&) = delete;
        Window& operator=(Window&&) = delete;
        
        [[nodiscard]] virtual bool ShouldClose() const = 0;
        virtual void SwapBuffers() = 0;
        virtual void PollEvents() = 0;
    };
} // namespace Nebula
