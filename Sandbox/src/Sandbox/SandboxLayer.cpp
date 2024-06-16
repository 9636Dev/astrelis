#include "SandboxLayer.hpp"

#include "NebulaEngine/Core/Application.hpp"
#include "NebulaEngine/Core/Bounds.hpp"
#include "NebulaEngine/Core/Profiler.hpp"
#include "NebulaEngine/Renderer/RenderCommand.hpp"
#include "NebulaEngine/Renderer/Renderer2D.hpp"

SandboxLayer::SandboxLayer()
{
    NEBULA_LOG_INFO("Sandbox Layer Initializing");
}

SandboxLayer::~SandboxLayer()
{
    NEBULA_LOG_INFO("Sandbox Layer Destroyed");
}

void SandboxLayer::OnAttach()
{
    NEBULA_PROFILE_SCOPE("SandboxLayer::OnAttach");
    Nebula::Renderer2D::Init();
    Nebula::Bounds viewport = Nebula::Application::Get().GetWindow().GetViewportBounds();
    Nebula::Renderer2D::SetViewport(viewport);
}

void SandboxLayer::OnDetach()
{
    NEBULA_PROFILE_SCOPE("SandboxLayer::OnDetach");
    Nebula::Renderer2D::Shutdown();
}

void SandboxLayer::OnUpdate()
{
    Nebula::RenderCommand::SetClearColor({0.6F, 0.2F, 0.6F, 1.0F});
    Nebula::RenderCommand::Clear();

    Nebula::Renderer2D::BeginScene();

    Nebula::Renderer2D::DrawQuad({0.0F, 0.0F}, {1.0F, 1.0F}, {1.0F, 1.0F, 1.0F, 1.0F});

    Nebula::Renderer2D::EndScene();
}

void SandboxLayer::OnUIRender()
{
    // This is not an overlay
}

void SandboxLayer::OnEvent(Nebula::Event& event)
{
}
