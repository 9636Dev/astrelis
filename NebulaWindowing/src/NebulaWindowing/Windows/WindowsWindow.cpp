#include "WindowsWindow.hpp"

#include "NebulaCore/Util/Assert.hpp"

namespace Nebula
{
    WindowsWindow::WindowsWindow(GLFWwindow* window) : m_Window(window)
    {
        m_Data.EventCallback = [](Event&) {};
        WindowHelper::SetEventCallback(m_Window, m_Data);
    }

    WindowsWindow::~WindowsWindow() {
        WindowHelper::TerminateWindow(m_Window);
    }

    bool WindowsWindow::ShouldClose() const noexcept { return glfwWindowShouldClose(m_Window) != 0; }

    void WindowsWindow::PollEvents() noexcept { glfwPollEvents(); }

    void WindowsWindow::SwapBuffers() noexcept { glfwSwapBuffers(m_Window); }

    Result<Ptr<Window>, WindowCreationError> CreateWindow(WindowProps& props)
    {
        return WindowHelper::CreateWindow(props).MapMove(
            [](GLFWwindow* window) { return MakePtr<WindowsWindow>(window).Cast<Window>(); });
    }
} // namespace Nebula

