#pragma once

#include <gsl/gsl>

#include "Event/Event.hpp"
#include "NebulaCore/Util/Pointer.hpp"
#include "NebulaCore/Util/Result.hpp"
#include "WindowProps.hpp"

namespace Nebula
{
    class Window
    {
    public:
        using EventCallbackFn            = std::function<void(Event&)>;
        Window()                         = default;
        Window(const Window&)            = delete;
        Window& operator=(const Window&) = delete;
        Window(Window&&)                 = delete;
        Window& operator=(Window&&)      = delete;
        virtual ~Window()                = default;

        [[nodiscard]] virtual bool ShouldClose() const noexcept = 0;
        virtual void PollEvents() noexcept                      = 0;
        virtual void SwapBuffers() noexcept                     = 0;

        virtual bool IsOk() const noexcept                                      = 0;
        virtual void SetEventCallback(const EventCallbackFn& callback) noexcept = 0;

        virtual void* GetNativeWindow() const noexcept = 0;
    };

    extern Result<Ptr<Window>, WindowCreationError> CreateWindow(WindowProps& props);
} // namespace Nebula
