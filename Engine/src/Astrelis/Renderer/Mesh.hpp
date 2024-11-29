#pragma once

#include "Astrelis/Core/Math.hpp"

namespace Astrelis {
    struct Vertex2D {
        // TODO(Feature): Support transparency
        Vec3f Position;
        Vec2f TexCoord;
    };

    struct Mesh2D {
        using IndicesType = std::uint32_t;
        std::vector<Vertex2D>    Vertices;
        std::vector<IndicesType> Indices;
    };
} // namespace Astrelis
