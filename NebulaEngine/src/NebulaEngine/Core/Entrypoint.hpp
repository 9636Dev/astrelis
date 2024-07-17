/**
* @file Entrypoint.hpp
* @brief The entrypoint of the application, the user should include this file instead of specifying a 'main' function, or defining 'NEBULA_ENTRYPOINT_NO_MAIN', and instead calling 'NebulaMain(int argc, char** argv)'
*/

#pragma once

#include "Application.hpp"
#include "Log.hpp"
#include "Profiler.hpp"

//extern Nebula::Ptr<Nebula::Application> Nebula::CreateApplication(CommandLineArguments args);

inline int NebulaMain(int argc, char** argv)
{
    NEBULA_PROFILE_START_SESSION("Startup", "profiles/NebulaProfileStartup.json");
    Nebula::CreationStatus status              = Nebula::CreationStatus::SUCCESS;
    Nebula::CommandLineArguments args          = Nebula::CommandLineArguments::Parse(argc, argv);
    Nebula::ScopedPtr<Nebula::Application> app = Nebula::CreateApplication(args, status);

    if (status != Nebula::CreationStatus::SUCCESS)
    {
        NEBULA_CORE_LOG_ERROR("Failed to create application: {0}", static_cast<std::uint16_t>(status));
        return -1;
    }
    NEBULA_PROFILE_END_SESSION();

    NEBULA_PROFILE_START_SESSION("Runtime", "profiles/NebulaProfileRun.json");
    app->Run();
    NEBULA_PROFILE_END_SESSION();

    NEBULA_PROFILE_START_SESSION("Shutdown", "profiles/NebulaProfileShutdown.json");
    app.Reset();
    NEBULA_PROFILE_END_SESSION();

    return 0;
}

#ifndef NEBULA_ENTRYPOINT_NO_MAIN
// NOLINTNEXTLINE(misc-definitions-in-headers)
int main(int argc, char** argv) { return NebulaMain(argc, argv); }
#endif
