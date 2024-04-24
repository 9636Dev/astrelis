#pragma once

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
        OptimizationLevel Optimization;
    };
} // namespace Nebula::ShaderConductor
