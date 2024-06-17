#include "MacOSWindow.hpp"

#include "../GLFW/GLFWWindowHelper.hpp"

namespace Nebula
{
    MacOSWindow::MacOSWindow(GLFWwindow* window, MacOSWindowData data) :
        m_Window(window),
        m_Data(std::move(data))
    {
        GLFWWindowHelper::SetEventCallbacks(m_Window, m_Data);
        glfwMakeContextCurrent(m_Window);
    }


    MacOSWindow::~MacOSWindow()
    {
        GLFWWindowHelper::DestroyWindow(m_Window);
    }

    void MacOSWindow::OnUpdate()
    {
        glfwPollEvents();
        //m_RenderContext->SwapBuffers();
    }

    Result<RefPtr<Window>, std::string> MacOSWindow::Create(const WindowProps &props)
    {
        auto res = GLFWWindowHelper::CreateLegacyWindow(props);
        return res.MapMove([props](GLFWwindow* window) {
            return static_cast<RefPtr<Window>>(RefPtr<MacOSWindow>::Create(window, MacOSWindowData(props.Title, props.Width, props.Height)));
        });
    }
} // namespace Nebula

