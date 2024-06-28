#include "SandboxLayer.hpp"
#include "NebulaEngine/Scene/VoxelScene.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/glm.hpp>

#include "NebulaEngine/Core/Application.hpp"
#include "NebulaEngine/Core/Log.hpp"
#include "NebulaEngine/Core/Pointer.hpp"
#include "NebulaEngine/Core/Profiler.hpp"
#include "NebulaEngine/Core/Time.hpp"

#include <glm/ext/matrix_transform.hpp>

SandboxLayer::SandboxLayer() { NEBULA_LOG_INFO("Sandbox Layer Initializing"); }

SandboxLayer::~SandboxLayer() { NEBULA_LOG_INFO("Sandbox Layer Destroyed"); }

void SandboxLayer::OnAttach()
{
    auto& app = Nebula::Application::Get();
    NEBULA_PROFILE_SCOPE("SandboxLayer::OnAttach");
    auto viewportBounds = app.GetWindow()->GetViewportBounds();
    if (m_Renderer == nullptr)
    {
        m_Renderer = std::move(Nebula::ScopedPtr<Nebula::VoxelRenderer>::Create(app.GetWindow(), viewportBounds));
    }
    m_Renderer->Init();

    m_Camera.SetProjectionMatrix(glm::perspective(
        glm::radians(60.0F), static_cast<float>(viewportBounds.Width) / static_cast<float>(viewportBounds.Height), 0.1F,
        10.0F));

    m_Scene.m_Chunks.push_back({
        {0, 0, 0},
        {
            Nebula::Voxel::Create({0, 0, 0}, {1.0F, 1.0F, 1.0F, 1.0F})
        }
    });
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
    m_CpuTime               = Nebula::Time::ElapsedTime<Nebula::Milliseconds>(start, Nebula::Time::Now());

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
