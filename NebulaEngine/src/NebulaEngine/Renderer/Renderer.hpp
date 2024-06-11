#pragma once

#include "NebulaEngine/Core/Bounds.hpp"
#include "NebulaEngine/Core/Pointer.hpp"
#include "NebulaEngine/Core/Result.hpp"
#include "NebulaEngine/Core/Window.hpp"

#include "Mesh.hpp"
#include "Transform.hpp"

#include <string>

namespace Nebula
{
    enum class RendererAPI
    {
        None   = 0,
        OpenGL = 1
    };

    class Renderer
    {
    public:
        Renderer()                           = default;
        virtual ~Renderer()                  = default;
        Renderer(const Renderer&)            = delete;
        Renderer& operator=(const Renderer&) = delete;
        Renderer(Renderer&&)                 = delete;
        Renderer& operator=(Renderer&&)      = delete;

        virtual void Viewport(const Bounds& bounds) = 0;
        virtual void Clear() = 0;
        virtual void DrawMesh(const StaticMesh& mesh, const Transform& transform) = 0;
        // This should clear the screen
        virtual void BeginFrame() = 0;
        virtual void EndFrame() = 0;

        static Result<Ptr<Renderer>, std::string> Create(Ref<Window> window, Bounds bounds);

    private:
        static RendererAPI s_RendererAPI;
    };
} // namespace Nebula
