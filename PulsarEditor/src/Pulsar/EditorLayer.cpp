#include "EditorLayer.hpp"

#include "NebulaEngine/Core/Application.hpp"
#include "NebulaEngine/Core/Bounds.hpp"
#include "NebulaEngine/Core/Time.hpp"

#include <imgui.h>
#include <imgui_internal.h>

#include <utility>

namespace Pulsar
{
    const Nebula::Bounds renderResolution(0, 0, 1280, 720);

    EditorLayer::EditorLayer(std::string rootDirectory) :
        Layer("EditorLayer"),
        m_AssetPanel(Nebula::File(std::move(rootDirectory))),
        m_GamePreview(Nebula::Application::Get().GetWindow(), renderResolution)
    {
    }

    EditorLayer::~EditorLayer() {}

    void EditorLayer::OnAttach()
    {
        // Don't store imgui.ini settings
        ImGui::GetIO().IniFilename = nullptr;
    }

    void EditorLayer::OnDetach() {}

    void EditorLayer::OnUpdate() {
        m_GamePreview.RenderScene();
    }

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

        ImGui::Begin("Console");

        // Scrollable text area
        ImGui::BeginChild("ConsoleScrollingRegion", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

        ImGui::TextUnformatted("Hello, world!");

        ImGui::EndChild();

        ImGui::End();

        ImGui::Begin("Hierarchy");

        ImGui::Text("Object 1");

        ImGui::End();
    }

    void EditorLayer::OnEvent(Nebula::Event& event) { (void)event; }
} // namespace Pulsar
