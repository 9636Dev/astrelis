#include "SandboxLayer.hpp"

#include "NebulaEngine/Core/Application.hpp"
#include "NebulaEngine/Core/Base.hpp"
#include "imgui.h"

SandboxLayer::SandboxLayer()
{
    m_Mesh.Vertices = {
        { { -0.5F, -0.5F, 0.0F }, { 1.0F, 0.0F, 0.0F } },
        { { 0.5F, -0.5F, 0.0F }, { 0.0F, 1.0F, 0.0F } },
        { { 0.0F, 0.5F, 0.0F }, { 0.0F, 0.0F, 1.0F } },
    };
    m_Mesh.Indices = { 0, 1, 2 };
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
    Nebula::Application::Get().GetRenderer().DrawMesh(m_Mesh, m_Transform);
}

void SandboxLayer::OnUIRender()
{
    ImGui::Begin("Sandbox Layer");

    ImGui::Text("Hello, World!");
    ImGui::End();
}

void SandboxLayer::OnEvent(Nebula::Event& event)
{
    NEBULA_LOG_INFO("Sandbox Layer Event: {0}", event.ToString());
}
