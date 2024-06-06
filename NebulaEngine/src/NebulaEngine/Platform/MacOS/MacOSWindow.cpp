#include "MacOSWindow.hpp"

#include "NebulaEngine/Platform/GLFW/GLFWWindowHelper.hpp"
#include "NebulaEngine/Platform/OpenGL/OpenGLRenderContext.hpp"
#include "NebulaEngine/Renderer/RenderContext.hpp"

namespace Nebula
{
    MacOSWindow::MacOSWindow(GLFWwindow* window, MacOSWindowData data) :
        m_Window(window),
        m_Data(std::move(data))
    {
        GLFWWindowHelper::SetEventCallbacks(m_Window, m_Data);
        m_RenderContext = RenderContext::Create(m_Window);
        glfwMakeContextCurrent(m_Window);
    }


    MacOSWindow::~MacOSWindow()
    {
        GLFWWindowHelper::DestroyWindow(m_Window);
    }

    void MacOSWindow::OnUpdate()
    {
        glfwPollEvents();
        m_RenderContext->SwapBuffers();
    }

    Result<Ptr<Window>, std::string> CreateWindow(const WindowProps &props)
    {
        auto res = GLFWWindowHelper::CreateLegacyWindow(props);
        return res.MapMove([props](GLFWwindow* window) {
            return MakePtr<MacOSWindow>(window, MacOSWindowData(props.Title, props.Width, props.Height)).Cast<Window>();
        });
    }
} // namespace Nebula

