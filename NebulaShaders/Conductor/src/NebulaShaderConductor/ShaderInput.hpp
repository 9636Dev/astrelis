#pragma once

#include <string>
#include <utility>

#include "TargetProfile.hpp"

namespace Nebula::ShaderConductor
{
    struct ShaderInput
    {
        std::string FileName;
        std::string Source;
        std::string EntryPoint;
        TargetProfile Profile = {};

        ShaderInput()                              = default;
        ~ShaderInput()                             = default;
        ShaderInput(const ShaderInput&)            = default;
        ShaderInput(ShaderInput&&)                 = default;
        ShaderInput& operator=(const ShaderInput&) = default;
        ShaderInput& operator=(ShaderInput&&)      = default;

        ShaderInput(std::string fileName, std::string source, std::string entryPoint, const TargetProfile& profile) :
            FileName(std::move(fileName)),
            Source(std::move(source)),
            EntryPoint(std::move(entryPoint)),
            Profile(profile)
        {
        }
    };
} // namespace Nebula::ShaderConductor
