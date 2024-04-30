#include <fstream>
#include <iostream>
#include <sstream>
#include <utility>
#include <vector>

#include "NebulaCore/Log.hpp"
#include "NebulaShaderCompiler/ShaderCompiler.hpp"
#include "NebulaShaderConductor/Conductor.hpp"

#include "Arguments.hpp"
#include "NebulaShaderConductor/ShaderInput.hpp"
#include "NebulaShaderConductor/ShaderOutput.hpp"
#include "NebulaShaderConductor/TargetProfile.hpp"

namespace CLI
{
    int main(std::vector<std::string> args);
} // namespace CLI

// TODO(9636Dev): Refactor, and add both stages of compiler building to same CLI

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
        bool CorrectConfig = true;
        bool Verbose       = false;
        InputType Input    = InputType::File;
        std::string OutputFile;
        std::uint32_t OutputVersion                                  = 450;
        OutputType Output                                            = OutputType::SPIRV;
        Nebula::ShaderConductor::OptimizationLevel OptimizationLevel = Nebula::ShaderConductor::OptimizationLevel::None;
    };

    // NOLINTNEXTLINE(readability-function-cognitive-complexity)
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

    int GenerateCode(Config& config, std::string input)
    {
        Nebula::Shader::Compiler compiler(std::move(input));
        auto result = compiler.Compile();
        if (result.has_value())
        {
            auto [row, col] = compiler.GetRowColumn(result->Index);
            std::cerr << "Failed to compile shader at " << row << ":" << col << ": " << result->Message << '\n';
            return 1;
        }

        if (config.Verbose)
        {
            std::cout << "Successfully compiled shader\n";
        }

        auto bindings      = compiler.GetBindings();
        auto textures      = compiler.GetTextures();
        auto inputs        = compiler.GetInputs();
        auto fragmentInput = compiler.GetPixelInputs();

        auto vertexEntry   = compiler.GetVertexEntrypoint();
        auto fragmentEntry = compiler.GetPixelEntrypoint();

        auto sources = compiler.GetSources();

        if (!sources.Vertex.has_value() || !sources.Pixel.has_value())
        {
            std::cerr << "Generated shaders are not supported yet\n";
            return 1;
        }

        auto vertexSource   = sources.Shared.value_or("") + sources.Vertex.value();
        auto fragmentSource = sources.Shared.value_or("") + sources.Pixel.value();

        if (config.Verbose)
        {
            std::cout << "Bindings:\n";
            for (const auto& binding : bindings)
            {
                std::cout << "  " << binding.Type << " " << binding.Name << " -> " << binding.Target << '\n';
            }
            std::cout << "Textures:\n";
            for (const auto& texture : textures)
            {
                std::cout << "  " << texture.Type << " " << texture.Name << " -> " << texture.Target << '\n';
            }
            std::cout << "Inputs:\n";
            for (const auto& input : inputs)
            {
                std::cout << "  " << input.Type << " " << input.Name << " -> " << input.Semantic << '\n';
            }
            std::cout << "Pixel Inputs:\n";
            for (const auto& input : fragmentInput)
            {
                std::cout << "  " << input.Type << " " << input.Name << " -> " << input.Semantic << '\n';
            }

            std::cout << "Vertex Entrypoint: " << vertexEntry << '\n';
            std::cout << "Pixel Entrypoint: " << fragmentEntry << '\n';

            std::cout << "Vertex Shader:\n" << vertexSource << '\n';
            std::cout << "Pixel Shader:\n" << fragmentSource << '\n';
        }

        Nebula::ShaderConductor::ShaderConductor conductor;
        Nebula::ShaderConductor::TargetProfile vertexProfile(Nebula::Shader::ShaderStage::Vertex, 6, 0);
        Nebula::ShaderConductor::TargetProfile fragmentProfile(Nebula::Shader::ShaderStage::Pixel, 6, 0);

        Nebula::ShaderConductor::ShaderInput vertexInput;
        vertexInput.Source = vertexSource;
        vertexInput.EntryPoint = vertexEntry;
        vertexInput.Profile = vertexProfile;
        vertexInput.FileName = compiler.GetMeta().Name + "_VS";


        Nebula::ShaderConductor::ShaderInput pixelInput;
        pixelInput.Source = fragmentSource;
        pixelInput.EntryPoint = fragmentEntry;
        pixelInput.Profile = fragmentProfile;
        pixelInput.FileName = compiler.GetMeta().Name + "_PS";

        Nebula::ShaderConductor::ShaderOutput output;
        output.Optimization = config.OptimizationLevel;


        auto spirvVertex = conductor.CompileToSPIRV(vertexInput, output);
        auto spirvPixel = conductor.CompileToSPIRV(pixelInput, output);

        if (!spirvVertex.success)
        {
            std::cerr << "Failed to compile vertex shader: " << spirvVertex.errorMessage << '\n';
            return 1;
        }

        if (!spirvPixel.success)
        {
            std::cerr << "Failed to compile fragment shader: " << spirvPixel.errorMessage << '\n';
            return 1;
        }

        if (config.Output == OutputType::GLSL)
        {
            Nebula::ShaderConductor::GLSLOutput glslOutput;
            glslOutput.Version = config.OutputVersion;
            glslOutput.Optimization = config.OptimizationLevel;

            auto [glslVertex, vertexMeta] = Nebula::ShaderConductor::ShaderConductor::CompileToGLSL(spirvVertex.spirvCode, glslOutput);
            auto [glslPixel, fragmentMeta] = Nebula::ShaderConductor::ShaderConductor::CompileToGLSL(spirvPixel.spirvCode, glslOutput);

            if (!glslVertex.second.empty())
            {
                std::cerr << "Failed to compile vertex shader to GLSL: " << glslVertex.second << '\n';
                return 1;
            }

            if (!glslPixel.second.empty())
            {
                std::cerr << "Failed to compile fragment shader to GLSL: " << glslPixel.second << '\n';
                return 1;
            }

            std::cout << "\n\nVertex Shader:\n" << glslVertex.first << '\n';
            std::cout << "\n\nFragment Shader:\n" << glslPixel.first << '\n';

        }
        else if (config.Output == OutputType::MSL)
        {
            Nebula::ShaderConductor::MslOutput mslOutput;
            mslOutput.Optimization = config.OptimizationLevel;

            auto [mslVertex, vertexMeta] = Nebula::ShaderConductor::ShaderConductor::CompileToMsl(spirvVertex.spirvCode, mslOutput);
            auto [mslPixel, fragmentMeta] = Nebula::ShaderConductor::ShaderConductor::CompileToMsl(spirvPixel.spirvCode, mslOutput);

            if (!mslVertex.second.empty())
            {
                std::cerr << "Failed to compile vertex shader to MSL: " << mslVertex.second << '\n';
                return 1;
            }

            if (!mslPixel.second.empty())
            {
                std::cerr << "Failed to compile fragment shader to MSL: " << mslPixel.second << '\n';
                return 1;
            }

            std::cout << "\n\nVertex Shader:\n" << mslVertex.first << '\n';
            std::cout << "\n\nFragment Shader:\n" << mslPixel.first << '\n';
        }

        return 0;
    }

    int main(std::vector<std::string> args)
    {
        auto options = ParseOptions(RemoveOptions(args));
        if (options.contains("help") || options.contains("h") || options.contains("?"))
        {
            // We completely ignore the other arguments
            std::cout << "Usage: " << args[0]
                      << " <shader file>\n"
                         "Options:\n"
                         "  -help    -- prints this message\n"
                         "  -verbose -- outputs verbose debugging information\n"
                         "  -output=<type> -- specifies the output type (glsl, spirv, hlsl, msl)\n"
                         "  -outputversion=<version> -- specifies the output version (default: 450)\n"
                         "  -optimization=<level> -- specifies the optimization level (none, 1, 22, 3)\n";
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

        return GenerateCode(config, std::move(input));
    }
} // namespace CLI

