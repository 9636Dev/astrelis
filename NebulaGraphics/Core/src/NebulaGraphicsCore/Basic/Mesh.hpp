#pragma once

#include "../Math.hpp"
#include "../Geometry/Transform.hpp"

#include <vector>

namespace Nebula::Basic
{
    using Transform = Nebula::Transform;

    struct Vertex
    {
        Vector3f Position;
        Vector2f TexCoord;
    };

    class Mesh
    {
    public:
        friend class Renderer;

        Mesh() = default;
        Mesh(const Mesh&) = default;
        Mesh& operator=(const Mesh&) = default;
        Mesh(Mesh&&) = default;
        Mesh& operator=(Mesh&&) = default;
        ~Mesh() = default;

        std::vector<Vertex>& GetVertices() { return m_Vertices; }
        std::vector<uint32_t>& GetIndices() { return m_Indices; }

    private:
        std::vector<Vertex> m_Vertices;
        std::vector<uint32_t> m_Indices;
    };
} // namespace Nebula::Basic
