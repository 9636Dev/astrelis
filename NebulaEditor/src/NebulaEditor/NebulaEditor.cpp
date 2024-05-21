#include "NebulaCore/Log/Log.hpp"
#include "NebulaCore/Profile/Profile.hpp"
#include "NebulaCore/Util/Pointer.hpp"
#include "NebulaWindowing/Window.hpp"
#include "NebulaWindowing/WindowHelper.hpp"

int main(int argc, char** argv)
{
    Nebula::Log::Init();
    NEBULA_PROFILE_ADD_INSTRUMENTORS();
    NEBULA_PROFILE_RECURSION_LIMIT(1);
    NEBULA_PROFILE_BEGIN_SESSION("Nebula Editor", "NebulaEditorProfile.json");
    NEB_CORE_LOG_INFO("Nebula Editor Started");

    {
        Nebula::WindowProps props;
        props.Title = "Nebula Editor";
        props.Width = 1280;
        props.Height = 720;
        auto result = Nebula::CreateWindow(props);
        if (result.IsErr())
        {
            NEB_CORE_LOG_ERROR("Failed to create window: {0}", static_cast<std::uint32_t>(result.UnwrapErr()));
            return -1;
        }
        auto window = std::move(result.Unwrap());
        window->SetEventCallback([](Nebula::Event& event) {
            NEB_CORE_LOG_TRACE("{0}", event.ToString());
        });

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
    }

    NEB_CORE_LOG_INFO("Nebula Editor Shutting Down");
    NEBULA_PROFILE_END_SESSION();
    return 0;
}
