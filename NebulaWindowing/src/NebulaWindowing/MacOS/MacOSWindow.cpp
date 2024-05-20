#include "MacOSWindow.hpp"

namespace Nebula
{
    MacOSWindow::MacOSWindow(GLFWwindow* window)
    : m_Window(window)
    {
    }

    MacOSWindow::~MacOSWindow()
    {
        glfwDestroyWindow(m_Window);
    }

    bool MacOSWindow::ShouldClose() const noexcept
    {
        return glfwWindowShouldClose(m_Window) != 0;
    }

    void MacOSWindow::PollEvents() noexcept
    {
        glfwPollEvents();
    }

    void MacOSWindow::SwapBuffers() noexcept
    {
        glfwSwapBuffers(m_Window);
    }

    Result<Ptr<Window>, WindowCreationError> CreateWindow(WindowProps& props)
    {
        auto res = WindowHelper::CreateWindow(props);
        return res.MapMove([](GLFWwindow* window) {
            return MakePtr<MacOSWindow>(window).Cast<Window>();
        });
    }
} // namespace Nebula


