#include "NebulaEngine/Core/Application.hpp"
#include "NebulaEngine/Core/Entrypoint.hpp"

#include "EditorLayer.hpp"
#include "NebulaEngine/Core/Pointer.hpp"

class EditorApplication : public Nebula::Application
{
public:
    explicit EditorApplication(const Nebula::ApplicationSpecification& spec)
        : Nebula::Application(spec)
    {
        // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
        PushOverlay(Nebula::OwnedPtr<EditorLayer*>::Create(spec.WorkingDirectory));
    }
};

Nebula::ScopedPtr<Nebula::Application> Nebula::CreateApplication(Nebula::CommandLineArguments args)
{
    ApplicationSpecification spec;
    spec.Name = "Nebula Editor";
    spec.WorkingDirectory = "./";
    spec.Arguments = std::move(args);
    return ScopedPtr<EditorApplication>::Create(spec);
}

