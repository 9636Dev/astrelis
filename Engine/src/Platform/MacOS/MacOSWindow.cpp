#include "MacOSWindow.hpp"
#include "Astrelis/Core/Base.hpp"

#include "Astrelis/Renderer/RendererAPI.hpp"
#include "Platform/GLFW/GLFWWindowHelper.hpp"

namespace Astrelis
{
    MacOSWindow::MacOSWindow(GLFWwindow* window, MacOSWindowData data) :
        m_Window(window),
        m_Data(std::move(data)),
        m_Context(nullptr)
    {
    }

    MacOSWindow::~MacOSWindow()
    {
        ASTRELIS_PROFILE_SCOPE("Astrelis::MacOSWindow::~MacOSWindow");
        m_Context->Shutdown();
        GLFWWindowHelper::DestroyWindow(std::move(m_Window));
    }

    void MacOSWindow::BeginFrame()
    {
        ASTRELIS_PROFILE_SCOPE("Astrelis::MacOSWindow::BeginFrame");
        m_Context->BeginFrame();
    }

    void MacOSWindow::EndFrame() {
        ASTRELIS_PROFILE_SCOPE("Astrelis::MacOSWindow::EndFrame");
        m_Context->EndFrame();
    }

    void MacOSWindow::OnUpdate() {
        ASTRELIS_PROFILE_SCOPE("Astrelis::MacOSWindow::OnUpdate");
        glfwPollEvents();
    }

    void MacOSWindow::WaitForEvents() { glfwWaitEvents(); }

    Rect2Di MacOSWindow::GetViewportBounds() const
    {
        // Get size using GLFW
        std::int32_t width  = 0;
        std::int32_t height = 0;
        glfwGetFramebufferSize(m_Window.Raw(), &width, &height);
        return Rect2Di(0, 0, width, height);
    }

    Result<RefPtr<MacOSWindow>, std::string> MacOSWindow::Create(const WindowProps& props)
    {
        ASTRELIS_PROFILE_SCOPE("Astrelis::MacOSWindow::Create");
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

        auto window = RefPtr<MacOSWindow>::Create(windowRes.Unwrap(), MacOSWindowData(props.Title, props.Dimensions));
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

