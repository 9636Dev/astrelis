#include "Astrelis/Astrelis.hpp"
#include "Astrelis/Core/Entrypoint.hpp"
#include "Astrelis/Renderer/RendererAPI.hpp"

#include <utility>

#include "SandboxApplication.hpp"

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
