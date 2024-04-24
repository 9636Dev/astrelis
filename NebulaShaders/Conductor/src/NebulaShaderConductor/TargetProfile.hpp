#pragma once

#include <string>

namespace Nebula::ShaderConductor
{
    enum class ShaderStage
    {
        Vertex,
        Pixel,
    };

    struct TargetProfile
    {
        ShaderStage Stage;
        std::size_t Major;
        std::size_t Minor;

        [[nodiscard]] std::wstring ToString() const;
    };
} // namespace Nebula::ShaderConductor
