#pragma once

#include "../Window.hpp"
#include <GLFW/glfw3.h>

namespace Nebula
{
    class WindowsWindow : public Window
    {
    public:
        WindowsWindow();
        WindowsWindow(const WindowsWindow&) = delete;
        WindowsWindow& operator=(const WindowsWindow&) = delete;
        WindowsWindow(WindowsWindow&&) = delete;
        WindowsWindow& operator=(WindowsWindow&&) = delete;
        ~WindowsWindow() override;

        [[nodiscard]] bool ShouldClose() const noexcept override;
        void PollEvents() noexcept override;
        void SwapBuffers() noexcept override;

        inline bool IsOk() const noexcept override { return m_Window != nullptr; }

    private:
        GLFWwindow* m_Window;
    };
}  // namespace Nebula
