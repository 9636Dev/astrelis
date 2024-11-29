#include "Astrelis/Core/Application.hpp"

#include "Astrelis/Core/Entrypoint.hpp"
#include "Astrelis/Core/Pointer.hpp"

#include "EditorLayer.hpp"

namespace AstrelisEditor {
    class EditorApplication : public Astrelis::Application {
    public:
        explicit EditorApplication(
            const Astrelis::ApplicationSpecification& spec, Astrelis::CreationStatus& status)
            : Astrelis::Application(spec, status) {
            // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
            PushOverlay(static_cast<Astrelis::OwnedPtr<Astrelis::Layer*>>(
                Astrelis::OwnedPtr<EditorLayer*>::Create("resources")));
            //m_Window->SetVSync(false);
        }
    };
} // namespace AstrelisEditor

Astrelis::ScopedPtr<Astrelis::Application> Astrelis::CreateApplication(
    Astrelis::CommandLineArguments args, Astrelis::CreationStatus& status) {
    ApplicationSpecification spec;
    spec.Name             = "Astrelis Editor";
    spec.WorkingDirectory = "./run";
    spec.Arguments        = std::move(args);
    return static_cast<Astrelis::ScopedPtr<Astrelis::Application>>(
        ScopedPtr<AstrelisEditor::EditorApplication>::Create(spec, status));
}

ASTRELIS_ENTRYPOINT
