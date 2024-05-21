#pragma once

#include "../Window.hpp"
#include <GLFW/glfw3.h>

namespace Nebula
{
    struct WindowsWindowData
    {
        std::function<void(Event&)> EventCallback;
    };

    class WindowsWindow : public Window
    {
    public:
        explicit WindowsWindow(GLFWwindow* window);
        WindowsWindow(const WindowsWindow&) = delete;
        WindowsWindow& operator=(const WindowsWindow&) = delete;
        WindowsWindow(WindowsWindow&&) = delete;
        WindowsWindow& operator=(WindowsWindow&&) = delete;
        ~WindowsWindow() override;

        [[nodiscard]] bool ShouldClose() const noexcept override;
        void PollEvents() noexcept override;
        void SwapBuffers() noexcept override;

        inline bool IsOk() const noexcept override { return m_Window != nullptr; }
        inline void SetEventCallback(const EventCallbackFn& callback) noexcept override { m_Data.EventCallback = callback; }

    private:
        WindowsWindowData m_Data;
        GLFWwindow* m_Window;
    };
}  // namespace Nebula
