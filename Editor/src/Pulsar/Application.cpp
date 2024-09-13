#include "Astrelis/Core/Application.hpp"
#include "Astrelis/Core/Entrypoint.hpp"

#include "Astrelis/Core/Pointer.hpp"
#include "EditorLayer.hpp"

namespace Pulsar
{
    class EditorApplication : public Astrelis::Application
    {
    public:
        explicit EditorApplication(const Astrelis::ApplicationSpecification& spec, Astrelis::CreationStatus& status) :
            Astrelis::Application(spec, status)
        {
            // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
            PushOverlay(static_cast<Astrelis::OwnedPtr<Astrelis::Layer*>>(
                Astrelis::OwnedPtr<EditorLayer*>::Create("resources")));
            m_RenderSystem->SetBlitSwapchain(false);
        }
    };
} // namespace Pulsar

Astrelis::ScopedPtr<Astrelis::Application> Astrelis::CreateApplication(Astrelis::CommandLineArguments args,
                                                                       Astrelis::CreationStatus& status)
{
    ApplicationSpecification spec;
    spec.Name             = "Astrelis Editor";
    spec.WorkingDirectory = "./run";
    spec.Arguments        = std::move(args);
    return static_cast<Astrelis::ScopedPtr<Astrelis::Application>>(
        ScopedPtr<Pulsar::EditorApplication>::Create(spec, status));
}

