/**
* @file Entrypoint.hpp
* @brief The entrypoint of the application, the user should include this file instead of specifying a 'main' function, or defining 'ASTRELIS_ENTRYPOINT_NO_MAIN', and instead calling 'AstrelisMain(int argc, char** argv)'
*/

#pragma once

#include "Application.hpp"
#include "Log.hpp"
#include "Profiler.hpp"

//extern Astrelis::Ptr<Astrelis::Application> Astrelis::CreateApplication(CommandLineArguments args);

inline int AstrelisMain(int argc, char** argv)
{
    Astrelis::CreationStatus status     = Astrelis::CreationStatus::SUCCESS;
    Astrelis::CommandLineArguments args = Astrelis::CommandLineArguments::Parse(argc, argv);
    Astrelis::ScopedPtr<Astrelis::Application> app;
    app = Astrelis::CreateApplication(args, status);

    if (status != Astrelis::CreationStatus::SUCCESS)
    {
        ASTRELIS_CORE_LOG_ERROR("Failed to create application: {0}", static_cast<std::uint16_t>(status));
        return -1;
    }
    app->Run();
    app.Reset();

    return 0;
}

#ifndef ASTRELIS_ENTRYPOINT_NO_MAIN
// NOLINTNEXTLINE(misc-definitions-in-headers)
int main(int argc, char** argv) { return AstrelisMain(argc, argv); }
#endif
