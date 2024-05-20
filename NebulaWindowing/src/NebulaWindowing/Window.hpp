#pragma once

#include <gsl/gsl>

#include "NebulaCore/Util/Result.hpp"

namespace Nebula
{
    class Window
    {
    public:
        Window() = default;
        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;
        Window(Window&&) = delete;
        Window& operator=(Window&&) = delete;
        virtual ~Window() = default;

        [[nodiscard]] virtual bool ShouldClose() const noexcept = 0;
        virtual void PollEvents() noexcept = 0;
        virtual void SwapBuffers() noexcept = 0;

        virtual bool IsOk() const noexcept = 0;
    };

    extern gsl::owner<Window*> CreateWindow();
}  // namespace Nebula
