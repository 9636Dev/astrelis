#pragma once

#include "../Window.hpp"
#include <GLFW/glfw3.h>

namespace Nebula
{
    class MacOSWindow : public Window
    {
    public:
        MacOSWindow();
        MacOSWindow(const MacOSWindow&) = delete;
        MacOSWindow& operator=(const MacOSWindow&) = delete;
        MacOSWindow(MacOSWindow&&) = delete;
        MacOSWindow& operator=(MacOSWindow&&) = delete;
        ~MacOSWindow() override;

        [[nodiscard]] bool ShouldClose() const noexcept override;
        void PollEvents() noexcept override;
        void SwapBuffers() noexcept override;

        inline bool IsOk() const noexcept override { return m_Window != nullptr; }

    private:
        GLFWwindow* m_Window;
    };
}  // namespace Nebula

