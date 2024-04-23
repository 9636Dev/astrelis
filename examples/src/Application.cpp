#include "NebulaCore/Log.hpp"
#include "NebulaGraphicsCore/Window.hpp"
#include "NebulaGraphicsRenderer/Renderer.hpp"
#include "NebulaGraphicsRenderer/Window.hpp"

int main(int argc, char** argv)
{
    Nebula::Log::Init();
    NEB_CORE_LOG_INFO("Hello from NebulaCore!");

#define EXAMPLES_USE_OPENGL
#ifdef EXAMPLES_USE_OPENGL
    Nebula::WindowProps<Nebula::OpenGLContext> props("Nebula", 1'080, 720, {4, 1});
    const std::string libraryPath = "lib/NebulaGraphicsOpenGL";
    auto result = Nebula::CreateWindow(libraryPath, props);
    if (result.Error != Nebula::WindowCreationResult::ErrorType::None)
    {
        NEB_CORE_LOG_ERROR("Failed to create window");
        return -1;
    }
#elif defined(EXAMPLES_USE_METAL)
    Nebula::WindowProps<Nebula::MetalContext> props("Nebula", 1'080, 720, {});
    const std::string libraryPath = "lib/NebulaGraphicsMetal";
    auto result = Nebula::CreateWindow(libraryPath, props);
    if (result.Error != Nebula::WindowCreationResult::ErrorType::None)
    {
        NEB_CORE_LOG_ERROR("Failed to create window");
        return -1;
    }

#else
    #error "No graphics API selected"
#endif

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

    // Renderer first, since it might depend on the window
    NEB_CORE_LOG_TRACE("Destroying renderer");
    Nebula::DestroyRenderer(renderer);
    NEB_CORE_LOG_TRACE("Destroying window");
    Nebula::DestroyWindow(window);

    return 0;
}
