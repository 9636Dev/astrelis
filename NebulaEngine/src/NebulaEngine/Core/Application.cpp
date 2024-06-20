#include "Application.hpp"

#include "Base.hpp"
#include "NebulaEngine/Events/WindowEvent.hpp"
#include "NebulaEngine/UI/ImGui/ImGuiBackend.hpp"
#include "NebulaEngine/UI/ImGui/ImGuiLayer.hpp"
#include "Profiler.hpp"
#include "Time.hpp"
#include "Window.hpp"

#include <utility>

namespace Nebula
{
    Application* Application::s_Instance = nullptr;

    Application::Application(ApplicationSpecification specification) :
        m_Specification(std::move(specification)),
        m_Window(nullptr),
        m_ImGuiLayer(nullptr)
    {
        NEBULA_PROFILE_SCOPE("Application::Application");
        Nebula::Log::Init();
        NEBULA_VERIFY(s_Instance == nullptr, "Application already exists (Should be singleton)");
        s_Instance = this;

        auto res = Window::Create();
        if (res.IsErr())
        {
            NEBULA_LOG_ERROR("Failed to create window: {0}", res.UnwrapErr());
            return;
        }

        m_Window = std::move(res.Unwrap());
        m_Window->SetEventCallback(NEBULA_BIND_EVENT_FN(Application::OnEvent));
        // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
        OwnedPtr<ImGuiLayer*> imguiLayer(new ImGuiLayer(ImGuiBackend::Create(m_Window)));
        m_ImGuiLayer = imguiLayer.Raw();
        PushOverlay(static_cast<OwnedPtr<Layer*>>(imguiLayer)); // Ownership transferred to LayerStack
    }

    Application::~Application()
    {
        NEBULA_PROFILE_SCOPE("Application::~Application");
        for (auto& layer : m_LayerStack)
        {
            layer->OnDetach();
        }
    }

    void Application::Run()
    {
        while (m_Running)
        {
            m_Window->BeginFrame();

            for (auto& layer : m_LayerStack)
            {
                layer->OnUpdate();
            }

            m_ImGuiLayer->Begin();

            for (auto& layer : m_LayerStack)
            {
                layer->OnUIRender();
            }

            m_ImGuiLayer->End();

            m_Window->EndFrame();
            m_Window->OnUpdate();
        }
    }

    void Application::OnEvent(Event& event)
    {
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<WindowClosedEvent>(NEBULA_BIND_EVENT_FN(Application::OnWindowClose));
        dispatcher.Dispatch<ViewportResizedEvent>(NEBULA_BIND_EVENT_FN(Application::OnViewportResize));

        for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
        {
            if (event.Handled)
            {
                break;
            }

            (*--it)->OnEvent(event);
        }
    }

    bool Application::OnWindowClose([[maybe_unused]] WindowClosedEvent& event)
    {
        m_Running = false;
        return true;
    }

    bool Application::OnViewportResize(ViewportResizedEvent& event)
    {
        if (event.GetWidth() == 0 || event.GetHeight() == 0)
        {
            return false;
        }

        // TODO(6/20/24) Resize the framebuffers for other libraries, like OpenGL

        return false;
    }

    void Application::PushLayer(OwnedPtr<Layer*> layer)
    {
        layer->OnAttach();
        m_LayerStack.PushLayer(std::move(layer));
    }

    void Application::PushOverlay(OwnedPtr<Layer*> overlay)
    {
        overlay->OnAttach();
        m_LayerStack.PushOverlay(std::move(overlay));
    }

    OwnedPtr<Layer*> Application::PopLayer(RawRef<Layer*> layer)
    {
        layer->OnDetach();
        return m_LayerStack.PopLayer(std::move(layer));
    }

    OwnedPtr<Layer*> Application::PopOverlay(RawRef<Layer*> overlay)
    {
        overlay->OnDetach();
        return m_LayerStack.PopOverlay(std::move(overlay));
    }
} // namespace Nebula
