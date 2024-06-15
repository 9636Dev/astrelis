#pragma once

#include "Application.hpp"
#include "QProfile/Macros.hpp"

//extern Nebula::Ptr<Nebula::Application> Nebula::CreateApplication(CommandLineArguments args);

// NOLINTNEXTLINE(misc-definitions-in-headers)
int main(int argc, char** argv)
{
    QPROFILE_START_SESSION("Startup", "profiles/NebulaProfileStartup.json");
    Nebula::CommandLineArguments args    = Nebula::CommandLineArguments::Parse(argc, argv);
    Nebula::Ptr<Nebula::Application> app = Nebula::CreateApplication(args);
    QPROFILE_END_SESSION();

    QPROFILE_START_SESSION("Runtime", "profiles/NebulaProfileRun.json");
    app->Run();
    QPROFILE_END_SESSION();

    QPROFILE_START_SESSION("Shutdown", "profiles/NebulaProfileShutdown.json");
    app.Reset();
    QPROFILE_END_SESSION();

    return 0;
}
