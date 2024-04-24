#pragma once

#include "Geometry/Transform.hpp"
#include "Mesh/BaseMesh.hpp"

#include <memory>
#include <utility>

namespace Nebula
{
    class RenderableObject
    {
    public:
        friend class Renderer;
        friend class OpenGLRenderer;

        RenderableObject(const Transform& transform, std::shared_ptr<BaseMesh> mesh) :
            m_Transform(transform),
            m_Mesh(std::move(mesh))
        {
        }
    private:
        Transform m_Transform;
        std::shared_ptr<BaseMesh> m_Mesh;
        // We don't care about material right now
    };
} // namespace Nebula
