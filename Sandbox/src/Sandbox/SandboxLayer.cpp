#include "SandboxLayer.hpp"

#include "NebulaEngine/Core/Application.hpp"
#include "NebulaEngine/Core/Log.hpp"
#include "NebulaEngine/Core/Pointer.hpp"
#include "NebulaEngine/Core/Profiler.hpp"

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
}

void SandboxLayer::OnDetach()
{
    NEBULA_PROFILE_SCOPE("SandboxLayer::OnDetach");
    m_Renderer2D->Shutdown();
}

void SandboxLayer::OnUpdate()
{
    m_Renderer2D->BeginFrame();

    m_Renderer2D->EndFrame();
}

void SandboxLayer::OnUIRender()
{
    // This is not an overlay
}

void SandboxLayer::OnViewportResize(Nebula::WindowResizedEvent& event)
{
    NEBULA_CORE_LOG_INFO("SandboxLayer::OnViewportResize: {0}, {1}", event.GetWidth(), event.GetHeight());
    auto viewport = Nebula::Application::Get().GetWindow()->GetViewportBounds();
    m_Renderer2D->ResizeViewport(viewport);
    event.Handled = true;
}

void SandboxLayer::OnEvent(Nebula::Event& event)
{
    Nebula::EventDispatcher dispatcher(event);
    dispatcher.Dispatch<Nebula::WindowResizedEvent>(NEBULA_BIND_EVENT_FN(SandboxLayer::OnViewportResize));
}
