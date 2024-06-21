#pragma once

#include "Application.hpp"
#include "NebulaEngine/Core/Profiler.hpp"

//extern Nebula::Ptr<Nebula::Application> Nebula::CreateApplication(CommandLineArguments args);

// NOLINTNEXTLINE(misc-definitions-in-headers)
int main(int argc, char** argv)
{
    NEBULA_PROFILE_START_SESSION("Startup", "profiles/NebulaProfileStartup.json");
    Nebula::CommandLineArguments args          = Nebula::CommandLineArguments::Parse(argc, argv);
    Nebula::ScopedPtr<Nebula::Application> app = Nebula::CreateApplication(args);
    NEBULA_PROFILE_END_SESSION();

    NEBULA_PROFILE_START_SESSION("Runtime", "profiles/NebulaProfileRun.json");
    app->Run();
    NEBULA_PROFILE_END_SESSION();

    NEBULA_PROFILE_START_SESSION("Shutdown", "profiles/NebulaProfileShutdown.json");
    app.Reset();
    NEBULA_PROFILE_END_SESSION();

    return 0;
}
