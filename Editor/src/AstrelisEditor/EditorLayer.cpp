#include "EditorLayer.hpp"

#include "Astrelis/Core/Base.hpp"

#include "Astrelis/Core/Application.hpp"
#include "Astrelis/IO/File.hpp"

#include <fstream>
#include <imgui.h>
#include <imgui_internal.h>
#include <utility>

#include "ShaderCompiler/ShaderConductor.hpp"

namespace AstrelisEditor {
    EditorLayer::EditorLayer(std::string rootDirectory)
        : Layer("EditorLayer"), m_AssetPanel(Astrelis::File(std::move(rootDirectory))),
          m_Renderer2D(
              Astrelis::Application::Get().GetWindow(), Astrelis::Rect2Di(0, 0, 1280, 720)) {
    }

    EditorLayer::~EditorLayer() {
    }

    void EditorLayer::OnAttach() {
        // Don't store imgui.ini settings
        ImGui::GetIO().IniFilename = nullptr;
        if (!m_Renderer2D.Init()) {
            ASTRELIS_LOG_ERROR("Failed to initialize Renderer2D");
        }
    }

    void EditorLayer::OnDetach() {
        m_Renderer2D.Shutdown();
    }

    void EditorLayer::OnUpdate() {
        m_Renderer2D.BeginFrame();
        m_Renderer2D.AddInstance(Astrelis::InstanceData {Astrelis::Mat4f(1.0F)});
        m_Renderer2D.EndFrame();
    }

    void EditorLayer::OnUIRender() {
        ImGuiID     dockspaceId = ImGui::DockSpaceOverViewport();
        static bool initialized = false;
        if (!initialized) {
            auto inspector = ImGui::DockBuilderSplitNode(
                dockspaceId, ImGuiDir_Right, 0.2F, nullptr, &dockspaceId);
            ImGui::DockBuilderDockWindow("Inspector", inspector);
            auto bottomPanel = ImGui::DockBuilderSplitNode(
                dockspaceId, ImGuiDir_Down, 0.2F, nullptr, &dockspaceId);
            ImGui::DockBuilderDockWindow("Assets", bottomPanel);
            ImGui::DockBuilderDockWindow("Console", bottomPanel);
            auto hierarchy = ImGui::DockBuilderSplitNode(
                dockspaceId, ImGuiDir_Left, 0.2F, nullptr, &dockspaceId);
            ImGui::DockBuilderDockWindow("Hierarchy", hierarchy);
            // Remaiing space is for the editor
            ImGui::DockBuilderDockWindow("Editor", dockspaceId);
            initialized = true;
        }

        ImGui::Begin("Inspector");

        ImGui::Text("FPS: %f", ImGui::GetIO().Framerate);
        static bool vsync = Astrelis::Application::Get().GetWindow()->IsVSync();
        if (ImGui::Button("Toggle VSync")) {
            vsync = !vsync;
            Astrelis::Application::Get().GetWindow()->SetVSync(vsync);
        }

        ImGui::End();

        m_AssetPanel.Draw();

        m_Console.Render();

        ImGui::Begin("Hierarchy");

        ImGui::Text("Object 1");

        ImGui::End();

        ImGui::Begin("Editor");

        // We render the image, by getting it from RenderSystem
        ImGui::Image(
            Astrelis::Application::Get().GetRenderSystem()->GetGraphicsImage(), ImVec2(1280, 720));

        ImGui::End();
    }

    void EditorLayer::OnEvent(Astrelis::Event& event) {
        ASTRELIS_UNUSED(event);
    }
} // namespace AstrelisEditor
