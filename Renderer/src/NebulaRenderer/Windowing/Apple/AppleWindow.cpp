#include "AppleWindow.hpp"

#include "../WindowHelper.hpp"

namespace Nebula
{
    AppleWindow::AppleWindow(GLFWwindow* window) : m_Window(window)
    {
        m_Data.EventCallback = [](Event& event) {};
        WindowHelper::SetEventCallbacks(m_Window, m_Data);

        glfwMakeContextCurrent(m_Window); // This is needed on Wayland to show the window
    }

    AppleWindow::~AppleWindow() { WindowHelper::TerminateWindow(m_Window); }

    bool AppleWindow::ShouldClose() const noexcept { return glfwWindowShouldClose(m_Window) != 0; }

    void AppleWindow::PollEvents() noexcept { glfwPollEvents(); }

    void AppleWindow::SwapBuffers() noexcept { glfwSwapBuffers(m_Window); }

    Result<Ptr<Window>, WindowCreationError> CreateWindow(WindowProps& props)
    {
        return WindowHelper::CreateWindow(props).MapMove(
            [](GLFWwindow* window) { return MakePtr<AppleWindow>(window).Cast<Window>(); });
    }
} // namespace Nebula
