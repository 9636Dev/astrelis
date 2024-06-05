#include "SandboxLayer.hpp"

#include "NebulaEngine/Core/Base.hpp"
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
    NEBULA_LOG_INFO("Sandbox Layer Updated");
}

void SandboxLayer::OnUIRender()
{
    ImGui::Begin("Sandbox Layer");
    ImGui::End();
}

void SandboxLayer::OnEvent(Nebula::Event& event)
{
    NEBULA_LOG_INFO("Sandbox Layer Event: {0}", event.ToString());
}
