#include "MacOSWindow.hpp"

namespace Nebula
{
    MacOSWindow::MacOSWindow(GLFWwindow* window) : m_Window(window), m_Data()
    {
        m_Data.EventCallback = [](Event&) {};
        WindowHelper::SetEventCallback(m_Window, m_Data);
    }

    MacOSWindow::~MacOSWindow() { WindowHelper::TerminateWindow(m_Window); }

    bool MacOSWindow::ShouldClose() const noexcept { return glfwWindowShouldClose(m_Window) != 0; }

    void MacOSWindow::PollEvents() noexcept { glfwPollEvents(); }

    void MacOSWindow::SwapBuffers() noexcept { glfwSwapBuffers(m_Window); }

    Result<Ptr<Window>, WindowCreationError> CreateWindow(WindowProps& props)
    {
        return WindowHelper::CreateWindow(props).MapMove(
            [](GLFWwindow* window) { return MakePtr<MacOSWindow>(window).Cast<Window>(); });
    }
} // namespace Nebula


