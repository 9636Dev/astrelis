#include "GamePreview.hpp"

#include <utility>

#include "NebulaEngine/Core/Application.hpp"
#include "NebulaEngine/Core/Log.hpp"
#include <imgui.h>

namespace Pulsar
{
    GamePreview2D::GamePreview2D(Nebula::RefPtr<Nebula::Window> window, Nebula::Rect2Di bounds) :
        m_Renderer(std::move(window), bounds),
        m_Viewport(bounds)
    {
        if (!m_Renderer.Init())
        {
            NEBULA_CORE_LOG_ERROR("Failed to initialize Renderer2D");
        }
    }

    GamePreview2D::~GamePreview2D() { m_Renderer.Shutdown(); }

    void GamePreview2D::RenderScene(Nebula::Scene2D& scene)
    {
        m_Renderer.BeginFrame();
        m_Renderer.RenderScene(scene, m_Camera);
        m_Renderer.EndFrame();
    }

    void GamePreview2D::Render()
    {
        ImGui::Begin("Editor");
        auto imguiPos  = ImGui::GetCursorScreenPos();
        auto imguiSize = ImGui::GetContentRegionAvail();

        ImGui::GetWindowDrawList()->AddImage(Nebula::Application::Get().GetRenderSystem()->GetGraphicsImage(), imguiPos,
                                             ImVec2(imguiPos.x + imguiSize.x, imguiPos.y + imguiSize.y), ImVec2(0, 1),
                                             ImVec2(1, 0));

        ImGui::End();
    }
} // namespace Pulsar
