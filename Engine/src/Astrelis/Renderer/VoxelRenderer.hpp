#pragma once

#include "BaseRenderer.hpp"
#include "VertexBuffer.hpp"

#include "Astrelis/Core/Geometry.hpp"
#include "Astrelis/Core/Math.hpp"
#include "Astrelis/Core/Pointer.hpp"
#include "Astrelis/Core/Window.hpp"
#include "Astrelis/Renderer/Camera.hpp"
#include "Astrelis/Scene/VoxelScene.hpp"

namespace Astrelis
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
        VoxelRenderer(RefPtr<Window> window, Rect2Di viewport);
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

} // namespace Astrelis
