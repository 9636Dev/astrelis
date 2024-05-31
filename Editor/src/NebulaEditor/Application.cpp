#include "NebulaCore/Log.hpp"
#include "NebulaRenderer/Windowing/Window.hpp"

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
    Nebula::Log::Init();
    NEBULA_CORE_LOG_INFO("Starting Nebula Application");

    Nebula::WindowProps props("Nebula", 1280, 720);
    auto creationResult = Nebula::CreateWindow(props);
    if (creationResult.IsErr())
    {
        NEBULA_CORE_LOG_ERROR("Failed to create window: {0}", static_cast<std::uint32_t>(creationResult.UnwrapErr()));
        return -1;
    }

    auto window = std::move(creationResult.Unwrap());

    while (!window->ShouldClose())
    {
        window->SwapBuffers();
        window->PollEvents();
    }

    window.Reset();

    NEBULA_CORE_LOG_INFO("Shutting down Nebula Application");
    return 0;
}
