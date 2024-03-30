#include "NebulaCore/Log.hpp"
#include "NebulaGraphicsCore/Window.hpp"
#include "NebulaGraphicsRenderer/Window.hpp"

int main(int argc, char** argv)
{
    Nebula::Log::Init();
    NEB_CORE_LOG_INFO("Hello from NebulaCore!");

    Nebula::Window* window = Nebula::CreateGLFWWindow("lib/NebulaGraphicsOpenGL.dll");
    while (!window->ShouldClose())
    {
        window->SwapBuffers();
        window->PollEvents();

    }

    return 0;
}
