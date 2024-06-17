#include <utility>

#include "SandboxApplication.hpp"

#include "NebulaEngine/Core/Entrypoint.hpp"
#include "NebulaEngine/Core/Application.hpp"

Nebula::ScopedPtr<Nebula::Application> Nebula::CreateApplication(CommandLineArguments args)
{
    Nebula::ApplicationSpecification spec;
    spec.Name = "Sandbox";
    spec.Arguments = std::move(args);
    spec.WorkingDirectory = "../";

    return Nebula::ScopedPtr<SandboxApplication>::Create(spec);
}
