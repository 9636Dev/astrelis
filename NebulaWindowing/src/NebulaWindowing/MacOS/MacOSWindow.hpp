#pragma once

#include "../Window.hpp"
#include <GLFW/glfw3.h>

namespace Nebula
{
    struct MacOSWindowData
    {
        std::function<void(Event&)> EventCallback;
    };

    class MacOSWindow : public Window
    {
    public:
        explicit MacOSWindow(GLFWwindow* window);
        MacOSWindow(const MacOSWindow&) = delete;
        MacOSWindow& operator=(const MacOSWindow&) = delete;
        MacOSWindow(MacOSWindow&&) = delete;
        MacOSWindow& operator=(MacOSWindow&&) = delete;
        ~MacOSWindow() override;

        [[nodiscard]] bool ShouldClose() const noexcept override;
        void PollEvents() noexcept override;
        void SwapBuffers() noexcept override;

        inline bool IsOk() const noexcept override { return m_Window != nullptr; }
        inline void SetEventCallback(const EventCallbackFn& callback) noexcept override { m_Data.EventCallback = callback; }

    private:
        GLFWwindow* m_Window;
        MacOSWindowData m_Data;
    };
}  // namespace Nebula

