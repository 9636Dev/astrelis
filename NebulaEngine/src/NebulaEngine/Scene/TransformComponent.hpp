#pragma once

#include <glm/glm.hpp>

namespace Nebula
{
    // TODO: Find some way to efficiently store this data, and translation, rotation, and scale
    struct TransformComponent
    {
        glm::mat4 Transform = glm::mat4(1.0F);
    };
} // namespace Nebula
