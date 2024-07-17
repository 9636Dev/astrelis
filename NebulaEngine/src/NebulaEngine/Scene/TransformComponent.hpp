#pragma once

#include "NebulaEngine/Core/Math.hpp"

namespace Nebula
{
    // TODO: Find some way to efficiently store this data, and translation, rotation, and scale
    struct TransformComponent
    {
        mat4 Transform = mat4(1.0F);
    };
} // namespace Nebula
