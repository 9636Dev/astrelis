#pragma once

#include "NebulaEngine/Core/Bounds.hpp"
#include "NebulaEngine/Core/Pointer.hpp"
#include "NebulaEngine/Core/Result.hpp"
#include "NebulaEngine/Core/Window.hpp"

#include "Mesh.hpp"
#include "Transform.hpp"
#include "Material.hpp"

#include <string>
#include <vector>

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
        /**
         * @brief Clear the screen
         * @note Do not need to call Clear() if BeginFrame() is called
         * @see BeginFrame()
         */
        virtual void Clear() = 0;

        /**
         * @brief Adds a mesh to the batch renderer
         * @param mesh The mesh to render
         * @param transform The transform of the mesh
         * @note EndFrame needs to be called to flush the batch renderer
         */
        virtual void DrawMesh(const StaticMesh& mesh, const Transform& transform, const Material& material) = 0;

        virtual void InstanceMesh(const StaticMesh& mesh, const std::vector<Transform>& transforms, const std::vector<Material>& materials) = 0;
        /**
         * @brief Begin the frame
         * @note This function should be called before any rendering is done, as it will clear the screen
         * @see Clear()
         */
        virtual void BeginFrame() = 0;

        /**
         * @brief End the frame
         * @note This function should be called after all rendering is done, as it flushes the batch renderer
         */
        virtual void EndFrame() = 0;

        /**
        * @brief Create a new Renderer
        * @param window The window to render to
        * @param bounds The bounds of the window
        * @return A Result containing either a Renderer or an error message
        * @note The RendererAPI is currently hardcoded to OpenGL
        */
        static Result<Ptr<Renderer>, std::string> Create(Ref<Window> window, Bounds bounds);

        static RendererAPI GetAPI() { return s_RendererAPI; }
    private:
        static RendererAPI s_RendererAPI;
    };
} // namespace Nebula
