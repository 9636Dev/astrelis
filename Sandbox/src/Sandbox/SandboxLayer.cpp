#include "SandboxLayer.hpp"

#include "NebulaEngine/Core/Application.hpp"
#include "NebulaEngine/Core/Log.hpp"
#include "NebulaEngine/Core/Pointer.hpp"
#include "NebulaEngine/Core/Profiler.hpp"
#include "NebulaEngine/Core/Time.hpp"
#include "NebulaEngine/Scene/TransformComponent.hpp"

#include <future>

SandboxLayer::SandboxLayer() { NEBULA_LOG_INFO("Sandbox Layer Initializing"); }

SandboxLayer::~SandboxLayer() { NEBULA_LOG_INFO("Sandbox Layer Destroyed"); }

void SandboxLayer::OnAttach()
{
    auto& app = Nebula::Application::Get();
    NEBULA_PROFILE_SCOPE("SandboxLayer::OnAttach");
    auto viewportBounds = app.GetWindow()->GetViewportBounds();
    if (m_Renderer == nullptr)
    {
        m_Renderer = std::move(Nebula::ScopedPtr<Nebula::Renderer2D>::Create(app.GetWindow(), viewportBounds));
    }
    m_Renderer->Init();

    auto entity = m_Scene.CreateEntity();
    m_Scene.AddComponent(entity, Nebula::TransformComponent {
                                     Nebula::Math::Translate(Nebula::Mat4f(1.0F), Nebula::Vec3f(0.0F, 0.0F, 0.1F))});
}

void SandboxLayer::OnDetach()
{
    NEBULA_PROFILE_SCOPE("SandboxLayer::OnDetach");
    m_Renderer->Shutdown();
}

void SandboxLayer::OnUpdate()
{
    m_Renderer->BeginFrame();

    Nebula::TimePoint start = Nebula::Time::Now();
    m_Renderer->RenderScene(m_Scene, m_Camera);
    m_CpuTime = Nebula::Time::ElapsedTime<Nebula::Milliseconds>(start, Nebula::Time::Now());

    m_Renderer->EndFrame();
}

void SandboxLayer::OnUIRender()
{
    ImGui::Begin("Debug Info");
    ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);

    auto deltaTime       = Nebula::Time::DeltaTimeMillis();
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
        m_ImageCapture = Nebula::Application::Get().GetRenderSystem()->CaptureFrame();
    }

    auto& window = Nebula::Application::Get().GetWindow();
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

void SandboxLayer::OnViewportResize(Nebula::WindowResizedEvent& event)
{
    m_Renderer->ResizeViewport();
    event.Handled = true;
}

void SandboxLayer::OnEvent(Nebula::Event& event)
{
    Nebula::EventDispatcher dispatcher(event);
    dispatcher.Dispatch<Nebula::WindowResizedEvent>(NEBULA_BIND_EVENT_FN(SandboxLayer::OnViewportResize));
}
