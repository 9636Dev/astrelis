#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "NebulaCore/Log.hpp"
#include "NebulaShaderCompiler/Lexer.hpp"
#include "NebulaShaderCompiler/Token.hpp"

#include "Arguments.hpp"

namespace CLI
{
    int main(std::vector<std::string> args);
} // namespace CLI

int main(int argc, char** argv)
{
    Nebula::Log::Init();
    auto args = CLI::ConstructArgumentList(argc, argv);
    return CLI::main(std::move(args));
}

namespace CLI
{
    enum class InputType
    {
        File,
        Stdin
    };

    struct Config
    {
        bool Verbose    = false;
        InputType Input = InputType::File;
        std::string OutputFile;
    };

    Config ToConfig(const std::map<std::string, std::string>& options)
    {
        Config config;
        if (options.contains("verbose"))
        {
            config.Verbose = true;
        }
        if (options.contains("output"))
        {
            config.OutputFile = options.at("output");
        }
        else if (options.contains("o"))
        {
            config.OutputFile = options.at("o");
        }
        if (options.contains("in"))
        {
            if (options.at("in") == "stdin")
            {
                config.Input = InputType::Stdin;
            }
            else if (options.at("in") == "file")
            {
                config.Input = InputType::File;
            }
            else
            {
                std::cerr << "Invalid input type: -in=" << options.at("in") << '\n';
                std::exit(1);
            }
        }

        return config;
    }

    int main(std::vector<std::string> args)
    {
        auto options = ParseOptions(RemoveOptions(args));
        if (options.contains("help"))
        {
            // We completely ignore the other arguments
            std::cout << "Usage: " << args[0]
                      << " <shader file>\n"
                         "Options:\n"
                         "  -help    -- prints this message\n"
                         "  -verbose -- outputs verbose debugging information\n";
            return 0;
        }
        auto config = ToConfig(options);
        std::string input;
        switch (config.Input)
        {
        case InputType::Stdin: {
            if (args.size() > 1)
            {
                std::cerr << "Warning: Ignoring input file\n";
                return 1;
            }

            std::string line;
            while (std::getline(std::cin, line))
            {
                input += line + '\n';
            }
            break;
        }
        case InputType::File: {
            if (args.size() == 1)
            {
                std::cerr << "Usage: " << args[0] << " <shader file>\n";
                return 1;
            }

            std::ifstream file(args[1]);
            if (!file.good())
            {
                std::cerr << "Failed to read file: " << args[1] << '\n';
                return 1;
            }

            // Read the file
            std::stringstream buffer;
            buffer << file.rdbuf();
            input = buffer.str();
            break;
        }
        }

        if (config.Verbose)
        {
            std::cout << "Start processing input (" << input.size() << " characters)\n";
        }

        Nebula::Shader::Lexer lexer(input);

        Nebula::Shader::Token token;
        do
        {
            token = lexer.NextToken();
            std::cout << token.ToString() << '\n';
        } while (token.Type != Nebula::Shader::TokenType::EndOfFile);

        return 0;
    }
} // namespace CLI

