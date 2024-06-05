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

    Result<Ptr<Window>, std::string> CreateWindow(const WindowProps &props)
    {
        auto res = GLFWWindowHelper::CreateWindow(props);
        return res.MapMove([props](GLFWwindow* window) {
            return MakePtr<LinuxWindow>(window, LinuxWindowData(props.Title, props.Width, props.Height)).Cast<Window>();
        });
    }
} // namespace Nebula
