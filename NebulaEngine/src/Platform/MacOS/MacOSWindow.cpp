#include "MacOSWindow.hpp"

#include "../GLFW/GLFWWindowHelper.hpp"
#include "NebulaEngine/Renderer/RendererAPI.hpp"

namespace Nebula
{
    MacOSWindow::MacOSWindow(GLFWwindow* window, MacOSWindowData data) : m_Window(window), m_Data(std::move(data)), m_Context(nullptr)
    {
        GLFWWindowHelper::SetEventCallbacks(m_Window.Raw(), m_Data);
        m_Context = GraphicsContext::Create(m_Window.Raw());
        m_Context->Init();
    }

    MacOSWindow::~MacOSWindow() {
        m_Context->Shutdown();
        GLFWWindowHelper::DestroyWindow(std::move(m_Window));
    }

    void MacOSWindow::BeginFrame()
    {
        m_Context->BeginFrame();
    }

    void MacOSWindow::EndFrame()
    {
        m_Context->EndFrame();
    }

    void MacOSWindow::OnUpdate()
    {
        glfwPollEvents();
    }

    void MacOSWindow::WaitForEvents()
    {
        glfwWaitEvents();
    }

    Bounds MacOSWindow::GetViewportBounds() const
    {
        // Get size using GLFW
        std::int32_t width = 0;
        std::int32_t height = 0;
        glfwGetFramebufferSize(m_Window.Raw(), &width, &height);
        return Bounds(0, 0, width, height);
    }

    Result<RefPtr<MacOSWindow>, std::string> MacOSWindow::Create(const WindowProps& props)
    {
        Result<GLFWwindow*, std::string> window("No Window API selected");

        switch (RendererAPI::GetAPI())
        {
        case RendererAPI::API::None:
        case RendererAPI::API::Vulkan:
            window = GLFWWindowHelper::CreateNonAPIWindow(props);
            break;
        };

        return window.MapMove([props](GLFWwindow* window) {
            return RefPtr<MacOSWindow>::Create(window, MacOSWindowData(props.Title, props.Width, props.Height));
        });
    }
} // namespace Nebula

