#pragma once

#include "Astrelis/Core/Geometry.hpp"
#include "Astrelis/Core/Math.hpp"
#include "Astrelis/Core/Pointer.hpp"
#include "Astrelis/Core/Window.hpp"
#include "Astrelis/Renderer/BaseRenderer.hpp"
#include "Astrelis/Renderer/BindingDescriptor.hpp"
#include "Astrelis/Renderer/TextureImage.hpp"

#include "VertexBuffer.hpp"

namespace Astrelis {
    struct CameraUniformData {
        Mat4f View       = Mat4f(1.0F);
        Mat4f Projection = Mat4f(1.0F);
    };

    struct InstanceData {
        Mat4f Transform;
    };

    class Renderer2D : public BaseRenderer {
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
        void EndFrame() override;

        void AddInstance(const InstanceData& instance);
    private:
        void DrawInstances();

        // ========================
        // Rendering States
        // ========================
        RefPtr<VertexBuffer>         m_VertexBuffer;
        RefPtr<VertexBuffer>         m_InstanceBuffer;
        RefPtr<IndexBuffer>          m_IndexBuffer;
        CameraUniformData            m_UBO;
        RefPtr<BindingDescriptorSet> m_Bindings;
        RefPtr<UniformBuffer>        m_UniformBuffer;
        RefPtr<TextureImage>         m_TextureImage;
        RefPtr<TextureSampler>       m_TextureSampler;

        // ========================
        // Rendering Data
        // ========================

        // For now everything is a quad
        std::vector<InstanceData> m_Instances;
    };

} // namespace Astrelis
