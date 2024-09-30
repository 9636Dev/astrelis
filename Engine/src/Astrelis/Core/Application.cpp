#include "Application.hpp"

#include "Astrelis/Core/Base.hpp"

#include "Astrelis/Core/GlobalConfig.hpp"
#include "Astrelis/Events/WindowEvent.hpp"
#include "Astrelis/IO/File.hpp"
#include "Astrelis/UI/ImGui/ImGuiBackend.hpp"
#include "Astrelis/UI/ImGui/ImGuiLayer.hpp"

#include <csignal>
#include <filesystem>
#include <utility>

#include "Time.hpp"
#include "Window.hpp"

namespace Astrelis {
    void ApplicationSignalHandler(int signal) {
        static std::uint8_t signalCount = 0;
        ASTRELIS_LOG_DEBUG("Signal received: {0}", signal);
        ASTRELIS_CORE_ASSERT(Application::HasInstance(), "Application instance does not exist");
        Application::Get().m_Running = false;
        if (signalCount++ > 2) {
            ASTRELIS_LOG_ERROR("Received signal too many times, forcing exit");
            // NOLINTNEXTLINE(concurrency-mt-unsafe)
            std::exit(signal);
        }
    }

    Application* Application::s_Instance = nullptr;

    Application::Application(ApplicationSpecification specification, CreationStatus& status)
        : m_Specification(std::move(specification)), m_Window(nullptr), m_ImGuiLayer(nullptr) {
        ASTRELIS_PROFILE_FUNCTION();
        ASTRELIS_CORE_VERIFY(Astrelis::Log::Init(), "Logger failed to initialize");
        ASTRELIS_CORE_VERIFY(
            s_Instance == nullptr, "Application already exists (Should be singleton)");
        s_Instance = this;

        // Register signal handler
        ASTRELIS_CORE_VERIFYN(std::signal(SIGINT, ApplicationSignalHandler) != SIG_ERR,
            "Failed to register signal handler for SIGINT");
        ASTRELIS_CORE_VERIFYN(std::signal(SIGTERM, ApplicationSignalHandler) != SIG_ERR,
            "Failed to register signal handler for SIGTERM");
#ifdef ASTRELIS_PLATFORM_WINDOWS
        ASTRELIS_CORE_VERIFYN(std::signal(SIGBREAK, SignalHandler) != SIG_ERR,
            "Failed to register signal handler for SIGBREAK");
#endif

        // First we chdir into the working directory
        if (!m_Specification.WorkingDirectory.empty()) {
            File workingDir(m_Specification.WorkingDirectory);
            if (workingDir.Exists()) {
                ASTRELIS_CORE_LOG_DEBUG("Application changed into working directory: {}",
                    workingDir.GetAbsolutePath().string());
                std::filesystem::current_path(m_Specification.WorkingDirectory);
            }
            else {
                ASTRELIS_CORE_LOG_WARN("Working directory does not exist! (Did not change)");
            }
        }

        {
            ASTRELIS_PROFILE_SCOPE("Setup Window");
            ASTRELIS_CORE_ASSERT(!m_Specification.Name.empty(), "Application name cannot be empty");
            auto res = Window::Create(WindowProps(m_Specification.Name));
            if (res.IsErr()) {
                ASTRELIS_LOG_ERROR("Failed to create window: {0}", res.UnwrapErr());
                status = CreationStatus::WINDOW_CREATION_FAILED;
                return;
            }

            m_Window = std::move(res.Unwrap());
            ASTRELIS_CORE_ASSERT(m_Window != nullptr, "Window is nullptr, but no error was thrown");
            m_Window->SetEventCallback(ASTRELIS_BIND_EVENT_FN(Application::OnEvent));
        }
        {
            ASTRELIS_PROFILE_SCOPE("Setup RenderSystem");
            m_RenderSystem = RenderSystem::Create(m_Window);
            auto res       = m_RenderSystem->Init();
            if (res.IsErr()) {
                ASTRELIS_LOG_ERROR("Failed to initialize RenderSystem: {0}", res.UnwrapErr());
                status = CreationStatus::RENDER_SYSTEM_CREATION_FAILED;
                return;
            }
        }

        {
            ASTRELIS_PROFILE_SCOPE("Setup ImGuiLayer");
            // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
            OwnedPtr<ImGuiLayer*> imguiLayer(new ImGuiLayer(ImGuiBackend::Create(m_Window)));
            m_ImGuiLayer = imguiLayer.Raw();
            PushOverlay(
                static_cast<OwnedPtr<Layer*>>(imguiLayer)); // Ownership transferred to LayerStack
            status = CreationStatus::SUCCESS;
        }

        ASTRELIS_CORE_LOG_INFO("Application created successfully");
        ASTRELIS_CORE_LOG_DEBUG("Using Astrelis version: {0}", ASTRELIS_VERSION_STRING);
    }

