#include "NebulaCore/Log.hpp"
#include "NebulaRenderer/Renderer.hpp"
#include "NebulaRenderer/UI/UI.hpp"
#include "NebulaRenderer/Windowing/Window.hpp"

#include <imgui.h>

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
    Nebula::Log::Init();
    NEBULA_CORE_LOG_INFO("Starting Nebula Application");

    Nebula::WindowProps props("Nebula", 1'280, 720);
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
        ImGui::DockSpaceOverViewport();

        // When we render, we set the FBO
        renderer->Render();

        ImGui::Begin("Hello, world!");

        ImGui::Text("This is some useful text.");

        ImGui::End();

        ImGui::Begin("Another window");

        ImGui::Text("Hello, world!");

        ImGui::End();


        Nebula::UI::EndFrame();
        window->PollEvents();
    }

    renderer.Reset();
    window.Reset();

    NEBULA_CORE_LOG_INFO("Shutting down Nebula Application");
    return 0;
}
