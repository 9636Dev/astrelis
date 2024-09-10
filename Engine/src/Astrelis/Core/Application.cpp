#include "Application.hpp"

#include <filesystem>
#include <utility>

#include "Assert.hpp"
#include "Log.hpp"
#include "Profiler.hpp"
#include "Time.hpp"
#include "Utils/Function.hpp"
#include "Window.hpp"

#include "Astrelis/Events/WindowEvent.hpp"
#include "Astrelis/IO/File.hpp"
#include "Astrelis/UI/ImGui/ImGuiBackend.hpp"
#include "Astrelis/UI/ImGui/ImGuiLayer.hpp"

namespace Astrelis
{
    Application* Application::s_Instance = nullptr;

    Application::Application(ApplicationSpecification specification, CreationStatus& status) :
        m_Specification(std::move(specification)),
        m_Window(nullptr),
        m_ImGuiLayer(nullptr)
    {
        ASTRELIS_PROFILE_SCOPE("Astrelis::Application::Application");
        ASTRELIS_VERIFY(Astrelis::Log::Init(), "Logger failed to initialize");
        ASTRELIS_VERIFY(s_Instance == nullptr, "Application already exists (Should be singleton)");
        s_Instance = this;

        // First we chdir into the working directory
        if (!m_Specification.WorkingDirectory.empty())
        {
            File workingDir(m_Specification.WorkingDirectory);
            if (workingDir.Exists())
            {
                ASTRELIS_CORE_LOG_DEBUG("Application changed into working directory: {}",
                                        workingDir.GetAbsolutePath().string());
                std::filesystem::current_path(m_Specification.WorkingDirectory);
            }
            else
            {
                ASTRELIS_CORE_LOG_WARN("Working directory does not exist! (Did not change)");
            }
        }

        {
            ASTRELIS_PROFILE_SCOPE("Astrelis::Application::Application::SetupWindow");
            auto res = Window::Create();
            if (res.IsErr())
            {
                ASTRELIS_LOG_ERROR("Failed to create window: {0}", res.UnwrapErr());
                status = CreationStatus::WINDOW_CREATION_FAILED;
                return;
            }

            m_Window = std::move(res.Unwrap());
            m_Window->SetEventCallback(ASTRELIS_BIND_EVENT_FN(Application::OnEvent));
        }
        {
            ASTRELIS_PROFILE_SCOPE("Astrelis::Application::Application::SetupRenderSystem");
            m_RenderSystem = RenderSystem::Create(m_Window);
            if (!m_RenderSystem->Init())
            {
                ASTRELIS_LOG_ERROR("Failed to initialize RenderSystem");
                status = CreationStatus::RENDER_SYSTEM_CREATION_FAILED;
                return;
            }
        }

        {
            ASTRELIS_PROFILE_SCOPE("Astrelis::Application::Application::SetupImGui");
            // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
            OwnedPtr<ImGuiLayer*> imguiLayer(new ImGuiLayer(ImGuiBackend::Create(m_Window)));
            m_ImGuiLayer = imguiLayer.Raw();
            PushOverlay(static_cast<OwnedPtr<Layer*>>(imguiLayer)); // Ownership transferred to LayerStack
            status = CreationStatus::SUCCESS;
        }
    }

    Application::~Application()
    {
        ASTRELIS_PROFILE_SCOPE("Astrelis::Application::~Application");
        m_RenderSystem->Shutdown();
        for (auto& layer : m_LayerStack)
        {
            layer->OnDetach();
        }
        // Deinit logger, restarting the app is undefined behaviour
        Log::SetInitialized(false);
    }

    void Application::Run()
    {
        TimePoint lastFrameTime = Time::Now();
        while (m_Running)
        {
            ASTRELIS_PROFILE_SCOPE("Astrelis::Application::Run::Frame");
            Time::s_DeltaTime = Time::ElapsedTime<Milliseconds>(lastFrameTime, Time::Now());
            lastFrameTime     = Time::Now();
            m_Window->BeginFrame();

            m_RenderSystem->StartGraphicsRenderPass();
            {
                ASTRELIS_PROFILE_SCOPE("Astrelis::Application::Run::UpdateLayers");
                for (auto& layer : m_LayerStack)
                {
                    layer->OnUpdate();
                }
            }
            m_RenderSystem->EndGraphicsRenderPass();
            m_RenderSystem->BlitSwapchain();

            m_ImGuiLayer->Begin();

            {
                ASTRELIS_PROFILE_SCOPE("Astrelis::Application::Run::UpdateUI");
                for (auto& layer : m_LayerStack)
                {
                    layer->OnUIRender();
                }
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
        dispatcher.Dispatch<WindowClosedEvent>(ASTRELIS_BIND_EVENT_FN(Application::OnWindowClose));
        dispatcher.Dispatch<ViewportResizedEvent>(ASTRELIS_BIND_EVENT_FN(Application::OnViewportResize));

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
        ASTRELIS_UNUSED(this, event);
        return false;
    }

    void Application::PushLayer(OwnedPtr<Layer*> layer)
    {
        ASTRELIS_PROFILE_SCOPE("Astrelis::Application::PushLayer");
        layer->OnAttach();
        m_LayerStack.PushLayer(std::move(layer));
    }

    void Application::PushOverlay(OwnedPtr<Layer*> overlay)
    {
        ASTRELIS_PROFILE_SCOPE("Astrelis::Application::PushOverlay");
        overlay->OnAttach();
        m_LayerStack.PushOverlay(std::move(overlay));
    }

    OwnedPtr<Layer*> Application::PopLayer(RawRef<Layer*> layer)
    {
        ASTRELIS_PROFILE_SCOPE("Astrelis::Application::PopLayer");
        layer->OnDetach();
        return m_LayerStack.PopLayer(std::move(layer));
    }

    OwnedPtr<Layer*> Application::PopOverlay(RawRef<Layer*> overlay)
    {
        ASTRELIS_PROFILE_SCOPE("Astrelis::Application::PopOverlay");
        overlay->OnDetach();
        return m_LayerStack.PopOverlay(std::move(overlay));
    }
} // namespace Astrelis
