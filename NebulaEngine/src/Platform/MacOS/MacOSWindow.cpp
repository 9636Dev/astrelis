#include "MacOSWindow.hpp"

#include "../GLFW/GLFWWindowHelper.hpp"
#include "NebulaEngine/Core/Profiler.hpp"
#include "NebulaEngine/Renderer/RendererAPI.hpp"

namespace Nebula
{
    MacOSWindow::MacOSWindow(GLFWwindow* window, MacOSWindowData data) :
        m_Window(window),
        m_Data(std::move(data)),
        m_Context(nullptr)
    {
    }

    MacOSWindow::~MacOSWindow()
    {
        NEBULA_PROFILE_SCOPE("MacOSWindow::~MacOSWindow");
        m_Context->Shutdown();
        GLFWWindowHelper::DestroyWindow(std::move(m_Window));
    }

    void MacOSWindow::BeginFrame() { m_Context->BeginFrame(); }

    void MacOSWindow::EndFrame() { m_Context->EndFrame(); }

    void MacOSWindow::OnUpdate() { glfwPollEvents(); }

    void MacOSWindow::WaitForEvents() { glfwWaitEvents(); }

    Bounds MacOSWindow::GetViewportBounds() const
    {
        // Get size using GLFW
        std::int32_t width  = 0;
        std::int32_t height = 0;
        glfwGetFramebufferSize(m_Window.Raw(), &width, &height);
        return Bounds(0, 0, width, height);
    }

    Result<RefPtr<MacOSWindow>, std::string> MacOSWindow::Create(const WindowProps& props)
    {
        NEBULA_PROFILE_SCOPE("MacOSWindow::Create");
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
            RefPtr<MacOSWindow>::Create(windowRes.Unwrap(), MacOSWindowData(props.Title, props.Width, props.Height));
        GLFWWindowHelper::SetEventCallbacks(window->m_Window.Raw(), window->m_Data);
        window->m_Context = GraphicsContext::Create(window->m_Window.Raw());
        if (!window->m_Context->Init())
        {
            return "Failed to initialize context!";
        }
        return window;
    }
} // namespace Nebula

