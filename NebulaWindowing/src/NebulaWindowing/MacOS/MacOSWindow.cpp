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
        if (res.IsErr())
        {
            return res.UnwrapErr();
        }

        return MakePtr<MacOSWindow>(res.Unwrap()).Cast<Window>();
    }
} // namespace Nebula


