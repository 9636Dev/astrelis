#include "SandboxLayer.hpp"

#include <future>

#include "Astrelis/Core/Application.hpp"
#include "Astrelis/Core/Log.hpp"
#include "Astrelis/Core/Pointer.hpp"
#include "Astrelis/Core/Time.hpp"
#include "Astrelis/Renderer/RenderSystem.hpp"

SandboxLayer::SandboxLayer() { ASTRELIS_LOG_INFO("Sandbox Layer Initializing"); }

SandboxLayer::~SandboxLayer() { ASTRELIS_LOG_INFO("Sandbox Layer Destroyed"); }

void SandboxLayer::OnAttach() { auto& app = Astrelis::Application::Get(); }

void SandboxLayer::OnDetach() {}

void SandboxLayer::OnUpdate() {}

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

    if (m_ImageFuture.valid() && m_ImageFuture.wait_for(std::chrono::seconds(0)) == std::future_status::ready)
    {
        auto image = std::move(m_ImageFuture.get());
        image.Save("capture.png");
    }

    if (ImGui::Button("Capture Frame"))
    {
        Astrelis::FrameCaptureProps props {0, 0};
        m_ImageFuture = Astrelis::Application::Get().GetRenderSystem()->CaptureFrame(props);
    }

    ImGui::End();
}

void SandboxLayer::OnViewportResize(Astrelis::WindowResizedEvent& event) { event.Handled = true; }

void SandboxLayer::OnEvent(Astrelis::Event& event)
{
    Astrelis::EventDispatcher dispatcher(event);
    dispatcher.Dispatch<Astrelis::WindowResizedEvent>(ASTRELIS_BIND_EVENT_FN(SandboxLayer::OnViewportResize));
}
