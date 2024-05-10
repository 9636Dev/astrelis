#pragma once

#include "NebulaGraphicsCore/AssetLoader.hpp"
#include "NebulaGraphicsCore/RenderPass.hpp"
#include "NebulaGraphicsCore/RenderableObject.hpp"
#include <limits>
#include <memory>
#include <vector>

namespace Nebula
{
    class Renderer
    {
    public:
        Renderer()                           = default;
        Renderer(const Renderer&)            = delete;
        Renderer& operator=(const Renderer&) = delete;
        Renderer(Renderer&&)                 = delete;
        Renderer& operator=(Renderer&&)      = delete;

        virtual ~Renderer() = default;

        virtual void Render() = 0;


        /**
        * @brief Adds a renderable object to the renderer
        * @param renderableObject The renderable object to add
        * @param renderPassIndex The index of the render pass to add the renderable object to, defaults to the end of the render passes
        */
        void AddRenderPass(RenderPass renderPass, std::size_t insertionIndex = std::numeric_limits<std::size_t>::max());

        /**
        * @brief Removes a render pass from the renderer
        * @param index The index of the render pass to remove
        * @return The render pass that was removed
        */
        RenderPass RemoveRenderPass(std::size_t index);

        /**
        * @brief Adds a renderable object to the renderer
        * @param renderableObject The renderable object to add
        * @param renderPassIndex The index of the render pass to add the renderable object to, defaults to the first render pass
        * @note If the render pass index is out of bounds, the renderable object will be added to the first render pass
        */
        void AddRenderableObject(RenderableObject renderableObject, std::size_t renderPassIndex);

        [[nodiscard]] std::size_t GetIndexOfRenderPass(const std::string& name) const;

        virtual void OnResize(std::uint32_t width, std::uint32_t height)            = 0;
        virtual void SetClearColor(float red, float green, float blue, float alpha) = 0;
        virtual AssetLoader& GetAssetLoader()                                       = 0;

        RenderableObject& GetRenderableObject(std::size_t index) { return m_RenderableObjects[index]; }
    protected:
        virtual void InternalAddRenderPass(RenderPass renderPass, std::size_t insertionIndex)                    = 0;
        virtual void InternalRemoveRenderPass(std::size_t index)                                                 = 0;
        virtual void InternalAddRenderableObject(RenderableObject renderableObject, std::size_t renderPassIndex) = 0;
        virtual void InternalRemoveRenderableObject(std::size_t index)                                           = 0;

        [[nodiscard]] std::size_t GetStartOffsetForRenderPass(std::size_t renderPassIndex) const;


        // We use a vector, because:
        // - We don't know how many render passes there will be
        // - We need to index into the vector to get the render pass
        // - Changing the size of the vector is not a common operation
        std::vector<RenderPass> m_RenderPasses; // NOLINT(cppcoreguidelines-non-private-member-variables-in-classes)

        // We use a vector, because:
        // - We don't know how many renderable objects there will be
        // - We need to iterate over the renderable objects quickly
        // - Changing the size is a common operation, but not as common as iterating over the renderable objects
        // - We don't use a 2D vector, because we store the offsets for the render passes in a different vector
        std::vector<RenderableObject>
            m_RenderableObjects; // NOLINT(cppcoreguidelines-non-private-member-variables-in-classes)
        // Offsets into the RenderableObjects vector, where each index corresponds to number of elements in each RenderPass
        // This is only access when switching render passes.
        // The render passes should be accessed in order, so each switch is just an addition of the offset to the current index
        // We do this to avoid changing offsets every time we add a new renderable object, which is O(n) operation, instead of O(1)
        std::vector<size_t>
            m_RenderPassObjectCount; // NOLINT(cppcoreguidelines-non-private-member-variables-in-classes)
    };

    struct RendererCreationResult
    {
        enum class ErrorType
        {
            None,
            LibraryLoadFailed,
            SymbolLoadFailed,
            InvalidArguments,
            ContextNotSupported,
            RendererInitFailed,
        };

        std::shared_ptr<Renderer> Renderer;

        ErrorType Error;
    };
} // namespace Nebula
