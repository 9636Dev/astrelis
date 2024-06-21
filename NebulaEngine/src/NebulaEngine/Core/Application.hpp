#pragma once

#include <string>
#include <vector>

#include "NebulaEngine/Core/LayerStack.hpp"
#include "NebulaEngine/UI/ImGui/ImGuiLayer.hpp"
#include "Pointer.hpp"
#include "Window.hpp"

#include "NebulaEngine/Events/WindowEvent.hpp"

int main(int argc, char** argv);

namespace Nebula
{
    struct CommandLineArguments
    {
        std::vector<std::string> Arguments;

        static CommandLineArguments Parse(int argc, char** argv)
        {
            CommandLineArguments result;
            for (int i = 1; i < argc; i++)
            {
                // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
                result.Arguments.emplace_back(argv[i]);
            }
            return result;
        }
    };

    struct ApplicationSpecification
    {
        std::string Name = "Nebula Application";
        std::string WorkingDirectory;
        CommandLineArguments Arguments;
    };

    class Application
    {
    public:
        friend int ::main(int argc, char** argv);
        explicit Application(ApplicationSpecification specification);
        virtual ~Application();
        Application(const Application&)            = delete;
        Application& operator=(const Application&) = delete;
        Application(Application&&)                 = delete;
        Application& operator=(Application&&)      = delete;

        RefPtr<Window>& GetWindow() { return m_Window; }

        const RefPtr<Window>& GetWindow() const { return m_Window; }

        static Application& Get() { return *s_Instance; }
    protected:
        void PushLayer(OwnedPtr<Layer*> layer);
        void PushOverlay(OwnedPtr<Layer*> overlay);
        [[nodiscard]] OwnedPtr<Layer*> PopLayer(RawRef<Layer*> layer);
        [[nodiscard]] OwnedPtr<Layer*> PopOverlay(RawRef<Layer*> overlay);

        void OnEvent(Event& event);
        bool OnWindowClose(WindowClosedEvent& event);
        bool OnViewportResize(ViewportResizedEvent& event);

        void Run();

        static Application* s_Instance;
        ApplicationSpecification m_Specification;
        bool m_Running = true;
        RefPtr<Window> m_Window;
        LayerStack m_LayerStack;
        RawRef<ImGuiLayer*> m_ImGuiLayer;
    };

    extern ScopedPtr<Application> CreateApplication(CommandLineArguments args);
} // namespace Nebula
