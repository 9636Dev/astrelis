#pragma once

#include <string>
#include <vector>

#include "Pointer.hpp"

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
        explicit Application(ApplicationSpecification specification);
        virtual ~Application();
        Application(const Application&)            = delete;
        Application& operator=(const Application&) = delete;
        Application(Application&&)                 = delete;
        Application& operator=(Application&&)      = delete;

        static Application& Get() { return *s_Instance; }
    private:
        void Run();
        friend int ::main(int argc, char** argv);

        ApplicationSpecification m_Specification;
        static Application* s_Instance;
    };

    extern Ptr<Application> CreateApplication(CommandLineArguments args);
} // namespace Nebula
