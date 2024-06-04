#pragma once

#include "Application.hpp"

//extern Nebula::Ptr<Nebula::Application> Nebula::CreateApplication(CommandLineArguments args);

// NOLINTNEXTLINE(misc-definitions-in-headers)
int main(int argc, char** argv)
{
    Nebula::CommandLineArguments args = Nebula::CommandLineArguments::Parse(argc, argv);
    Nebula::Ptr<Nebula::Application> app = Nebula::CreateApplication(args);

    app->Run();

    app.Reset();

    return 0;
}
