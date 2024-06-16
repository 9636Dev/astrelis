#include "EditorLayer.hpp"

#include "NebulaEngine/Core/Time.hpp"
#include "NebulaEngine/Renderer/Renderer2D.hpp"
#include "imgui.h"

EditorLayer::EditorLayer()
    : Layer("EditorLayer")
{
}

EditorLayer::~EditorLayer()
{
}

void EditorLayer::OnAttach()
{
    // We have to initialize a renderer, otherwise we will get a crash
    Nebula::Renderer2D::Init();
}

void EditorLayer::OnDetach()
{
}

void EditorLayer::OnUpdate()
{
}

void EditorLayer::OnUIRender()
{
    ImGui::Begin("Editor");
    ImGui::Text("FPS: %.1f (NE: %.2f)", ImGui::GetIO().Framerate, 1.0F / Nebula::Time::DeltaTime());
    ImGui::End();
}

void EditorLayer::OnEvent(Nebula::Event& event)
{
}
