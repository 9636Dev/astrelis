#include "NebulaCore/Log.hpp"
#include "NebulaRenderer/Renderer.hpp"
#include "NebulaRenderer/UI/UI.hpp"
#include "NebulaRenderer/Windowing/Window.hpp"

#include <imgui.h>
#include <imgui_internal.h> // For Docking

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
    Nebula::Log::Init();
    NEBULA_CORE_LOG_INFO("Starting Nebula Application");

    constexpr static std::uint32_t width  = 1'280;
    constexpr static std::uint32_t height = 720;
    Nebula::WindowProps props("Nebula", width, height);
    auto creationResult = Nebula::CreateWindow(props);
    if (creationResult.IsErr())
    {
        NEBULA_CORE_LOG_ERROR("Failed to create window: {0}", static_cast<std::uint32_t>(creationResult.UnwrapErr()));
        return -1;
    }

    auto window = std::move(creationResult.Unwrap());
    window->SetEventCallback([](Nebula::Event& event) { NEBULA_CORE_LOG_TRACE("{0}", event.ToString()); });

    auto rendererResult = Nebula::CreateRenderer({
        window.GetRef(), {720, 480}
    });
    if (rendererResult.IsErr())
    {
        NEBULA_CORE_LOG_ERROR("Failed to create renderer: {0}", static_cast<std::uint32_t>(rendererResult.UnwrapErr()));
        return -1;
    }

    Nebula::UI::Init(window.GetRef());
    auto renderer = std::move(rendererResult.Unwrap());
    renderer->SetClearColor(0.1F, 0.1F, 0.1F, 1.0F);


    while (!window->ShouldClose())
    {
        window->SwapBuffers();

        renderer->NewFrame();
        Nebula::UI::BeginFrame();

        auto dockspace_id = ImGui::DockSpaceOverViewport();
        static bool dockspaceSetup = false;
        if (!dockspaceSetup)
        {
            ImGui::DockBuilderRemoveNode(dockspace_id);
            ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace);

            auto propertiesId = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Right, 12.0F, nullptr, &dockspace_id);
            auto assetsId     = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Down, 16.0F, nullptr, &dockspace_id);
            auto objectListId = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Left, 16.0F, nullptr, &dockspace_id);
            ImGui::DockBuilderDockWindow("Assets", assetsId);
            ImGui::DockBuilderDockWindow("Object List", objectListId);
            ImGui::DockBuilderDockWindow("Properties", propertiesId);
            ImGui::DockBuilderDockWindow("Rendered Preview", dockspace_id);
            ImGui::DockBuilderFinish(dockspace_id);

            dockspaceSetup = true;
        }

        ImGui::Begin("Rendered Preview");

        renderer->Render();
        ImGui::End();

        ImGui::Begin("Object List");
        ImGui::Text("Object 1");
        ImGui::End();

        ImGui::Begin("Assets");
        ImGui::Text("Asset 1");
        ImGui::End();

        ImGui::Begin("Properties");
        ImGui::Text("Properties 1");
        ImGui::End();


        Nebula::UI::EndFrame();
        window->PollEvents();
    }

    renderer.Reset();
    window.Reset();

    NEBULA_CORE_LOG_INFO("Shutting down Nebula Application");
    return 0;
}
