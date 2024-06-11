#include "Application.hpp"

#include "Base.hpp"
#include "NebulaEngine/Core/Window.hpp"
#include "NebulaEngine/Events/WindowEvent.hpp"
#include "NebulaEngine/UI/ImGui/ImGuiLayer.hpp"

#include <utility>

namespace Nebula
{
    Application* Application::s_Instance = nullptr;

    Application::Application(ApplicationSpecification specification) :
        m_Specification(std::move(specification)),
        m_Window(nullptr)
    {
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
        gsl::owner<ImGuiLayer*> imguiLayer = new ImGuiLayer();
        PushOverlay(imguiLayer); // Ownership transferred to LayerStack
        m_ImGuiLayer = imguiLayer;

        auto rendererRes =
            Nebula::Renderer::Create(m_Window.GetRef(), m_Window->GetViewportBounds());
        if (res.IsOk())
        {
            m_Renderer = std::move(rendererRes.Unwrap());
        }
        else
        {
            NEBULA_LOG_ERROR("Failed to create renderer: {0}", rendererRes.UnwrapErr());
        }
    }

    Application::~Application() = default;

    void Application::Run()
    {
        while (m_Running)
        {
            m_Renderer->BeginFrame();

            for (auto* layer : m_LayerStack)
            {
                layer->OnUpdate();
            }

            m_Renderer->EndFrame();

            m_ImGuiLayer->Begin();

            for (auto* layer : m_LayerStack)
            {
                layer->OnUIRender();
            }

            m_ImGuiLayer->End();

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

        NEBULA_CORE_LOG_DEBUG("Resizing to {0}x{1}", event.GetWidth(), event.GetHeight());
        m_Renderer->Viewport(
            {0, 0, static_cast<std::int32_t>(event.GetWidth()), static_cast<std::int32_t>(event.GetHeight())});
        return false;
    }

    void Application::PushLayer(Layer* layer)
    {
        m_LayerStack.PushLayer(layer);
        layer->OnAttach();
    }

    void Application::PushOverlay(Layer* overlay)
    {
        m_LayerStack.PushOverlay(overlay);
        overlay->OnAttach();
    }

    void Application::PopLayer(Layer* layer)
    {
        m_LayerStack.PopLayer(layer);
        layer->OnDetach();
    }

    void Application::PopOverlay(Layer* overlay)
    {
        m_LayerStack.PopOverlay(overlay);
        overlay->OnDetach();
    }
} // namespace Nebula
