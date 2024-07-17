#include "Application.hpp"

#include "Assert.hpp"
#include "Log.hpp"
#include "Profiler.hpp"
#include "Time.hpp"
#include "Utils/Function.hpp"
#include "Window.hpp"

#include "NebulaEngine/Events/WindowEvent.hpp"
#include "NebulaEngine/IO/File.hpp"
#include "NebulaEngine/UI/ImGui/ImGuiBackend.hpp"
#include "NebulaEngine/UI/ImGui/ImGuiLayer.hpp"

#include <filesystem>
#include <utility>

namespace Nebula
{
    Application* Application::s_Instance = nullptr;

    Application::Application(ApplicationSpecification specification, CreationStatus& status) :
        m_Specification(std::move(specification)),
        m_Window(nullptr),
        m_ImGuiLayer(nullptr)
    {
        NEBULA_PROFILE_SCOPE("Application::Application");
        Nebula::Log::Init();
        Nebula::Profiling::Init();
        NEBULA_VERIFY(s_Instance == nullptr, "Application already exists (Should be singleton)");
        s_Instance = this;

        // First we chdir into the working directory
        if (!m_Specification.WorkingDirectory.empty())
        {
            File workingDir(m_Specification.WorkingDirectory);
            if (workingDir.Exists())
            {
                NEBULA_CORE_LOG_DEBUG("Application changed into working directory: {}",
                                      workingDir.GetAbsolutePath().string());
                std::filesystem::current_path(m_Specification.WorkingDirectory);
            }
            else
            {
                NEBULA_CORE_LOG_WARN("Working directory does not exist! (Did not change)");
            }
        }

        auto res = Window::Create();
        if (res.IsErr())
        {
            NEBULA_LOG_ERROR("Failed to create window: {0}", res.UnwrapErr());
            status = CreationStatus::WINDOW_CREATION_FAILED;
            return;
        }

        m_Window = std::move(res.Unwrap());
        m_Window->SetEventCallback(NEBULA_BIND_EVENT_FN(Application::OnEvent));
        m_RenderSystem = RenderSystem::Create(m_Window);
        if (!m_RenderSystem->Init())
        {
            NEBULA_LOG_ERROR("Failed to initialize RenderSystem");
            status = CreationStatus::RENDER_SYSTEM_CREATION_FAILED;
            return;
        }
        // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
        OwnedPtr<ImGuiLayer*> imguiLayer(new ImGuiLayer(ImGuiBackend::Create(m_Window)));
        m_ImGuiLayer = imguiLayer.Raw();
        PushOverlay(static_cast<OwnedPtr<Layer*>>(imguiLayer)); // Ownership transferred to LayerStack
        status = CreationStatus::SUCCESS;
    }

    Application::~Application()
    {
        NEBULA_PROFILE_SCOPE("Application::~Application");
        m_RenderSystem->Shutdown();
        for (auto& layer : m_LayerStack)
        {
            layer->OnDetach();
        }
        // Deinit logger
        Log::SetInitialized(false);
    }

    void Application::Run()
    {
        TimePoint lastFrameTime = Time::Now();
        while (m_Running)
        {
            Time::s_DeltaTime = Time::ElapsedTime<Milliseconds>(lastFrameTime, Time::Now());
            lastFrameTime     = Time::Now();
            m_Window->BeginFrame();

            m_RenderSystem->StartGraphicsRenderPass();
            for (auto& layer : m_LayerStack)
            {
                layer->OnUpdate();
            }
            m_RenderSystem->EndGraphicsRenderPass();
            m_RenderSystem->BlitSwapchain();

            m_ImGuiLayer->Begin();

            for (auto& layer : m_LayerStack)
            {
                layer->OnUIRender();
            }

            m_ImGuiLayer->End();

            m_RenderSystem->EndFrame();

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
        NEBULA_UNUSED(this, event);
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
