#include "NebulaEngine/Core/Application.hpp"
#include "NebulaEngine/Core/Entrypoint.hpp"

#include "EditorLayer.hpp"

class EditorApplication : public Nebula::Application
{
public:
    explicit EditorApplication(const Nebula::ApplicationSpecification& spec)
        : Nebula::Application(spec)
    {
        // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
        PushOverlay(new EditorLayer(spec.WorkingDirectory));
    }
};

Nebula::Ptr<Nebula::Application> Nebula::CreateApplication(Nebula::CommandLineArguments args)
{
    ApplicationSpecification spec;
    spec.Name = "Nebula Editor";
    spec.WorkingDirectory = "./";
    spec.Arguments = std::move(args);
    return MakePtr<EditorApplication>(spec).Cast<Nebula::Application>();
}

