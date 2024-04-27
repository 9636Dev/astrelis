#pragma once

#include "NebulaShaderCommon/ShaderStage.hpp"

#include <string>

namespace Nebula::ShaderConductor
{
    struct TargetProfile
    {
        Shader::ShaderStage Stage;
        std::size_t Major;
        std::size_t Minor;

        TargetProfile() = default;

        TargetProfile(Shader::ShaderStage stage, std::size_t major, std::size_t minor) :
            Stage(stage),
            Major(major),
            Minor(minor)
        {
        }

        [[nodiscard]] std::wstring ToString() const;
    };
} // namespace Nebula::ShaderConductor
