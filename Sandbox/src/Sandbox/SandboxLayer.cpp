#include "SandboxLayer.hpp"

#include "NebulaEngine/Core/Application.hpp"
#include "NebulaEngine/Core/Base.hpp"
#include "NebulaEngine/Core/Time.hpp"
#include "NebulaEngine/Renderer/RenderCommand.hpp"
#include "imgui.h"

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
    NEBULA_LOG_INFO("Sandbox Layer Attached");
}

void SandboxLayer::OnDetach()
{
    NEBULA_LOG_INFO("Sandbox Layer Detached");
}

void SandboxLayer::OnUpdate()
{
    Nebula::RenderCommand::SetClearColor({0.6F, 0.2F, 0.6F, 1.0F});
    Nebula::RenderCommand::Clear();
}

void SandboxLayer::OnUIRender()
{
    ImGui::Begin("Sandbox Layer");

    ImGui::Text("FPS: %lf (Imgui: %.1f)", 1.0 / Nebula::Time::DeltaTime(), ImGui::GetIO().Framerate);

    ImGui::End();
}

void SandboxLayer::OnEvent(Nebula::Event& event)
{
}
