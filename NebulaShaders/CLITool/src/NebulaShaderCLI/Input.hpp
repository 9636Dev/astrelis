#pragma once

#include <cstdint>
#include <map>
#include <string>

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
        std::uint32_t OutputVersion    = 450;
        OutputType Output              = OutputType::SPIRV;
        Optimization OptimizationLevel = Optimization::None;
    };

    Config ToConfig(std::map<std::string, std::string>& options);
} // namespace CLI
