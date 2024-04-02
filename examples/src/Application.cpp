#include "NebulaCore/Log.hpp"
#include "NebulaGraphicsCore/Window.hpp"
#include "NebulaGraphicsRenderer/Window.hpp"

int main(int argc, char** argv)
{
    Nebula::Log::Init();
    NEB_CORE_LOG_INFO("Hello from NebulaCore!");

    Nebula::WindowProps<Nebula::OpenGLContext> props("Nebula", 1'080, 720, {4, 5});
    auto result = Nebula::CreateWindow("lib/NebulaGraphicsOpenGL", props);
    if (result.Error != Nebula::WindowCreationResult::ErrorType::None)
    {
        NEB_CORE_LOG_ERROR("Failed to create window");
        return -1;
    }

    // !IMPORTANT! There must only be one shared pointer to the window when it is destroyed
    auto window = std::move(result.Window);
    window->SetVSync(true);
    window->SetEventCallback([](Nebula::Event& event) { NEB_CORE_LOG_TRACE("{0}", event.ToString()); });

    while (!window->ShouldClose())
    {
        window->SwapBuffers();
        window->PollEvents();
    }

    Nebula::DestroyWindow(window);

    return 0;
}
