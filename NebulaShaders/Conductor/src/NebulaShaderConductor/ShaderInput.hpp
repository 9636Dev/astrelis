#pragma once

#include <string>

#include "TargetProfile.hpp"

namespace Nebula::ShaderConductor
{
    struct ShaderInput
    {
        std::wstring FileName;
        std::wstring Source;
        std::wstring EntryPoint;
        TargetProfile Profile;
    };
} // namespace Nebula::ShaderConductor
