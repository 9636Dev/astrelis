#pragma once

#include "Mesh.hpp"
#include "NebulaGraphicsCore/Renderer.hpp"
#include "VisualProps.hpp"

#include <memory>

namespace Nebula::Basic
{
    // Basic renderer that supports low-level rendering, like drawing individual meshes or instanced meshes
    class Renderer
    {
    public:
        Renderer() = default;
        virtual ~Renderer() = default;
        Renderer(const Renderer&) = delete;
        Renderer& operator=(const Renderer&) = delete;
        Renderer(Renderer&&) = delete;
        Renderer& operator=(Renderer&&) = delete;

        virtual void AddMesh(std::shared_ptr<Mesh> mesh, const VisualProps& visualProps, const Transform& transform) = 0;
        virtual void AddMeshInstanced(std::shared_ptr<Mesh> mesh, const VisualProps& visualProps, const std::vector<Transform>& transforms) = 0;
        virtual void SetClearColor(float red, float green, float blue, float alpha) = 0;
        virtual void Render() = 0;
    };

    struct RendererCreationResult
    {
        using ErrorType = Nebula::RendererCreationResult::ErrorType;
        std::shared_ptr<Renderer> Renderer;

        ErrorType Error;
    };
} // namespace Nebula::Basic
