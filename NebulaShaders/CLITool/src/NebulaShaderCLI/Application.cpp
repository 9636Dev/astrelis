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
#include "NebulaShaderBuilder/Msl.hpp"

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

        {
            auto iter = std::find_if(config.OutputFormats.begin(), config.OutputFormats.end(),
                                     [](const auto& pair) { return pair.first == OutputType::GLSL; });

            if (iter != config.OutputFormats.end())
            {
                Nebula::ShaderConductor::GLSLOutput glslOutput;
                glslOutput.Version = iter->second;

                auto res = output.GenerateGlsl(glslOutput);
                if (!res.empty())
                {
                    std::cerr << res << std::endl;
                    return 1;
                }
            }
        }

        {
            auto iter = std::find_if(config.OutputFormats.begin(), config.OutputFormats.end(),
                                     [](const auto& pair) { return pair.first == OutputType::MSL; });

            if (iter != config.OutputFormats.end())
            {
                Nebula::ShaderConductor::MSLOutput mslOutput;
                mslOutput.MslVersion = iter->second;

                auto res = output.GenerateMsl(mslOutput);
                if (!res.empty())
                {
                    std::cerr << res << std::endl;
                    return 1;
                }
            }
        }

        {
            auto iter = std::find_if(config.OutputFormats.begin(), config.OutputFormats.end(),
                                     [](const auto& pair) { return pair.first == OutputType::HLSL; });

            if (iter != config.OutputFormats.end())
            {
                std::cout << "HLSL output not supported yet\n";
            }
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

    int GenerateCode(Config& config, const std::string& input)
    {
        auto output = config.OutputFormats[0].first;

        switch (output)
        {
        case OutputType::GLSL: {
            auto glsl = Nebula::Shader::Glsl::FromSource(input);

            if (!glsl.has_value())
            {
                std::cerr << "Failed to parse GLSL from compiled nsl\n";
                return 1;
            }

            if (config.Verbose)
            {
                std::cout << "Successfully parsed GLSL\n";

                // We print the metadata as well
                std::cout << "Metadata:\n";
                for (const auto& ubo : glsl->UniformBuffers)
                {
                    std::cout << "Uniform Buffer: " << ubo.Name << '\n';
                    for (const auto& uniform : ubo.Bindings)
                    {
                        std::cout << "  " << uniform.Name << " (" << static_cast<std::uint32_t>(uniform.Type) << ")\n";
                    }
                }
            }

            std::cout << "Vertex:\n" << glsl->VertexSource << '\n';
            std::cout << "Pixel:\n" << glsl->PixelSource << '\n';

            return 0;
        }
        case OutputType::SPIRV: {
            std::cout << "SPIRV output not supported yet\n";
            return 1;
        }
        case OutputType::HLSL: {
            std::cout << "HLSL output not supported yet\n";
            return 1;
        }
        case OutputType::MSL: {
            auto msl = Nebula::Shader::Msl::FromSource(input);

            if (!msl.has_value())
            {
                std::cerr << "Failed to parse MSL from compiled nsl\n";
                return 1;
            }

            if (config.Verbose)
            {
                std::cout << "Successfully parsed MSL\n";
            }

            std::cout << "Vertex:\n" << msl->VertexSource << '\n';
            std::cout << "Fragment:\n" << msl->PixelSource << '\n';
            return 1;
        }
        }


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

        return config.InputStage == 1 ? ToIntermediate(config, std::move(input)) : GenerateCode(config, input);
    }
} // namespace CLI

int main(int argc, char** argv)
{
    Nebula::Log::Init();
    auto args = CLI::ConstructArgumentList(argc, argv);
    return CLI::main(std::move(args));
}
