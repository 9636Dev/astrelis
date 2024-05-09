#include "Input.hpp"

#include <iostream>

namespace CLI
{
    inline static void PrintVerbose(Config& config)
    {
        if (config.Verbose)
        {
            std::cout << "Input stage: " << config.InputStage << '\n';
            std::cout << "Output file: " << config.OutputFile << '\n';
            std::cout << "Output type: " << static_cast<int>(config.Output) << '\n';
            std::cout << "Output version: " << config.OutputVersion << '\n';
        }
    }

    inline static void ParseOutputFile(Config& config, std::map<std::string, std::string>& options)
    {
        config.OutputFile = options.at("o");

        // If there is no extension, we add it
        if (config.OutputFile.find('.') == std::string::npos)
        {
            if (config.InputStage == 1)
            {
                config.OutputFile += ".cnsl";
            }
            else
            {
                switch (config.Output)
                {
                case OutputType::GLSL:
                    config.OutputFile += ".glsl";
                    break;
                case OutputType::SPIRV:
                    config.OutputFile += ".spv";
                    break;
                case OutputType::HLSL:
                    config.OutputFile += ".hlsl";
                    break;
                case OutputType::MSL:
                    config.OutputFile += ".metal";
                    break;
                }
            }
        }
    }

    inline static void ParseOutputVersion(Config& config, std::map<std::string, std::string>& options)
    {
        // Format: [output]<_[version]> (version is optional)
        auto output         = options.at("v");
        std::size_t version = 0;

        {
            auto pos = output.find('_');
            if (pos != std::string::npos)
            {
                try
                {
                    version = std::stoul(output.substr(pos + 1));
                }
                catch (const std::exception&)
                {
                    std::cerr << "Invalid version number: " << output.substr(pos + 1) << '\n';
                    config.CorrectConfig = false;
                    return;
                }
                output = output.substr(0, pos);
            }
        }

        if (output == "glsl")
        {
            config.Output = OutputType::GLSL;
            if (version == 0)
            {
                version = 450;
            }
        }
        else if (output == "spirv")
        {
            config.Output = OutputType::SPIRV;
        }
        else if (output == "hlsl")
        {
            config.Output = OutputType::HLSL;
            if (version == 0)
            {
                version = 11;
            }
        }
        else if (output == "msl")
        {
            config.Output = OutputType::MSL;
            if (version == 0)
            {
                version = 10'200;
            }
        }
        else
        {
            std::cerr << "Invalid output type: -output=" << output << '\n';
            config.CorrectConfig = false;
            return;
        }

        config.OutputVersion = version;
    }

    Config ToConfig(std::map<std::string, std::string>& options)
    {
        Config config;
        if (options.contains("verbose"))
        {
            config.Verbose = true;
            options.erase("verbose");
        }

        if (options.contains("s"))
        {
            try
            {
                auto stage = std::stoul(options.at("s"));
                if (stage < 1 || stage > 2)
                {
                    std::cerr << "Invalid input stage: " << stage << '\n';
                    config.CorrectConfig = false;
                    return config;
                }
                config.InputStage = stage;
            }
            catch (const std::exception&)
            {
                std::cerr << "Invalid input stage: " << options.at("s") << '\n';
                config.CorrectConfig = false;
                return config;
            }
            options.erase("s");
        }
        else
        {
            std::cerr << "Input stage not specified\n";
            config.CorrectConfig = false;
            return config;
        }

        if (options.contains("o"))
        {
            ParseOutputFile(config, options);
            options.erase("o");
            if (!config.CorrectConfig)
            {
                return config;
            }
        }
        else
        {
            std::cerr << "Output file not specified\n";
            config.CorrectConfig = false;
            return config;
        }
        if (options.contains("v"))
        {
            if (config.InputStage == 2)
            {
                std::cout << "Warning: Output version not supported for stage 2\n";
            }

            ParseOutputVersion(config, options);
            options.erase("v");
            if (!config.CorrectConfig)
            {
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
            options.erase("in");
        }

        if (!options.empty())
        {
            std::cout << "Warning: Unused options:\n";
            for (const auto& [key, value] : options)
            {
                std::cout << "\t" << key << ": " << value << '\n';
            }
        }

        PrintVerbose(config);
        return config;
    }
} // namespace CLI
