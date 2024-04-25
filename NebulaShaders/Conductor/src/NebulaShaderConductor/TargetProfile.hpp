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

        TargetProfile() = default;
        TargetProfile(ShaderStage stage, std::size_t major, std::size_t minor)
            : Stage(stage), Major(major), Minor(minor)
        {
        }

        [[nodiscard]] std::wstring ToString() const;
    };
} // namespace Nebula::ShaderConductor
