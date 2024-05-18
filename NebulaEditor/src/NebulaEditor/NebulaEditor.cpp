#include "NebulaCore/Log/Log.hpp"
#include "NebulaCore/Profile/Profile.hpp"
#include "NebulaWindowing/Window.hpp"

int main(int argc, char** argv)
{
    Nebula::Log::Init();
    NEBULA_PROFILE_ADD_INSTRUMENTORS();
    NEBULA_PROFILE_RECURSION_LIMIT(1);
    NEBULA_PROFILE_BEGIN_SESSION("Nebula Editor", "NebulaEditorProfile.json");
    NEB_CORE_LOG_INFO("Nebula Editor Started");

    gsl::owner<Nebula::Window*> window = Nebula::CreateWindow();

    if (!window->IsOk())
    {
        NEB_CORE_LOG_ERROR("Failed to create window");
        return -1;
    }

    while (!window->ShouldClose())
    {
        window->SwapBuffers();

        window->PollEvents();
    }

    delete window;

    NEB_CORE_LOG_INFO("Nebula Editor Shutting Down");
    NEBULA_PROFILE_END_SESSION();
    return 0;
}
