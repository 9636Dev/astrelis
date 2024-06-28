#include "NebulaEngine/Core/Application.hpp"
#include "NebulaEngine/Core/Entrypoint.hpp"

#include "EditorLayer.hpp"
#include "NebulaEngine/Core/Pointer.hpp"

namespace Pulsar
{
    class EditorApplication : public Nebula::Application
    {
    public:
        explicit EditorApplication(const Nebula::ApplicationSpecification& spec) : Nebula::Application(spec)
        {
            // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
            PushOverlay(static_cast<Nebula::OwnedPtr<Nebula::Layer*>>(
                Nebula::OwnedPtr<EditorLayer*>::Create("resources")));
        }
    };
} // namespace Pulsar

Nebula::ScopedPtr<Nebula::Application> Nebula::CreateApplication(Nebula::CommandLineArguments args)
{
    ApplicationSpecification spec;
    spec.Name             = "Nebula Editor";
    spec.WorkingDirectory = "./run";
    spec.Arguments        = std::move(args);
    return static_cast<Nebula::ScopedPtr<Nebula::Application>>(ScopedPtr<Pulsar::EditorApplication>::Create(spec));
}

