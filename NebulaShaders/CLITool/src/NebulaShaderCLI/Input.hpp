#pragma once

#include <cstdint>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include "NebulaShaderConductor/ShaderOutput.hpp"

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
        using Optimization             = Nebula::ShaderConductor::OptimizationLevel;
        std::string OutputFile         = "output";
        bool CorrectConfig             = true;
        bool Verbose                   = false;
        InputType Input                = InputType::File;
        std::size_t InputStage         = 1;
        Optimization OptimizationLevel = Optimization::None;
        std::vector<std::pair<OutputType, std::uint32_t>> OutputFormats;
    };

    Config ToConfig(std::map<std::string, std::string>& options);
} // namespace CLI
