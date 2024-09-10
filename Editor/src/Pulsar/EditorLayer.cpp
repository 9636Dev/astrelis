#include "EditorLayer.hpp"

#include "Astrelis/Core/Application.hpp"
#include "Astrelis/Core/Geometry.hpp"
#include "Astrelis/Core/Log.hpp"
#include "Astrelis/Core/Time.hpp"
#include "Astrelis/Scene/TransformComponent.hpp"

#include <imgui.h>
#include <imgui_internal.h>

#include <utility>

namespace Pulsar
{
    const Astrelis::Rect2D renderResolution(0, 0, 1'280, 720);

    EditorLayer::EditorLayer(std::string rootDirectory) :
        Layer("EditorLayer"),
        m_AssetPanel(Astrelis::File(std::move(rootDirectory))),
        m_GamePreview(Astrelis::Application::Get().GetWindow(), renderResolution)
    {
    }

    EditorLayer::~EditorLayer() {}

    void EditorLayer::OnAttach()
    {
        // Don't store imgui.ini settings
        ImGui::GetIO().IniFilename = nullptr;
        auto entity = m_Scene.CreateEntity();
        m_Scene.AddComponent(entity, Astrelis::TransformComponent());
    }

    void EditorLayer::OnDetach() {}

    void EditorLayer::OnUpdate() { m_GamePreview.RenderScene(m_Scene); }

    void EditorLayer::OnUIRender()
    {
        ImGuiID dockspaceId     = ImGui::DockSpaceOverViewport();
        static bool initialized = false;
        if (!initialized)
        {
            auto inspector = ImGui::DockBuilderSplitNode(dockspaceId, ImGuiDir_Right, 0.2F, nullptr, &dockspaceId);
            ImGui::DockBuilderDockWindow("Inspector", inspector);
            auto bottomPanel = ImGui::DockBuilderSplitNode(dockspaceId, ImGuiDir_Down, 0.2F, nullptr, &dockspaceId);
            ImGui::DockBuilderDockWindow("Assets", bottomPanel);
            ImGui::DockBuilderDockWindow("Console", bottomPanel);
            auto hierarchy = ImGui::DockBuilderSplitNode(dockspaceId, ImGuiDir_Left, 0.2F, nullptr, &dockspaceId);
            ImGui::DockBuilderDockWindow("Hierarchy", hierarchy);
            // Remaiing space is for the editor
            ImGui::DockBuilderDockWindow("Editor", dockspaceId);
            initialized = true;
        }

        m_GamePreview.Render();

        ImGui::Begin("Inspector");

        ImGui::Text("Select an object to inspect");

        ImGui::End();

        m_AssetPanel.Draw();

        m_Console.Render();

        ImGui::Begin("Hierarchy");

        ImGui::Text("Object 1");

        ImGui::End();
    }

    void EditorLayer::OnEvent(Astrelis::Event& event) { (void)event; }
} // namespace Pulsar
