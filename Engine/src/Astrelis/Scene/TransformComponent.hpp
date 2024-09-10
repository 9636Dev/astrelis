#pragma once

#include "Astrelis/Core/Math.hpp"

namespace Astrelis
{
    // TODO: Find some way to efficiently store this data, and translation, rotation, and scale
    struct TransformComponent
    {
        Mat4f Transform = Mat4f(1.0F);
    };
} // namespace Astrelis
