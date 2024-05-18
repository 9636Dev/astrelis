#pragma once

#include "../Window.hpp"
#include <GLFW/glfw3.h>

namespace Nebula
{
    class LinuxWindow : public Window
    {
    public:
        LinuxWindow();
        LinuxWindow(const LinuxWindow&) = delete;
        LinuxWindow& operator=(const LinuxWindow&) = delete;
        LinuxWindow(LinuxWindow&&) = delete;
        LinuxWindow& operator=(LinuxWindow&&) = delete;
        ~LinuxWindow() override;

        [[nodiscard]] bool ShouldClose() const noexcept override;
        void PollEvents() noexcept override;
        void SwapBuffers() noexcept override;

        inline bool IsOk() const noexcept override { return m_Window != nullptr; }

    private:
        GLFWwindow* m_Window;
    };
}  // namespace Nebula
