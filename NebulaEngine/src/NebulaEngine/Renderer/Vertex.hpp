#pragma once

#include <glm/glm.hpp>

namespace Nebula
{
    struct Vertex2D
    {
        glm::vec3 Position;
        glm::vec3 Color;
        glm::vec2 TexCoord;
    };
} // namespace Nebula
