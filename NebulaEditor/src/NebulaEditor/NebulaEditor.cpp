#include "NebulaCore/Log/Log.hpp"
#include "NebulaCore/Profile/Profile.hpp"

int main(int argc, char** argv)
{
    Nebula::Log::Init();
    NEBULA_PROFILE_ADD_INSTRUMENTORS();
    NEBULA_PROFILE_RECURSION_LIMIT(1);
    NEBULA_PROFILE_BEGIN_SESSION("Nebula Editor", "NebulaEditorProfile.json");
    NEB_CORE_LOG_INFO("Nebula Editor Started");


    NEB_CORE_LOG_INFO("Nebula Editor Shutting Down");
    NEBULA_PROFILE_END_SESSION();
    return 0;
}
