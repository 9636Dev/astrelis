#include "MacOSWindow.hpp"

#include "../GLFW/GLFWWindowHelper.hpp"

namespace Nebula
{
    MacOSWindow::MacOSWindow(GLFWwindow* window, MacOSWindowData data) : m_Window(window), m_Data(std::move(data)), m_Context(nullptr)
    {
        GLFWWindowHelper::SetEventCallbacks(m_Window, m_Data);
        glfwMakeContextCurrent(m_Window);
        m_Context = GraphicsContext::Create();
        m_Context->Init();
    }

    MacOSWindow::~MacOSWindow() {
        m_Context->Shutdown();
        GLFWWindowHelper::DestroyWindow(m_Window);
    }

    void MacOSWindow::OnUpdate()
    {
        glfwPollEvents();
        //m_RenderContext->SwapBuffers();
    }

    Result<RefPtr<MacOSWindow>, std::string> MacOSWindow::Create(const WindowProps& props)
    {
        return GLFWWindowHelper::CreateLegacyWindow(props).MapMove([props](GLFWwindow* window) {
            return RefPtr<MacOSWindow>::Create(window, MacOSWindowData(props.Title, props.Width, props.Height));
        });
    }
} // namespace Nebula

