#include "LinuxWindow.hpp"

#include "../WindowHelper.hpp"

namespace Nebula
{
    LinuxWindow::LinuxWindow(GLFWwindow* window) : m_Window(window)
    {
        m_Data.EventCallback = [](Event& event) {};
        WindowHelper::SetEventCallbacks(m_Window, m_Data);

        glfwMakeContextCurrent(m_Window); // This is needed on Wayland to show the window
    }

    LinuxWindow::~LinuxWindow() {
        WindowHelper::TerminateWindow(m_Window);
    }

    bool LinuxWindow::ShouldClose() const noexcept { return glfwWindowShouldClose(m_Window) != 0; }

    void LinuxWindow::PollEvents() noexcept { glfwPollEvents(); }

    void LinuxWindow::SwapBuffers() noexcept { glfwSwapBuffers(m_Window); }

    Result<Ptr<Window>, WindowCreationError> CreateWindow(WindowProps& props)
    {
        return WindowHelper::CreateWindow(props).MapMove(
            [](GLFWwindow* window) { return MakePtr<LinuxWindow>(window).Cast<Window>(); });
    }
} // namespace Nebula