    Application::~Application() {
        ASTRELIS_PROFILE_FUNCTION();
        ASTRELIS_CORE_VERIFYN(
            std::signal(SIGINT, SIG_DFL) != SIG_ERR, "Failed to disable signal handler for SIGINT");
        ASTRELIS_CORE_VERIFYN(std::signal(SIGTERM, SIG_DFL) != SIG_ERR,
            "Failed to disable signal handler for SIGTERM");
#ifdef ASTRELIS_PLATFORM_WINDOWS
        ASTRELIS_CORE_VERIFYN(std::signal(SIGBREAK, SIG_DFL) != SIG_ERR,
            "Failed to disable signal handler for SIGBREAK");
#endif

        m_RenderSystem->Shutdown();
        for (auto& layer : m_LayerStack) {
            layer->OnDetach();
        }
        // Deinit logger, restarting the app is undefined behaviour
        Log::SetInitialized(false);
    }

    void Application::Run() {
        ASTRELIS_CORE_VERIFY(m_Running, "Application is not running");
        ASTRELIS_CORE_VERIFY(m_Window != nullptr, "Window is nullptr");
        ASTRELIS_CORE_VERIFY(m_RenderSystem != nullptr, "RenderSystem is nullptr");

        if (GlobalConfig::IsDebugMode()) {
            if (m_LayerStack.Size() == 0) {
                ASTRELIS_LOG_WARN(
                    "No layers have been pushed onto the layer stack, use layers to interact with the application");
            }
        }

        TimePoint appStartTime  = Time::Now();
        TimePoint lastFrameTime = appStartTime;
        while (m_Running) {
            ASTRELIS_PROFILE_SCOPE("Run Frame");
            Time::s_DeltaTime = Time::ElapsedTime<Milliseconds>(lastFrameTime, Time::Now());
            lastFrameTime     = Time::Now();
            Time::s_TimeSinceAppStart =
                Time::ElapsedTime<Milliseconds>(appStartTime, lastFrameTime);

            m_Window->BeginFrame();

            m_RenderSystem->StartGraphicsRenderPass();
            {
                ASTRELIS_PROFILE_SCOPE("Update Layers");
                for (auto& layer : m_LayerStack) {
                    layer->OnUpdate();
                }
            }
            m_RenderSystem->EndGraphicsRenderPass();
            m_RenderSystem->BlitSwapchain();

            m_ImGuiLayer->Begin();

            {
                ASTRELIS_PROFILE_SCOPE("Render UI");
                for (auto& layer : m_LayerStack) {
                    layer->OnUIRender();
                }
            }

            m_ImGuiLayer->End();

            m_RenderSystem->EndFrame();

            m_Window->EndFrame();
            m_Window->OnUpdate();

            ASTRELIS_PROFILE_END_FRAME();
        }
    }

    void Application::OnEvent(Event& event) {
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<WindowCloseEvent>(ASTRELIS_BIND_EVENT_FN(Application::OnWindowClose));
        dispatcher.Dispatch<ViewportResizedEvent>(
            ASTRELIS_BIND_EVENT_FN(Application::OnViewportResize));

        for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
            if (event.Handled) {
                break;
            }

            ASTRELIS_CORE_ASSERT(*it != nullptr, "Layer is nullptr");
            (*--it)->OnEvent(event);
        }
    }

    bool Application::OnWindowClose(WindowCloseEvent& event) {
        ASTRELIS_UNUSED(event);
        m_Running = false;
        return true;
    }

    bool Application::OnViewportResize(ViewportResizedEvent& event) {
        ASTRELIS_UNUSED(this, event);
        return false;
    }

    void Application::PushLayer(OwnedPtr<Layer*> layer) {
        ASTRELIS_PROFILE_FUNCTION();
        layer->OnAttach();
        m_LayerStack.PushLayer(std::move(layer));
    }

    void Application::PushOverlay(OwnedPtr<Layer*> overlay) {
        ASTRELIS_PROFILE_FUNCTION();
        overlay->OnAttach();
        m_LayerStack.PushOverlay(std::move(overlay));
    }

    OwnedPtr<Layer*> Application::PopLayer(RawRef<Layer*> layer) {
        ASTRELIS_PROFILE_FUNCTION();
        layer->OnDetach();
        return m_LayerStack.PopLayer(std::move(layer));
    }

    OwnedPtr<Layer*> Application::PopOverlay(RawRef<Layer*> overlay) {
        ASTRELIS_PROFILE_FUNCTION();
        overlay->OnDetach();
        return m_LayerStack.PopOverlay(std::move(overlay));
    }
} // namespace Astrelis
