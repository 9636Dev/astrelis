#include <utility>

#include "NebulaEngine/Renderer/RendererAPI.hpp"
#include "SandboxApplication.hpp"

#include "NebulaEngine/Core/Application.hpp"
#include "NebulaEngine/Core/Entrypoint.hpp"

Nebula::ScopedPtr<Nebula::Application> Nebula::CreateApplication(CommandLineArguments args,
                                                                 Nebula::CreationStatus& status)
{
    Nebula::ApplicationSpecification spec;
    spec.Name             = "Sandbox";
    spec.Arguments        = std::move(args);
    spec.WorkingDirectory = "./run";

    Nebula::RendererAPI::SetBufferingMode(Nebula::RendererAPI::BufferingMode::Triple);

    return static_cast<Nebula::ScopedPtr<Nebula::Application>>(
        Nebula::ScopedPtr<SandboxApplication>::Create(spec, status));
}
