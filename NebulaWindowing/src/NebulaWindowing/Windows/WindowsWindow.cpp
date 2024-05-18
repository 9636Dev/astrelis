#include "WindowsWindow.hpp"

#include "NebulaCore/Util/Assert.hpp"

namespace Nebula
{
    WindowsWindow::WindowsWindow()
    {
        static bool s_GLFWInitialized = false;
        if (!s_GLFWInitialized)
        {
            int success = glfwInit();
            NEBULA_CORE_REQUIRE(success == GLFW_TRUE, "Could not initialize GLFW!");
            s_GLFWInitialized = true;
        }

        glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    #ifdef NEBULA_DEBUG
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
    #endif
        m_Window = glfwCreateWindow(1280, 720, "Nebula Engine", nullptr, nullptr);

        if (m_Window == nullptr)
        {
            glfwTerminate();
        }
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

    gsl::owner<Window*> CreateWindow()
    {
        return gsl::owner<Window*>(new WindowsWindow());
    }
} // namespace Nebula

