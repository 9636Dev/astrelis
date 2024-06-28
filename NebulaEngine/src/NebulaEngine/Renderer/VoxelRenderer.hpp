#pragma once

#include "NebulaEngine/Core/Bounds.hpp"
#include "NebulaEngine/Core/Pointer.hpp"
#include "NebulaEngine/Core/Window.hpp"

#include "BaseRenderer.hpp"
#include "TextureImage.hpp"
#include "VertexBuffer.hpp"

#include <chrono>
#include <glm/glm.hpp>

namespace Nebula
{
    struct UniformBufferObject
    {
        glm::mat4 View;
        glm::mat4 Projection;
    };

    class VoxelRenderer : public BaseRenderer
    {
    public:
        VoxelRenderer(RefPtr<Window> window, Bounds viewport);
        ~VoxelRenderer() override                      = default;
        VoxelRenderer(const VoxelRenderer&)            = delete;
        VoxelRenderer& operator=(const VoxelRenderer&) = delete;
        VoxelRenderer(VoxelRenderer&&)                 = delete;
        VoxelRenderer& operator=(VoxelRenderer&&)      = delete;

        bool InitComponents() override;
        void Shutdown() override;

        void BeginFrame() override;
        void EndFrame() override;
    private:
        // ========================
        // Rendering States
        // ========================
        RefPtr<VertexBuffer> m_VertexBuffer;
        RefPtr<VertexBuffer> m_InstanceBuffer;
        RefPtr<IndexBuffer> m_IndexBuffer;
        RefPtr<DescriptorSetLayout> m_DescriptorSetLayout;
        RefPtr<DescriptorSets> m_DescriptorSets;
        RefPtr<UniformBuffer> m_UniformBuffer;
        //RefPtr<TextureImage> m_TextureImage;
        //RefPtr<TextureSampler> m_TextureSampler;

        // ========================
        // Rendering Data
        // ========================
        UniformBufferObject m_UBO;
    };

} // namespace Nebula
