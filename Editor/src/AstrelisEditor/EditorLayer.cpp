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
        m_Instances[0].Transform =
            Astrelis::Math::Translate(Astrelis::Mat4f(1.0F), Astrelis::Vec3f(-0.5F, 0.0F, 0.0F));
        m_Instances[0].Color = Astrelis::Vec3f(1.0F, 0.0F, 0.0F);

        m_Instances[1].Transform =
            Astrelis::Math::Translate(Astrelis::Mat4f(1.0F), Astrelis::Vec3f(0.5F, 0.0F, 0.0F));
        m_Instances[1].Color = Astrelis::Vec3f(0.0F, 0.0F, 1.0F);
    }

    void EditorLayer::OnDetach() {
        m_Renderer2D.Shutdown();
    }

    void EditorLayer::OnUpdate() {
        m_ElapsedTime += static_cast<float>(Astrelis::Time::DeltaTime());
        m_Renderer2D.BeginFrame();

        m_Instances[0].Transform = Astrelis::Math::Rotate(
            Astrelis::Math::Translate(Astrelis::Mat4f(1.0F), Astrelis::Vec3f(-0.5F, 0.0F, 0.0F)),
            m_ElapsedTime, Astrelis::Vec3f(0.0F, 0.0F, -1.0F));
        m_Instances[1].Transform = Astrelis::Math::Rotate(
            Astrelis::Math::Translate(Astrelis::Mat4f(1.0F), Astrelis::Vec3f(0.5F, 0.0F, 0.0F)),
            m_ElapsedTime, Astrelis::Vec3f(0.0F, 0.0F, 1.0F));

        m_Renderer2D.SubmitInstanced(m_Mesh, m_Instances);
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
    }

    void EditorLayer::OnEvent(Astrelis::Event& event) {
        ASTRELIS_UNUSED(event);
    }
} // namespace AstrelisEditor
