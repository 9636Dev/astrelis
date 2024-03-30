#include "Window.hpp"
#include <NebulaCore/Log.hpp>

namespace Nebula
{
    GLFWWindow::GLFWWindow()
    {
        if (glfwInit() != GLFW_TRUE)
        {
            NEB_CORE_LOG_ERROR("Failed to initialize GLFW");
        }

        m_Window = glfwCreateWindow(800, 600, "Nebula", nullptr, nullptr);
        if (m_Window == nullptr)
        {
            NEB_CORE_LOG_ERROR("Failed to create GLFW window");
        }
    }

    GLFWWindow::~GLFWWindow()
    {
        glfwDestroyWindow(m_Window);
        glfwTerminate();
    }

    void GLFWWindow::SwapBuffers()
    {
        glfwSwapBuffers(m_Window);
    }

    void GLFWWindow::PollEvents()
    {
        glfwPollEvents();
    }

    bool GLFWWindow::ShouldClose() const
    {
        return static_cast<bool>(glfwWindowShouldClose(m_Window));
    }
} // namespace Nebula

extern "C"
{
    NEBULA_GRAPHICS_OPENGL_API Nebula::Window* InternalCreateGLFWWindow()
    {
        return new Nebula::GLFWWindow();
    }
}
