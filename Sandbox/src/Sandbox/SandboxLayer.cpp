#include "SandboxLayer.hpp"

#include "NebulaEngine/Core/Application.hpp"
#include "NebulaEngine/Core/Log.hpp"
#include "NebulaEngine/Core/Time.hpp"
#include "NebulaEngine/Core/Pointer.hpp"
#include "NebulaEngine/Core/Profiler.hpp"
#include "NebulaEngine/Scene/TransformComponent.hpp"

#include <glm/ext/matrix_transform.hpp>
#include <random>

SandboxLayer::SandboxLayer() { NEBULA_LOG_INFO("Sandbox Layer Initializing"); }

SandboxLayer::~SandboxLayer() { NEBULA_LOG_INFO("Sandbox Layer Destroyed"); }

void SandboxLayer::OnAttach()
{
    auto& app    = Nebula::Application::Get();
    NEBULA_PROFILE_SCOPE("SandboxLayer::OnAttach");
    if (m_Renderer2D == nullptr)
    {
        m_Renderer2D = std::move(
            Nebula::ScopedPtr<Nebula::Renderer2D>::Create(app.GetWindow(), app.GetWindow()->GetViewportBounds()));
    }
    m_Renderer2D->Init();

    m_Camera.SetViewMatrix(glm::translate(glm::mat4(1.0F), glm::vec3(-0.3F, 0.0F, 0.0F)));
    m_Camera2.SetViewMatrix(glm::translate(glm::mat4(1.0F), glm::vec3(0.3F, 0.0F, 0.0F)));
}

void SandboxLayer::OnDetach()
{
    NEBULA_PROFILE_SCOPE("SandboxLayer::OnDetach");
    m_Renderer2D->Shutdown();
}

void SandboxLayer::OnUpdate()
{
    m_Renderer2D->BeginFrame();

    Nebula::TimePoint start = Nebula::Time::Now();
    m_Renderer2D->RenderScene(m_Scene, m_Camera);
    m_CpuTime = Nebula::Time::ElapsedTime<Nebula::Milliseconds>(start, Nebula::Time::Now());

    m_Renderer2D->EndFrame();
}

void SandboxLayer::OnUIRender()
{
    ImGui::Begin("Debug Info");
    ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
    static float translation1 = 0.0F;
    ImGui::SliderFloat("Translation 1", &translation1, -1.0F, 1.0F);
    m_Camera.SetViewMatrix(glm::translate(glm::mat4(1.0F), glm::vec3(translation1, 0.0F, 0.0F)));

    static int count = 0;
    ImGui::InputInt("Count", &count);
    if (ImGui::Button("Add Quad"))
    {
        std::random_device rd;
        // Uniform dist -1 to 1
        std::uniform_real_distribution<float> dist(-1.0F, 1.0F);
        for (int i = 0; i < count; i++)
        {
            auto entity = m_Scene.CreateEntity();
            m_Scene.AddComponent<Nebula::TransformComponent>(entity, {glm::translate(glm::mat4(1.0F), glm::vec3(dist(rd), dist(rd), 0.0F))});
        }
    }

    auto deltaTime = Nebula::Time::DeltaTimeMillis();
    auto gpuTime = deltaTime - m_CpuTime;
    double gpuPercentage = (gpuTime / deltaTime) * 100.0;
    double cpuPercentage = (m_CpuTime / deltaTime) * 100.0;
    ImGui::Text("CPU Time: %.2fms (%.2f%%)", static_cast<double>(m_CpuTime), cpuPercentage);
    ImGui::Text("GPU Time: %.2fms (%.2f%%)", static_cast<double>(gpuTime), gpuPercentage);
    ImGui::Text("Total Time: %.2fms (%.1f FPS)", static_cast<double>(deltaTime), 1000.0 / deltaTime);

    ImGui::End();
}

void SandboxLayer::OnViewportResize(Nebula::WindowResizedEvent& event)
{
    m_Renderer2D->ResizeViewport();
    event.Handled = true;
}

void SandboxLayer::OnEvent(Nebula::Event& event)
{
    Nebula::EventDispatcher dispatcher(event);
    dispatcher.Dispatch<Nebula::WindowResizedEvent>(NEBULA_BIND_EVENT_FN(SandboxLayer::OnViewportResize));
}
