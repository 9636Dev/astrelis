#pragma once

#include <string>

namespace Nebula
{
    struct RenderPass
    {
        // The name of the render pass
        std::string Name;
        // The shader program to use for the render pass
        std::string ShaderProgram;
    };
} // namespace Nebula
