#pragma once

#include <string>

namespace Nebula
{
    // Just a temporary struct to hold shader data, in the future we will have a shader compiler
    struct ShaderProgram
    {
        std::string Name;
        std::string VertexShader;
        std::string FragmentShader;
    };
} // namespace Nebula
