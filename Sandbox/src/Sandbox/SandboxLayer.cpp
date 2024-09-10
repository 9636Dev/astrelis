#include "SandboxLayer.hpp"

#include <future>

#include "Astrelis/Core/Application.hpp"
#include "Astrelis/Core/Log.hpp"
#include "Astrelis/Core/Pointer.hpp"
#include "Astrelis/Core/Profiler.hpp"
#include "Astrelis/Core/Time.hpp"
#include "Astrelis/Renderer/RenderSystem.hpp"
#include "Astrelis/Scene/TransformComponent.hpp"

SandboxLayer::SandboxLayer() { ASTRELIS_LOG_INFO("Sandbox Layer Initializing"); }

SandboxLayer::~SandboxLayer() { ASTRELIS_LOG_INFO("Sandbox Layer Destroyed"); }

void SandboxLayer::OnAttach()
{
    auto& app = Astrelis::Application::Get();
    ASTRELIS_PROFILE_SCOPE("SandboxLayer::OnAttach");
    auto viewportBounds = app.GetWindow()->GetViewportBounds();
    if (m_Renderer == nullptr)
    {
        m_Renderer = std::move(Astrelis::ScopedPtr<Astrelis::Renderer2D>::Create(app.GetWindow(), viewportBounds));
    }
    m_Renderer->Init();

    auto entity = m_Scene.CreateEntity();
    m_Scene.AddComponent(entity, Astrelis::TransformComponent {
                                     Astrelis::Math::Translate(Astrelis::Mat4f(1.0F), Astrelis::Vec3f(0.0F, 0.0F, 0.1F))});
    m_Scene.SetDirty(true);
}

void SandboxLayer::OnDetach()
{
    ASTRELIS_PROFILE_SCOPE("SandboxLayer::OnDetach");
    m_Renderer->Shutdown();
}

void SandboxLayer::OnUpdate()
{
    m_Renderer->BeginFrame();

    Astrelis::TimePoint start = Astrelis::Time::Now();
    m_Renderer->RenderScene(m_Scene, m_Camera);
    m_CpuTime = Astrelis::Time::ElapsedTime<Astrelis::Milliseconds>(start, Astrelis::Time::Now());

    m_Renderer->EndFrame();
}

void SandboxLayer::OnUIRender()
{
    ImGui::Begin("Debug Info");
    ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);

    auto deltaTime       = Astrelis::Time::DeltaTimeMillis();
    auto gpuTime         = deltaTime - m_CpuTime;
    double gpuPercentage = (gpuTime / deltaTime) * 100.0;
    double cpuPercentage = (m_CpuTime / deltaTime) * 100.0;
    ImGui::Text("CPU Time: %.2fms (%.2f%%)", static_cast<double>(m_CpuTime), cpuPercentage);
    ImGui::Text("GPU Time: %.2fms (%.2f%%)", static_cast<double>(gpuTime), gpuPercentage);
    ImGui::Text("Total Time: %.2fms (%.1f FPS)", static_cast<double>(deltaTime), 1000.0 / deltaTime);

    if (m_ImageCapture.valid() && m_ImageCapture.wait_for(std::chrono::seconds(0)) == std::future_status::ready)
    {
        auto image = m_ImageCapture.get();
        image.Save("capture.png");
    }

    if (ImGui::Button("Capture Frame"))
    {
        Astrelis::FrameCaptureProps props {0, 0};
        m_ImageCapture = Astrelis::Application::Get().GetRenderSystem()->CaptureFrame(props);
    }

    auto& window = Astrelis::Application::Get().GetWindow();
    if (window->IsVSync())
    {
        if (ImGui::Button("Disable VSync"))
        {
            window->SetVSync(false);
        }
    }
    else
    {
        if (ImGui::Button("Enable VSync"))
        {
            window->SetVSync(true);
        }
    }

    ImGui::End();
}

void SandboxLayer::OnViewportResize(Astrelis::WindowResizedEvent& event)
{
    m_Renderer->ResizeViewport();
    event.Handled = true;
}

void SandboxLayer::OnEvent(Astrelis::Event& event)
{
    Astrelis::EventDispatcher dispatcher(event);
    dispatcher.Dispatch<Astrelis::WindowResizedEvent>(ASTRELIS_BIND_EVENT_FN(SandboxLayer::OnViewportResize));
}
