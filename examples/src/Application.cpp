#include "NebulaCore/Log.hpp"
#include "NebulaGraphicsCore/Window.hpp"
#include "NebulaGraphicsRenderer/Renderer.hpp"
#include "NebulaGraphicsRenderer/Window.hpp"

int main(int argc, char** argv)
{
    Nebula::Log::Init();
    NEB_CORE_LOG_INFO("Hello from NebulaCore!");

    Nebula::WindowProps<Nebula::OpenGLContext> props("Nebula", 1'080, 720, {4, 5});
    const std::string libraryPath = "lib/NebulaGraphicsOpenGL";
    auto result = Nebula::CreateWindow(libraryPath, props);
    if (result.Error != Nebula::WindowCreationResult::ErrorType::None)
    {
        NEB_CORE_LOG_ERROR("Failed to create window");
        return -1;
    }

    // !IMPORTANT! There must only be one shared pointer to the window when it is destroyed
    auto window = std::move(result.Window);
    auto rendererResult = Nebula::CreateRenderer(libraryPath, window, props.Context);
    if (rendererResult.Error != Nebula::RendererCreationResult::ErrorType::None)
    {
        NEB_CORE_LOG_ERROR("Failed to create renderer");
        return -1;
    }
    auto renderer = std::move(rendererResult.Renderer);
    window->SetVSync(true);
    //window->SetEventCallback([](Nebula::Event& event) { NEB_CORE_LOG_TRACE("{0}", event.ToString()); });

    while (!window->ShouldClose())
    {
        renderer->Render();

        window->PollEvents();
        window->SwapBuffers();
    }

    NEB_CORE_LOG_INFO("Shutting down NebulaCore");

    NEB_CORE_LOG_TRACE("Destroying window");
    Nebula::DestroyWindow(window);
    NEB_CORE_LOG_TRACE("Destroying renderer");
    Nebula::DestroyRenderer(renderer);

    return 0;
}
