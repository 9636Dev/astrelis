#pragma once

#include "NebulaShaderConductor/ShaderInput.hpp"
#include <string>

namespace Nebula
{
    struct ShaderProgram
    {
        std::string Name;
        ShaderConductor::ShaderInput VertexShader;
        ShaderConductor::ShaderInput FragmentShader;
    };
} // namespace Nebula
