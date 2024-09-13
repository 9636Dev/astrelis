#include "WindowsWindow.hpp"

#include "Astrelis/Renderer/RendererAPI.hpp"

#include "../GLFW/GLFWWindowHelper.hpp"

namespace Astrelis
{
    WindowsWindow::WindowsWindow(GLFWwindow* window, WindowsWindowData data) :
        m_Window(window),
        m_Data(std::move(data)),
        m_Context(nullptr)
    {
        GLFWWindowHelper::SetEventCallbacks(m_Window.Raw(), m_Data);
        m_Context = GraphicsContext::Create(m_Window.Raw());
        m_Context->Init();
    }

    WindowsWindow::~WindowsWindow()
    {
        m_Context->Shutdown();
        GLFWWindowHelper::DestroyWindow(std::move(m_Window));
    }

    void WindowsWindow::BeginFrame() { m_Context->BeginFrame(); }

    void WindowsWindow::EndFrame() { m_Context->EndFrame(); }

    void WindowsWindow::OnUpdate() { glfwPollEvents(); }

    void WindowsWindow::WaitForEvents() { glfwWaitEvents(); }

    Rect2Di WindowsWindow::GetViewportBounds() const
    {
        // Get size using GLFW
        std::int32_t width  = 0;
        std::int32_t height = 0;
        glfwGetFramebufferSize(m_Window.Raw(), &width, &height);
        return Rect2Di(0, 0, width, height);
    }

    Result<RefPtr<WindowsWindow>, std::string> WindowsWindow::Create(const WindowProps& props)
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
            RefPtr<WindowsWindow>::Create(windowRes.Unwrap(), WindowsWindowData(props.Title, props.Dimensions));
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
} // namespace Astrelis


