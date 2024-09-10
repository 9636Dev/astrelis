#include <utility>

#include "Astrelis/Renderer/RendererAPI.hpp"
#include "SandboxApplication.hpp"

#include "Astrelis/Core/Application.hpp"
#include "Astrelis/Core/Entrypoint.hpp"

Astrelis::ScopedPtr<Astrelis::Application> Astrelis::CreateApplication(CommandLineArguments args,
                                                                 Astrelis::CreationStatus& status)
{
    Astrelis::ApplicationSpecification spec;
    spec.Name             = "Sandbox";
    spec.Arguments        = std::move(args);
    spec.WorkingDirectory = "./run";

    Astrelis::RendererAPI::SetBufferingMode(Astrelis::RendererAPI::BufferingMode::Triple);

    return static_cast<Astrelis::ScopedPtr<Astrelis::Application>>(
        Astrelis::ScopedPtr<SandboxApplication>::Create(spec, status));
}
