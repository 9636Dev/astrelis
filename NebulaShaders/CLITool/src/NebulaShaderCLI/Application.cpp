#include <fstream>
#include <iostream>
#include <sstream>
#include <utility>
#include <vector>

#include "NebulaCore/Log.hpp"
#include "NebulaShaderConductor/Conductor.hpp"

#include "Arguments.hpp"
#include "NebulaShaderConductor/ShaderOutput.hpp"
#include "NebulaShaderConductor/TargetProfile.hpp"

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

    enum class OutputType
    {
        GLSL,
        SPIRV,
        HLSL,
        MSL,
    };

    struct Config
    {
        bool CorrectConfig                             = true;
        bool Verbose                                   = false;
        InputType Input                                = InputType::File;
        Nebula::ShaderConductor::TargetProfile Profile = {Nebula::ShaderConductor::ShaderStage::Vertex, 6, 0};
        std::string EntryPoint                         = "main";
        std::string OutputFile;
        std::uint32_t OutputVersion                    = 450;
        OutputType Output                                            = OutputType::SPIRV;
        Nebula::ShaderConductor::OptimizationLevel OptimizationLevel = Nebula::ShaderConductor::OptimizationLevel::None;
    };

    Config ToConfig(const std::map<std::string, std::string>& options)
    {
        Config config;
        if (options.contains("verbose"))
        {
            config.Verbose = true;
        }
        if (options.contains("outputfile"))
        {
            config.OutputFile = options.at("outputfile");
        }
        if (options.contains("entrypoint"))
        {
            config.EntryPoint = options.at("entrypoint");
        }
        if (options.contains("output"))
        {
            if (options.at("output") == "glsl")
            {
                config.Output = OutputType::GLSL;
            }
            else if (options.at("output") == "spirv")
            {
                config.Output = OutputType::SPIRV;
            }
            else if (options.at("output") == "hlsl")
            {
                config.Output = OutputType::HLSL;
            }
            else if (options.at("output") == "msl")
            {
                config.Output = OutputType::MSL;
            }
            else
            {
                std::cerr << "Invalid output type: -output=" << options.at("output") << '\n';
                config.CorrectConfig = false;
                return config;
            }
        }
        if (options.contains("outputversion"))
        {
            config.OutputVersion = std::stoul(options.at("outputversion"));
        }
        if (options.contains("optimization"))
        {
            if (options.at("optimization") == "none")
            {
                config.OptimizationLevel = Nebula::ShaderConductor::OptimizationLevel::None;
            }
            else if (options.at("optimization") == "1")
            {
                config.OptimizationLevel = Nebula::ShaderConductor::OptimizationLevel::Level1;
            }
            else if (options.at("optimization") == "22")
            {
                config.OptimizationLevel = Nebula::ShaderConductor::OptimizationLevel::Level2;
            }
            else if (options.at("optimization") == "3")
            {
                config.OptimizationLevel = Nebula::ShaderConductor::OptimizationLevel::Level3;
            }
            else
            {
                std::cerr << "Invalid optimization level: -optimization=" << options.at("optimization") << '\n';
                config.CorrectConfig = false;
                return config;
            }
        }
        if (options.contains("profile"))
        {
            auto profile = options.at("profile");
            if (profile == "vertex")
            {
                config.Profile.Stage = Nebula::ShaderConductor::ShaderStage::Vertex;
            }
            else if (profile == "fragment")
            {
                config.Profile.Stage = Nebula::ShaderConductor::ShaderStage::Pixel;
            }
            else
            {
                std::cerr << "Invalid profile: -profile=" << profile << '\n';
                config.CorrectConfig = false;
                return config;
            }
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
                config.CorrectConfig = false;
                return config;
            }
        }

        return config;
    }

    int GenerateCode(Config& config, std::string input, std::vector<std::string> args)
    {
        Nebula::ShaderConductor::ShaderConductor conductor;
        if (!conductor.Initialize())
        {
            std::cerr << "Failed to initialize ShaderConductor\n";
            return 1;
        }
        Nebula::ShaderConductor::ShaderInput shaderInput;
        shaderInput.FileName   = config.Input == InputType::File ? args[1] : "stdin";
        shaderInput.Source     = std::move(input);
        shaderInput.EntryPoint = config.EntryPoint;
        shaderInput.Profile    = config.Profile;

        Nebula::ShaderConductor::ShaderOutput shaderOutput;
        shaderOutput.Optimization = config.OptimizationLevel;

        Nebula::ShaderConductor::SPIRVCompilationResult result = conductor.CompileToSPIRV(shaderInput, shaderOutput);

        if (!result.success)
        {
            std::cerr << "Failed to compile shader: " << result.errorMessage << '\n';
            return 1;
        }

        if (config.Verbose)
        {
            std::cout << "Compiled shader to SPIR-V (" << result.spirvCode.size() << " words)\n";
        }

        if (config.Output == OutputType::SPIRV)
        {
            if (config.OutputFile.empty())
            {
                std::cout << "No output file specified, not writing to disk (binary format)\n";
                return 0;
            }

            // We just cast it to a void* and write it to the file
            std::ofstream file(config.OutputFile, std::ios::binary);
            if (!file.good())
            {
                std::cerr << "Failed to open output file: " << config.OutputFile << '\n';
                return 1;
            }

            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
            file.write(reinterpret_cast<const char*>(result.spirvCode.data()),
                       static_cast<std::int64_t>(result.spirvCode.size() * sizeof(uint32_t)));
            return 0;
        }

        std::pair<std::string, std::string> output;
        switch (config.Output)
        {
        case OutputType::SPIRV:
            return 1;
        case OutputType::GLSL: {
            Nebula::ShaderConductor::GLSLOutput glslOutput;
            glslOutput.Optimization = shaderOutput.Optimization; // Doesn't get used, but good to set anyway
            glslOutput.Version      = config.OutputVersion;
            // TODO(9636D): Make this configurable
            output = Nebula::ShaderConductor::ShaderConductor::CompileToGLSL(result.spirvCode, glslOutput);


            break;
        }
        case OutputType::HLSL: {
            Nebula::ShaderConductor::HLSLOutput hlslOutput;
            hlslOutput.Optimization = shaderOutput.Optimization; // Doesn't get used, but good to set anyway
            output = Nebula::ShaderConductor::ShaderConductor::CompileToHLSL(result.spirvCode, hlslOutput);
            break;
        }

        case OutputType::MSL: {
            Nebula::ShaderConductor::MetalOutput mslOutput;
            mslOutput.Optimization = shaderOutput.Optimization; // Doesn't get used, but good to set anyway
            output = Nebula::ShaderConductor::ShaderConductor::CompileToMetal(result.spirvCode, mslOutput);
            break;
        }
        }

        if (!config.OutputFile.empty())
        {
            std::ofstream file(config.OutputFile);
            if (!file.good())
            {
                std::cerr << "Failed to open output file: " << config.OutputFile << '\n';
                return 1;
            }

            if (output.second.empty())
            {
                file << output.first;
            }
            else
            {
                std::cerr << "Errors:\n" << output.second << '\n';
            }

            file << output.first;
        }
        else
        {
            if (output.second.empty())
            {
                std::cout << output.first;
            }
            else
            {
                std::cerr << "Errors:\n" << output.second << '\n';
            }
        }

        return 0;
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
        if (!config.CorrectConfig)
        {
            return 1;
        }

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

        return GenerateCode(config, std::move(input), std::move(args));
    }
} // namespace CLI

