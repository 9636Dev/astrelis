#include "EditorLayer.hpp"

#include "Astrelis/Core/Base.hpp"

#include "Astrelis/Core/Application.hpp"
#include "Astrelis/IO/File.hpp"

#include <imgui.h>
#include <imgui_internal.h>
#include <utility>

namespace AstrelisEditor {
    EditorLayer::EditorLayer(std::string rootDirectory)
        : Layer("EditorLayer"), m_AssetPanel(Astrelis::File(std::move(rootDirectory))),
          m_Renderer2D(
              Astrelis::Application::Get().GetWindow(), Astrelis::Rect2Di(0, 0, 1280, 720)),
          m_Mesh(), m_Instance() {
    }

    EditorLayer::~EditorLayer() {
    }

    void EditorLayer::OnAttach() {
        // Don't store imgui.ini settings
        ImGui::GetIO().IniFilename = nullptr;
        if (!m_Renderer2D.Init()) {
            ASTRELIS_LOG_ERROR("Failed to initialize Renderer2D");
        }

        m_Mesh.Vertices = {
            {{-0.5F, -0.5F, 0.0F}, {0.0F, 0.0F}},
            {{0.5F, -0.5F, 0.0F},  {1.0F, 0.0F}},
            {{0.5F, 0.5F, 0.0F},   {1.0F, 1.0F}},
            {{-0.5F, 0.5F, 0.0F},  {0.0F, 1.0F}},
        };

        m_Mesh.Indices = {0, 1, 2, 2, 3, 0};

        m_Instance.Transform = Astrelis::Mat4f(1.0F);
        m_Instance.Color     = Astrelis::Vec3f(1.0F, 0.0F, 0.0F);
    }

    void EditorLayer::OnDetach() {
        m_Renderer2D.Shutdown();
    }

    void EditorLayer::OnUpdate() {
        m_Renderer2D.BeginFrame();
        m_Instance.Transform =
            glm::rotate(m_Instance.Transform, 0.01F, Astrelis::Vec3f(0.0F, 0.0F, 1.0F));
        m_Renderer2D.Submit(m_Mesh, m_Instance);
        m_Renderer2D.EndFrame();
    }

    void EditorLayer::OnUIRender() {
        ImGui::Begin("Debug");

        ImGui::Text("FPS: %f", ImGui::GetIO().Framerate);
        static bool vsync = Astrelis::Application::Get().GetWindow()->IsVSync();
        if (ImGui::Button("Toggle VSync")) {
            vsync = !vsync;
            Astrelis::Application::Get().GetWindow()->SetVSync(vsync);
        }

        ImGui::End();
    }

    void EditorLayer::OnEvent(Astrelis::Event& event) {
        ASTRELIS_UNUSED(event);
    }
} // namespace AstrelisEditor
