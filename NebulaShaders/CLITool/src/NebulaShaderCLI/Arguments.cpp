#include "Arguments.hpp"

namespace CLI
{
    std::vector<std::string> ConstructArgumentList(int argc, char** argv)
    {

        std::vector<std::string> args;
        args.reserve(argc);
        for (int i = 0; i < argc; ++i)
        {
            // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
            args.emplace_back(argv[i]);
        }
        return args;
    }

    std::vector<std::string> RemoveOptions(std::vector<std::string>& args)
    {
        std::vector<std::string> newArgs;
        std::vector<std::string> options;

        for (auto& arg : args)
        {
            if (arg.starts_with('-'))
            {
                options.emplace_back(arg);
            }
            else
            {
                newArgs.emplace_back(arg);
            }
        }
        args = std::move(newArgs);
        return options;
    }

    std::map<std::string, std::string> ParseOptions(const std::vector<std::string>& args)
    {
        std::map<std::string, std::string> options;
        for (auto& arg : args)
        {
            auto pos = arg.find('=');
            if (pos != std::string::npos)
            {
                options[arg.substr(1, pos - 1)] = arg.substr(pos + 1);
            }
            else
            {
                options[arg.substr(1)] = "";
            }
        }
        return options;
    }
} // namespace CLI
