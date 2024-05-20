#include "WindowsWindow.hpp"

#include "NebulaCore/Util/Assert.hpp"

namespace Nebula
{
    WindowsWindow::WindowsWindow(GLFWwindow* window)
    : m_Window(window)
    {
    }
    WindowsWindow::~WindowsWindow()
    {
        glfwDestroyWindow(m_Window);
    }

    bool WindowsWindow::ShouldClose() const noexcept
    {
        return glfwWindowShouldClose(m_Window) != 0;
    }

    void WindowsWindow::PollEvents() noexcept
    {
        glfwPollEvents();
    }

    void WindowsWindow::SwapBuffers() noexcept
    {
        glfwSwapBuffers(m_Window);
    }

    Result<Ptr<Window>, WindowCreationError> CreateWindow(WindowProps& props)
    {
        auto res = WindowHelper::CreateWindow(props);
        return res.MapMove([](GLFWwindow* window) {
            return MakePtr<WindowsWindow>(window).Cast<Window>();
        });
    }
} // namespace Nebula

