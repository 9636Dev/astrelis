#pragma once

#include <cstddef>

namespace Nebula::ShaderConductor
{
    enum class OptimizationLevel
    {
        None,
        Level1,
        Level2,
        Level3,
    };

    struct ShaderOutput
    {
        OptimizationLevel Optimization = OptimizationLevel::None;
    };

    struct GLSLOutput : public ShaderOutput
    {
        // Version is in the format of 410 for 4.1
        std::size_t Version         = 410;
        bool Enable420PackExtension = false;
        bool GLES                   = false;
    };

    struct HLSLOutput : public ShaderOutput
    {
        // HLSL version is in the format of 50 for 5.0
        std::size_t HLSLVersion = 50;
    };

    struct MSLOutput : public ShaderOutput
    {
        // MSL version is in the format of 10200 for 1.2.0
        std::size_t MslVersion = 20000;
    };

} // namespace Nebula::ShaderConductor
