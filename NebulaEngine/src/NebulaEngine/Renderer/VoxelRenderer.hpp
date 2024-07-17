#pragma once

#include "NebulaEngine/Core/Bounds.hpp"
#include "NebulaEngine/Core/Math.hpp"
#include "NebulaEngine/Core/Pointer.hpp"
#include "NebulaEngine/Core/Window.hpp"
#include "NebulaEngine/Renderer/Camera.hpp"
#include "NebulaEngine/Scene/VoxelScene.hpp"

#include "BaseRenderer.hpp"
#include "TextureImage.hpp"
#include "VertexBuffer.hpp"

namespace Nebula
{
    struct CameraUniformData
    {
        Mat4f View;
        Mat4f Projection;
    };

    struct ChunkUniformData
    {
        Vec3i Position;
    };

    struct VoxelVertex
    {
        Vec3f Position;
    };

    struct VoxelInstance
    {
        std::uint32_t Offset;
        Vec4f Color;

        VoxelInstance(std::uint32_t offset, Vec4f color) : Offset(offset), Color(color) {}
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
        void RenderScene(VoxelScene& scene, Camera& camera);
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
        RefPtr<UniformBuffer> m_ChunkUniform;
        //RefPtr<TextureImage> m_TextureImage;
        //RefPtr<TextureSampler> m_TextureSampler;

        // ========================
        // Rendering Data
        // ========================
        CameraUniformData m_CameraData;
        ChunkUniformData m_ChunkData;
        std::vector<VoxelInstance> m_InstanceData;
    };

} // namespace Nebula
