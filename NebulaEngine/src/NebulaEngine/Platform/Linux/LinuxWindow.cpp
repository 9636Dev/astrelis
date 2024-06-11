#include "LinuxWindow.hpp"
#include "NebulaEngine/Platform/GLFW/GLFWWindowHelper.hpp"

namespace Nebula
{
    LinuxWindow::LinuxWindow(GLFWwindow* window, LinuxWindowData data) :
        m_Window(window),
        m_Data(std::move(data))
    {
        GLFWWindowHelper::SetEventCallbacks(m_Window, m_Data);
        m_RenderContext = RenderContext::Create(m_Window);
        m_RenderContext->Init();
        glfwMakeContextCurrent(m_Window);
    }

    LinuxWindow::~LinuxWindow()
    {
        GLFWWindowHelper::DestroyWindow(m_Window);
    }

    void LinuxWindow::OnUpdate()
    {
        glfwPollEvents();
        m_RenderContext->SwapBuffers();
    }

    Bounds LinuxWindow::GetViewportBounds() const
    {
        // Frame buffer size
        int width = 0;
        int height = 0;
        glfwGetFramebufferSize(m_Window, &width, &height);
        return { 0, 0, width, height };
    }

    Result<Ptr<LinuxWindow>, std::string> LinuxWindow::Create(const WindowProps &props)
    {
        auto res = GLFWWindowHelper::CreateWindow(props);
        return res.MapMove([props](GLFWwindow* window) {
            return MakePtr<LinuxWindow>(window, LinuxWindowData(props.Title, props.Width, props.Height));
        });
    }
} // namespace Nebula
