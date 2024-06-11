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
        m_RenderContext->Init();
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

    Bounds MacOSWindow::GetViewportBounds() const
    {
        // Frame buffer size
        int width = 0;
        int height = 0;
        glfwGetFramebufferSize(m_Window, &width, &height);
        return { 0, 0, width, height };
    }

    Result<Ptr<MacOSWindow>, std::string> MacOSWindow::Create(const WindowProps &props)
    {
        auto res = GLFWWindowHelper::CreateLegacyWindow(props);
        return res.MapMove([props](GLFWwindow* window) {
            return MakePtr<MacOSWindow>(window, MacOSWindowData(props.Title, props.Width, props.Height));
        });
    }
} // namespace Nebula

