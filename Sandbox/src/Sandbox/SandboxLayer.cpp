#include "SandboxLayer.hpp"

#include "NebulaEngine/Core/Application.hpp"
#include "NebulaEngine/Core/Log.hpp"
#include "NebulaEngine/Core/Pointer.hpp"
#include "NebulaEngine/Core/Profiler.hpp"

SandboxLayer::SandboxLayer() { NEBULA_LOG_INFO("Sandbox Layer Initializing"); }

SandboxLayer::~SandboxLayer() { NEBULA_LOG_INFO("Sandbox Layer Destroyed"); }

void SandboxLayer::OnAttach()
{
    NEBULA_PROFILE_SCOPE("SandboxLayer::OnAttach");
    if (m_Renderer2D == nullptr)
    {
        auto& app    = Nebula::Application::Get();
        m_Renderer2D = std::move(
            Nebula::ScopedPtr<Nebula::Renderer2D>::Create(app.GetWindow(), app.GetWindow()->GetViewportBounds()));
    }
    m_Renderer2D->Init();
}

void SandboxLayer::OnDetach()
{
    NEBULA_PROFILE_SCOPE("SandboxLayer::OnDetach");
    m_Renderer2D->Shutdown();
}

void SandboxLayer::OnUpdate() {}

void SandboxLayer::OnUIRender()
{
    // This is not an overlay
}

void SandboxLayer::OnEvent(Nebula::Event& event) {}
