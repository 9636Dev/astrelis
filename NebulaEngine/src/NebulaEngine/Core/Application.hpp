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

        static Application& Get() { return *s_Instance; }

        Window& GetWindow() { return *m_Window.Get(); }
    protected:
        void PushLayer(Layer* layer);
        void PushOverlay(Layer* overlay);
        void PopLayer(Layer* layer);
        void PopOverlay(Layer* overlay);

        void OnEvent(Event& event);
        bool OnWindowClose(WindowClosedEvent& event);

        void Run();

        static Application* s_Instance;
        ApplicationSpecification m_Specification;
        bool m_Running = true;
        Ptr<Window> m_Window;
        LayerStack m_LayerStack;
        ImGuiLayer* m_ImGuiLayer;
    };

    extern Ptr<Application> CreateApplication(CommandLineArguments args);
} // namespace Nebula
