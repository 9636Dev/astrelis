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
    m_Transform1.Translate({ 0.5F, -0.5F, 0.0F });
    m_Transform2.Translate({ -0.5F, -0.5F, 0.0F });

    m_Transforms.emplace_back();
    m_Transforms.back().Translate({0.5F, 0.5F, 0.0F});
    m_Transforms.emplace_back();
    m_Transforms.back().Translate({-0.5F, 0.5F, 0.0F});

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
    auto& renderer = Nebula::Application::Get().GetRenderer();
    renderer.DrawMesh(m_Mesh, m_Transform1);
    renderer.DrawMesh(m_Mesh, m_Transform2);

    renderer.InstanceMesh(m_Mesh, m_Transforms);
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
