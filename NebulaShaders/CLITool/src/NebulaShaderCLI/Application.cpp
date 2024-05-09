#include <fstream>
#include <iostream>
#include <sstream>
#include <utility>
#include <vector>


#include "Arguments.hpp"
#include "Input.hpp"
#include "NebulaCore/Log.hpp"
#include "NebulaShaderBuilder/FileFormat.hpp"
#include "NebulaShaderBuilder/Glsl.hpp"

namespace CLI
{
    int ToIntermediate(Config& config, std::string input)
    {
        auto [output, err] = Nebula::Shader::IntermediateFormat::FromSource(std::move(input));
        if (!err.empty())
        {
            std::cerr << "Failed to parse file: " << err << std::endl;
            return 1;
        }

        if (config.Verbose)
        {
            std::cout << "Successfully parsed file\n";
        }

        auto outputFile = Nebula::File::FromPathString(config.OutputFile);

        switch (config.Output)
        {
        case OutputType::GLSL: {
            Nebula::ShaderConductor::GLSLOutput glslOutput;
            glslOutput.Version = config.OutputVersion;

            auto res = output.GenerateGlsl(glslOutput);
            if (!res.empty())
            {
                std::cerr << res << std::endl;
                return 1;
            }
            break;
        }
        default:
            std::cerr << "Output format " << static_cast<int>(config.Output) << "not supported yet\n";
            return 1;
        }

        auto res = output.WriteToFile(outputFile);

        if (!res.Success)
        {
            std::cerr << "Failed to write output file" << std::endl;
            return 1;
        }

        if (config.Verbose)
        {
            std::cout << "Written " << res.BytesWritten << " bytes to " << outputFile.GetPath() << std::endl;
        }

        return 0;
    }

    int GenerateCode(Config& config, std::string input)
    {
        auto glsl = Nebula::Shader::Glsl::FromSource(std::move(input));

        if (!glsl.has_value())
        {
            std::cerr << "Failed to parse GLSL from compiled nsl\n";
            return 1;
        }

        if (config.Verbose)
        {
            std::cout << "Successfully parsed GLSL\n";
        }

        std::cout << "Vertex:\n" << glsl->VertexSource << '\n';
        std::cout << "Pixel:\n" << glsl->PixelSource << '\n';

        return 0;
    }

    int main(std::vector<std::string> args)
    {
        // We remove all the options from the argument list
        auto options = ParseOptions(RemoveOptions(args));
        if (options.contains("help") || options.contains("h") || options.contains("?"))
        {
            // We completely ignore the other arguments
            std::cout << "Usage: " << args[0]
                      << " <shader file>\n"
                         "Options:\n"
                         "  -help    -- prints this message\n"
                         "  -verbose -- outputs verbose debugging information\n"
                         "  -version -- prints the version of the compiler\n";
            return 0;
        }
        if (options.contains("version"))
        {
            std::cout << "Nebula Shader Compiler v0.1\n";
            return 0;
        }

        auto config = ToConfig(options);
        if (!config.CorrectConfig)
        {
            return 1;
        }

        // We get the input source
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
            std::cout << "Compiling to stage " << config.InputStage << '\n';
        }

        return config.InputStage == 1 ? ToIntermediate(config, std::move(input))
                                      : GenerateCode(config, std::move(input));
    }
} // namespace CLI

int main(int argc, char** argv)
{
    Nebula::Log::Init();
    auto args = CLI::ConstructArgumentList(argc, argv);
    return CLI::main(std::move(args));
}
