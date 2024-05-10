#include "Input.hpp"

#include <iostream>
#include <sstream>
#include <vector>

namespace CLI
{
    inline static void PrintVerbose(Config& config)
    {
        if (config.Verbose)
        {
            std::cout << "Input stage: " << config.InputStage << '\n';
            std::cout << "Output file: " << config.OutputFile << '\n';
            std::cout << "Outputs:\n";
            for (const auto& [output, version] : config.OutputFormats)
            {
                std::cout << "\t" << static_cast<int>(output) << ": " << version << '\n';
            }
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
                if (config.OutputFormats.size() != 1)
                {
                    std::cerr << "Output file can only be one type of output for stage 2\n";
                    config.CorrectConfig = false;
                    return;
                }
                switch (config.OutputFormats[0].first)
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
        // Format: [output]<_[version]> (version is optional) (comma separated list)
        auto output = options.at("v");

        // Split by comma
        std::stringstream sstream(output);
        std::vector<std::string> outputVersions;
        std::string temp;
        while (std::getline(sstream, temp, ','))
        {
            outputVersions.emplace_back(temp);
        }

        if (outputVersions.empty())
        {
            std::cerr << "Invalid output version: -v=" << output << '\n';
            config.CorrectConfig = false;
            return;
        }

        for (std::string outputVersion : outputVersions)
        {
            std::size_t version = 0;

            {
                auto pos = outputVersion.find('_');
                if (pos != std::string::npos)
                {
                    try
                    {
                        version = std::stoul(outputVersion.substr(pos + 1));
                    }
                    catch (const std::exception&)
                    {
                        std::cerr << "Invalid version number: " << outputVersion.substr(pos + 1) << '\n';
                        config.CorrectConfig = false;
                        return;
                    }
                    outputVersion = outputVersion.substr(0, pos);
                }
            }

            if (outputVersion == "glsl")
            {
                config.OutputFormats.emplace_back(OutputType::GLSL, version);
                if (version == 0)
                {
                    version = 450;
                }
            }
            else if (outputVersion == "spirv")
            {
                config.OutputFormats.emplace_back(OutputType::SPIRV, version);
            }
            else if (outputVersion == "hlsl")
            {
                config.OutputFormats.emplace_back(OutputType::HLSL, version);
                if (version == 0)
                {
                    version = 11;
                }
            }
            else if (outputVersion == "msl")
            {
                config.OutputFormats.emplace_back(OutputType::MSL, version);
                if (version == 0)
                {
                    version = 10'200;
                }
            }
            else
            {
                std::cerr << "Invalid output type: -v=" << outputVersion << '\n';
                config.CorrectConfig = false;
                return;
            }
        }
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

        if (options.contains("v"))
        {
            ParseOutputVersion(config, options);
            options.erase("v");
            if (!config.CorrectConfig)
            {
                return config;
            }
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
