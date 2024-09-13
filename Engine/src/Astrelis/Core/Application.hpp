#pragma once

#include "Astrelis/Events/WindowEvent.hpp"
#include "Astrelis/Renderer/RenderSystem.hpp"
#include "Astrelis/UI/ImGui/ImGuiLayer.hpp"

#include <atomic>
#include <string>
#include <vector>

#include "LayerStack.hpp"
#include "Pointer.hpp"
#include "Window.hpp"

int AstrelisMain(int argc, char** argv);

namespace Astrelis
{
    /// @brief Representation of command line arguments, in a more C++ way, as a std::vector of std::string
    struct CommandLineArguments
    {
        std::vector<std::string> Arguments;

        /// @brief Parses the command line arguments from the given argc and argv
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

    /// @brief Representation of an application version, with a release type, major, minor, and patch version
    struct ApplicationVersion
    {
        enum class ReleaseType : std::uint8_t
        {
            Alpha            = 0,
            Beta             = 1,
            ReleaseCandidate = 2,
            Release          = 3
        };

        ReleaseType Type;
        std::uint8_t Major;
        std::uint8_t Minor;
        std::uint8_t Patch;
    };

    /// @brief Specifications for a application, which will be used internally
    struct ApplicationSpecification
    {
        /**
        * @brief The name of the application, this is used primarily when debugging, and logging
        */
        std::string Name           = "Astrelis Application";
        ApplicationVersion Version = {ApplicationVersion::ReleaseType::Alpha, 0, 0, 1};
        /**
         * @brief The working directory of the application, use './' for the current directory.
        */
        std::string WorkingDirectory;
        CommandLineArguments Arguments;
    };

    enum class CreationStatus : std::uint16_t
    {
        SUCCESS                       = 0,
        WINDOW_CREATION_FAILED        = 1,
        RENDER_SYSTEM_CREATION_FAILED = 2
    };

    /// @brief The main application of Astrelis, including the core logic of the engine, and window lifetimes
    /// It stores basic state information:
    /// - m_IsRunning - Whether the application is running
    /// - m_Specification - The application specification
    /// - m_LayerStack - The layer stack of the application, @see LayerStack
    /// - m_ImGuiLayer - The ImGui layer, which is always on top of the layer stack, @see ImGuiLayer
    /// And also per window state information:
    /// - m_Window - The window of the application, see @see Window
    ///  @note You can create your own windows in your own layers, but the application is designed to have a main window, with a render system, @see RenderSystem
    /// - m_RenderSystem - A window specific render system, you can create your own renderers, that render into different frame buffers, etc..
    /// @note Initializing another Application after one is terminated is undefined behaviour. The application be the entire program, and should be a singleton.
    /// @note Resources should be correctly freed after termination, but reinitializing the application should work, but is not tested.
    class Application
    {
    public:
        friend int ::AstrelisMain(int argc, char** argv);
        friend void ApplicationSignalHandler(int signal);

        /// @brief Creates an application with the given specification
        /// @param specification The specification of the application
        /// @param status The status of the creation, if the creation was successful, or if it failed
        /// @note You should call this in the Astrelis::CreateApplication function, which should be defined in your application
        explicit Application(ApplicationSpecification specification, CreationStatus& status);
        virtual ~Application();
        Application(const Application&)            = delete;
        Application& operator=(const Application&) = delete;
        Application(Application&&)                 = delete;
        Application& operator=(Application&&)      = delete;

        RefPtr<Window>& GetWindow() { return m_Window; }

        const RefPtr<Window>& GetWindow() const { return m_Window; }

        RefPtr<RenderSystem>& GetRenderSystem() { return m_RenderSystem; }

        const RefPtr<RenderSystem>& GetRenderSystem() const { return m_RenderSystem; }

        const ApplicationSpecification& GetSpecification() const { return m_Specification; }

        /// Gets the application instance, which should be a singleton. The program will crash and throw an error if more than 1 application is created and not destroyed.
        static Application& Get() { return *s_Instance; }

        static bool HasInstance() { return s_Instance != nullptr; }
    protected:
        /// @brief Push a layer onto the stack, which will be updated and rendererd before all Overlays
        /// This method transfers the ownership to the layer stack, which means that deleting should not be done by the user. @see OwnedPtr
        void PushLayer(OwnedPtr<Layer*> layer);
        /// @brief Push an overlay onto the layer stack, which is after all of the layers
        /// This method transfers the ownership to the layer stack, which means that deleting should not be done by the user. @see OwnedPtr
        void PushOverlay(OwnedPtr<Layer*> overlay);
        /// @brief Pops a certain layer from the stack
        /// The owner is transfered back to the owner, which means that the user will need to handle the destruction. @see OwnedPtr
        [[nodiscard]] OwnedPtr<Layer*> PopLayer(RawRef<Layer*> layer);
        /// @brief Pops a certain overlay from the stack
        /// The owner is transfered back to the owner, which means that the user will need to handle the destruction. @see OwnedPtr
        [[nodiscard]] OwnedPtr<Layer*> PopOverlay(RawRef<Layer*> overlay);

        void OnEvent(Event& event);
        bool OnWindowClose(WindowCloseEvent& event);
        bool OnViewportResize(ViewportResizedEvent& event);

        /// This will start the application. This is called internally in 'AstrelisMain'.
        /// The gameloop outlines these steps:
        ///  - Check if the window is closed / application is closed.
        ///  - Begin the frame using the graphics context, this can initialize per farme components, acquire frames, or clear the screen.
        ///  - Update all of the layers
        ///  - Update all of the overlays (after all the layers)
        ///  - End the frame (submit render commands, etc..)
        ///  - Query user input
        void Run();

        static Application* s_Instance;
        ApplicationSpecification m_Specification;
        std::atomic_bool m_Running = true;
        RefPtr<Window> m_Window;
        RefPtr<RenderSystem> m_RenderSystem;
        LayerStack m_LayerStack;
        RawRef<ImGuiLayer*> m_ImGuiLayer;
    };

    /// @brief Define this function in your application to create an instance of your Application class, extending the base class
    extern ScopedPtr<Application> CreateApplication(CommandLineArguments args, CreationStatus& status);
} // namespace Astrelis

