#pragma once

#include <array>
namespace Nebula
{
    struct Vertex
    {
        std::array<float, 3> Position;
        std::array<float, 4> Color;
    };
} // namespace Nebula
