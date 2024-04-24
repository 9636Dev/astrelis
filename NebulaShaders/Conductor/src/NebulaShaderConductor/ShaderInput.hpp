#pragma once

#include <string>

#include "TargetProfile.hpp"

namespace Nebula::ShaderConductor
{
    struct ShaderInput
    {
        std::string FileName;
        std::string Source;
        std::string EntryPoint;
        TargetProfile Profile;
    };
} // namespace Nebula::ShaderConductor
