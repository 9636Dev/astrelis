#include "MacOSWindow.hpp"

#include "../WindowHelper.hpp"

namespace Nebula
{
    MacOSWindow::MacOSWindow(GLFWwindow* window) : m_Window(window)
    {
        m_Data.EventCallback = [](Event& event) {};
        WindowHelper::SetEventCallbacks(m_Window, m_Data);

        glfwMakeContextCurrent(m_Window); // This is needed on Wayland to show the window
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
