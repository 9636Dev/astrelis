#include "EditorLayer.hpp"

#include "Astrelis/Core/Base.hpp"

#include "Astrelis/Core/Application.hpp"
#include "Astrelis/Core/Time.hpp"
#include "Astrelis/IO/File.hpp"

#include <imgui.h>
#include <imgui_internal.h>
#include <utility>

namespace AstrelisEditor {
    EditorLayer::EditorLayer(std::string rootDirectory)
        : Layer("EditorLayer"), m_AssetPanel(Astrelis::File(std::move(rootDirectory))),
          m_Renderer2D(
              Astrelis::Application::Get().GetWindow(), Astrelis::Rect2Di(0, 0, 1280, 720)),
          m_Mesh() {
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

        m_Instances.resize(2);
        m_Instances[0].Transform = Astrelis::Mat4f(1.0F);
        m_Instances[0].Color     = Astrelis::Vec3f(1.0F, 0.0F, 0.0F);

        m_Instances[1].Transform = Astrelis::Mat4f(1.0F);
        m_Instances[1].Color     = Astrelis::Vec3f(0.0F, 0.0F, 1.0F);
    }

    void EditorLayer::OnDetach() {
        m_Renderer2D.Shutdown();
    }

    void EditorLayer::OnUpdate() {
        m_Renderer2D.BeginFrame();
        float elapsedTime = static_cast<float>(Astrelis::Time::TimeSinceAppStart());


        m_Instances[0].Transform = Astrelis::Mat4f(1.0F);
        m_Instances[0].Transform.Translated(Astrelis::Vec3f(-0.5F, 0.0F, 0.0F));
        m_Instances[0].Transform.Rotated(elapsedTime, Astrelis::Vec3f(0.0F, 0.0F, 1.0F));

        m_Instances[1].Transform = Astrelis::Mat4f(1.0F);
        m_Instances[1].Transform.Translated(Astrelis::Vec3f(0.5F, 0.0F, 0.0F));
        m_Instances[1].Transform.Rotated(-elapsedTime, Astrelis::Vec3f(0.0F, 0.0F, 1.0F));

        m_Renderer2D.SubmitInstanced(m_Mesh, m_Instances);
        m_Renderer2D.EndFrame();
    }

    void EditorLayer::OnUIRender() {
        // Create docking context and make this the main window

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

        ImGui::Text("Select an object to inspect");


        ImGui::Text("FPS: %f", ImGui::GetIO().Framerate);
        static bool vsync = Astrelis::Application::Get().GetWindow()->IsVSync();
        if (ImGui::Button("Toggle VSync")) {
            vsync = !vsync;
            Astrelis::Application::Get().GetWindow()->SetVSync(vsync);
        }

        if (m_CaptureFuture.valid()
            && m_CaptureFuture.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
            auto image = m_CaptureFuture.get();
            image.Save("capture.png");
        }

        if (ImGui::Button("Capture Frame")) {
            Astrelis::FrameCaptureProps props = {0, 0};
            m_CaptureFuture = Astrelis::Application::Get().GetRenderSystem()->CaptureFrame(props);
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
