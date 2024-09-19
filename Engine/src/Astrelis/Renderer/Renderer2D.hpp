#pragma once

#include "Astrelis/Core/Geometry.hpp"
#include "Astrelis/Core/Math.hpp"
#include "Astrelis/Core/Pointer.hpp"
#include "Astrelis/Core/Window.hpp"

#include "BaseRenderer.hpp"
#include "BindingDescriptor.hpp"
#include "Mesh.hpp"
#include "TextureImage.hpp"
#include "VertexBuffer.hpp"

namespace Astrelis {
    struct CameraUniformData {
        Mat4f View       = Mat4f(1.0F);
        Mat4f Projection = Mat4f(1.0F);
    };

    struct InstanceData {
        Mat4f Transform;
        Vec3f Color;
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

        void Submit(const Mesh2D& mesh, InstanceData instance);
    private:
        // ========================
        // Rendering States
        // ========================
        RefPtr<VertexBuffer>         m_VertexBuffer;
        RefPtr<VertexBuffer>         m_InstanceBuffer;
        RefPtr<IndexBuffer>          m_IndexBuffer;
        CameraUniformData            m_UBO;
        RefPtr<BindingDescriptorSet> m_Bindings;
        RefPtr<UniformBuffer>        m_UniformBuffer;

        // ========================
        // Rendering Data
        // ========================

        // For now everything is a quad
        std::vector<InstanceData> m_Instances;
    };

} // namespace Astrelis
