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
        std::size_t Version = 410;
        bool GLES = false;
    };

    struct HLSLOutput : public ShaderOutput
    {
        std::size_t HLSLVersion = 50;
    };

    struct MetalOutput : public ShaderOutput
    {
        std::size_t MslVersion = 2;
    };

} // namespace Nebula::ShaderConductor
