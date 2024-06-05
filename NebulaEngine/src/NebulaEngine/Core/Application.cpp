#include "Application.hpp"

#include "Base.hpp"
#include "NebulaEngine/Core/Window.hpp"
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

        auto res = CreateWindow();
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
    }

    Application::~Application() = default;

    void Application::Run()
    {
        while (m_Running)
        {
            for (auto* layer : m_LayerStack)
            {
                layer->OnUpdate();
            }

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
