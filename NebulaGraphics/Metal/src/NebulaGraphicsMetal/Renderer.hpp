#pragma once

#include "NebulaGraphicsCore/Renderer.hpp"
#include "Window.hpp"

#include "AssetLoader.hpp"

#include <memory>

namespace Nebula
{
    struct MetalMesh
    {
        id<MTLBuffer> VertexBuffer;
        id<MTLBuffer> IndexBuffer;
    };

    class MetalRenderer : public Renderer
    {
    public:
        explicit MetalRenderer(std::shared_ptr<MetalWindow> window);
        ~MetalRenderer() override;
        MetalRenderer(const MetalRenderer&) = delete;
        MetalRenderer& operator=(const MetalRenderer&) = delete;
        MetalRenderer(MetalRenderer&&) = delete;
        MetalRenderer& operator=(MetalRenderer&&) = delete;

        void Render() override;

        void InternalAddRenderPass(RenderPass renderPass, std::size_t insertionIndex) override;
        void InternalRemoveRenderPass(std::size_t index) override;
        void InternalAddRenderableObject(RenderableObject renderableObject, std::size_t renderableIndex) override;
        void InternalRemoveRenderableObject(std::size_t index) override;

        void OnResize(std::uint32_t width, std::uint32_t height) override;
        void SetClearColor(float red, float green, float blue, float alpha) override;

        [[nodiscard]] AssetLoader& GetAssetLoader() override { return m_AssetLoader; }

        id<MTLCommandQueue> m_CommandQueue;
        std::vector<MetalMesh> m_MetalMeshes;
        std::vector<id<MTLRenderPipelineState>> m_MetalRenderPasses;
        std::shared_ptr<MetalWindow> m_Window; // We need to access layer and device
        MetalAssetLoader m_AssetLoader;
    };
} // namespace Nebula
