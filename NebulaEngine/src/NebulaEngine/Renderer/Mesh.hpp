#pragma once

#include "NebulaEngine/Renderer/Vertex.hpp"
#include <cstdint>
#include <vector>
namespace Nebula
{
    struct StaticMesh
    {
        std::vector<Vertex> Vertices;
        std::vector<std::uint32_t> Indices;

        StaticMesh() = default;
    };
} // namespace Nebula

