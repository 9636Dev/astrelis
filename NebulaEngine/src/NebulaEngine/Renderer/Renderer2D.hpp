#pragma once

#include "NebulaEngine/Core/Geometry.hpp"
#include "NebulaEngine/Core/Math.hpp"
#include "NebulaEngine/Core/Pointer.hpp"
#include "NebulaEngine/Core/Window.hpp"
#include "NebulaEngine/Renderer/BaseRenderer.hpp"
#include "NebulaEngine/Renderer/TextureImage.hpp"
#include "NebulaEngine/Scene/Scene2D.hpp"

#include "Camera.hpp"
#include "VertexBuffer.hpp"

namespace Nebula
{
    struct CameraUniformData
    {
        Mat4f View       = Mat4f(1.0F);
        Mat4f Projection = Mat4f(1.0F);
    };

    struct InstanceData
    {
        Mat4f Transform;
    };

    class Renderer2D : public BaseRenderer
    {
    public:
        Renderer2D(RefPtr<Window> window, Rect2Di viewport);
        ~Renderer2D() override                   = default;
        Renderer2D(const Renderer2D&)            = delete;
        Renderer2D& operator=(const Renderer2D&) = delete;
        Renderer2D(Renderer2D&&)                 = delete;
        Renderer2D& operator=(Renderer2D&&)      = delete;

        bool InitComponents() override;
        void Shutdown() override;

        void BeginFrame() override;
        void RenderScene(Scene2D& scene, Camera& camera);
        void EndFrame() override;
    private:
        void DrawInstances();

        // ========================
        // Rendering States
        // ========================
        RefPtr<VertexBuffer> m_VertexBuffer;
        RefPtr<VertexBuffer> m_InstanceBuffer;
        RefPtr<IndexBuffer> m_IndexBuffer;
        CameraUniformData m_UBO;
        RefPtr<DescriptorSetLayout> m_DescriptorSetLayout;
        RefPtr<DescriptorSets> m_DescriptorSets;
        RefPtr<UniformBuffer> m_UniformBuffer;
        RefPtr<TextureImage> m_TextureImage;
        RefPtr<TextureSampler> m_TextureSampler;

        // ========================
        // Rendering Data
        // ========================

        // For now everything is a quad
        std::vector<InstanceData> m_Instances;
    };

} // namespace Nebula
