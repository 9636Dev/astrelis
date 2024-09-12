#pragma once

#include "Astrelis/Core/Math.hpp"

namespace Astrelis
{
    struct Vertex2D
    {
        Vec3f Position;
        Vec3f Color;
        Vec2f TexCoord;

        constexpr Vertex2D(Vec3f position, Vec3f color, Vec2f texCoord) noexcept :
            Position(position),
            Color(color),
            TexCoord(texCoord)
        {
        }
    };
} // namespace Astrelis
