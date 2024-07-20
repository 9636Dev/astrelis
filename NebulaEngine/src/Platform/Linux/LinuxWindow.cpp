#include "LinuxWindow.hpp"

#include "NebulaEngine/Renderer/RendererAPI.hpp"
#include "Platform/GLFW/GLFWWindowHelper.hpp"

namespace Nebula
{
    LinuxWindow::LinuxWindow(GLFWwindow* window, LinuxWindowData data) :
        m_Window(window),
        m_Data(std::move(data)),
        m_Context(nullptr)
    {
    }

    LinuxWindow::~LinuxWindow()
    {
        m_Context->Shutdown();
        GLFWWindowHelper::DestroyWindow(std::move(m_Window));
    }

    void LinuxWindow::OnUpdate() { glfwPollEvents(); }

    void LinuxWindow::WaitForEvents() { glfwWaitEvents(); }

    void LinuxWindow::BeginFrame() { m_Context->BeginFrame(); }

    void LinuxWindow::EndFrame() { m_Context->EndFrame(); }

    Rect2Di LinuxWindow::GetViewportBounds() const
    {
        // Get size using GLFW
        std::int32_t width  = 0;
        std::int32_t height = 0;
        glfwGetFramebufferSize(m_Window.Raw(), &width, &height);
        return Rect2Di(0, 0, width, height);
    }

    Result<RefPtr<LinuxWindow>, std::string> LinuxWindow::Create(const WindowProps& props)
    {
        Result<GLFWwindow*, std::string> windowRes("No Window API selected");

        switch (RendererAPI::GetAPI())
        {
        case RendererAPI::API::None:
        case RendererAPI::API::Vulkan:
            windowRes = GLFWWindowHelper::CreateNonAPIWindow(props);
            break;
        };

        if (windowRes.IsErr())
        {
            return windowRes.UnwrapErr();
        }

        auto window =
            RefPtr<LinuxWindow>::Create(windowRes.Unwrap(), LinuxWindowData(props.Title, props.Dimensions));
        GLFWWindowHelper::SetEventCallbacks(window->m_Window.Raw(), window->m_Data);
        ContextProps ctxProps;
        ctxProps.VSync    = props.VSync;
        window->m_Context = GraphicsContext::Create(window->m_Window.Raw(), ctxProps);
        if (!window->m_Context->Init())
        {
            return "Failed to initialize context!";
        }
        return window;
    }
} // namespace Nebula


