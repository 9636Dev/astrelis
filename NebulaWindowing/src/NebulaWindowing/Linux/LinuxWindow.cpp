#include "LinuxWindow.hpp"

#include "NebulaCore/Log/Log.hpp"
#include "NebulaCore/Util/Assert.hpp"

namespace Nebula
{
    LinuxWindow::LinuxWindow(GLFWwindow* window)
    : m_Window(window)
    {
        glfwMakeContextCurrent(m_Window); // This is needed on Wayland to show the window
    }

    LinuxWindow::~LinuxWindow()
    {
        glfwDestroyWindow(m_Window);
    }

    bool LinuxWindow::ShouldClose() const noexcept
    {
        return glfwWindowShouldClose(m_Window) != 0;
    }

    void LinuxWindow::PollEvents() noexcept
    {
        glfwPollEvents();
    }

    void LinuxWindow::SwapBuffers() noexcept
    {
        glfwSwapBuffers(m_Window);
    }

    Result<Ptr<Window>, WindowCreationError> CreateWindow(WindowProps& props)
    {
        auto res = WindowHelper::CreateWindow(props);
        return res.MapMove([](GLFWwindow* window) {
            return MakePtr<LinuxWindow>(window).Cast<Window>();
        });
    }
} // namespace Nebula
