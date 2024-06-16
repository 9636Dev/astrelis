#include "Renderer2D.hpp"

#include "NebulaEngine/Renderer/Shader.hpp"
#include "NebulaEngine/Renderer/VertexArray.hpp"
#include "RenderCommand.hpp"

#include <glad/glad.h>

namespace Nebula
{
    struct Renderer2DData
    {
        // Not sure about alignment here
        struct QuadVertex
        {
            std::array<float, 2> Position;
            std::array<float, 4> Color;
        };

        static const uint32_t MaxQuads        = 1'000;
        static const uint32_t MaxVertices     = MaxQuads * 4;
        static const uint32_t MaxIndices      = MaxQuads * 6;
        static const uint32_t MaxTextureSlots = 32;

        Ptr<VertexArray> QuadVertexArray;
        Ptr<VertexBuffer> QuadVertexBuffer;
        Ptr<IndexBuffer> QuadIndexBuffer;
        std::vector<QuadVertex> QuadVertexBufferBase;
        std::vector<std::uint32_t> QuadIndexBufferBase;
        Ptr<Shader> QuadShader;
    };

    // NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
    static Renderer2DData s_Data;

    void Renderer2D::Init()
    {
        RenderCommand::Init();

        s_Data.QuadVertexArray = VertexArray::Create();
        s_Data.QuadVertexBuffer =
            VertexBuffer::Create(Renderer2DData::MaxVertices * sizeof(Renderer2DData::QuadVertex));
        s_Data.QuadVertexBuffer->SetLayout({
            {ShaderDataType::Float2, "a_Position"},
            {ShaderDataType::Float4, "a_Color"   }
        });
        s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);
        s_Data.QuadIndexBuffer = IndexBuffer::Create(Renderer2DData::MaxIndices);
        s_Data.QuadVertexArray->SetIndexBuffer(s_Data.QuadIndexBuffer.GetRef());

        {
            const char* vertexShaderSource   = R"(
                #version 330 core

                layout(location = 0) in vec2 a_Position;
                layout(location = 1) in vec4 a_Color;

                out vec4 v_Color;

                void main()
                {
                    v_Color = a_Color;
                    gl_Position = vec4(a_Position, 0.0, 1.0);
                }
            )";
            const char* fragmentShaderSource = R"(
                #version 330 core

                in vec4 v_Color;

                out vec4 color;

                void main()
                {
                    color = v_Color;
                }
            )";

            s_Data.QuadShader = Shader::Create("BasicQuad", vertexShaderSource, fragmentShaderSource);
            NEBULA_CORE_ASSERT(s_Data.QuadShader, "Failed to create QuadShader");
        }
    }

    void Renderer2D::Shutdown()
    {
        s_Data.QuadVertexArray.Reset();
        s_Data.QuadVertexBuffer.Reset();
        s_Data.QuadIndexBuffer.Reset();
        s_Data.QuadShader.Reset();
    }

    void Renderer2D::SetViewport(const Bounds& bounds)
    {
        RenderCommand::SetViewport(bounds.X, bounds.Y, bounds.Width, bounds.Height);
    }

    void Renderer2D::BeginScene() { StartBatch(); }

    void Renderer2D::EndScene() { Flush(); }

    void Renderer2D::DrawQuad(const Vector2f& position, const Vector2f& size, const Vector4f& color)
    {
        if (s_Data.QuadVertexBufferBase.size() >= Renderer2DData::MaxVertices)
        {
            NextBatch();
        }

        float posX = position.x() - size.x() / 2.0F;
        float posY = position.y() - size.y() / 2.0F;

        s_Data.QuadVertexBufferBase.reserve(4);
        s_Data.QuadVertexBufferBase.push_back({
            {posX, posY},
            {color.x(), color.y(), color.z(), color.w()}
        });
        s_Data.QuadVertexBufferBase.push_back({
            {posX + size.x(), posY},
            {color.x(), color.y(), color.z(), color.w()}
        });
        s_Data.QuadVertexBufferBase.push_back({
            {posX + size.x(), posY + size.y()},
            {color.x(), color.y(), color.z(), color.w()}
        });
        s_Data.QuadVertexBufferBase.push_back({
            {posX, posY + size.y()},
            {color.x(), color.y(), color.z(), color.w()}
        });

        std::uint32_t curSize = s_Data.QuadVertexBufferBase.size();
        s_Data.QuadIndexBufferBase.reserve(6);
        s_Data.QuadIndexBufferBase.push_back(curSize - 4); // 0
        s_Data.QuadIndexBufferBase.push_back(curSize - 3); // 1
        s_Data.QuadIndexBufferBase.push_back(curSize - 2); // 2
        s_Data.QuadIndexBufferBase.push_back(curSize - 2); // 2
        s_Data.QuadIndexBufferBase.push_back(curSize - 1); // 3
        s_Data.QuadIndexBufferBase.push_back(curSize - 4); // 0
    }

    void Renderer2D::Flush() { EndBatch(); }

    void Renderer2D::StartBatch()
    {
        s_Data.QuadVertexBufferBase.clear();
        s_Data.QuadIndexBufferBase.clear();
    }

    void Renderer2D::EndBatch()
    {
        s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase.data(),
                                         s_Data.QuadVertexBufferBase.size() * sizeof(Renderer2DData::QuadVertex));
        s_Data.QuadIndexBuffer->SetData(s_Data.QuadIndexBufferBase.data(), s_Data.QuadIndexBufferBase.size());
        s_Data.QuadShader->Bind();
        RenderCommand::DrawIndexed(s_Data.QuadVertexArray);
    }

    void Renderer2D::NextBatch()
    {
        Flush();
        StartBatch();
    }
} // namespace Nebula
