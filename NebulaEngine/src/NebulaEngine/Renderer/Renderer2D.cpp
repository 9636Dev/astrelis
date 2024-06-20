#include "Renderer2D.hpp"

#include "NebulaEngine/Core/Log.hpp"
#include "NebulaEngine/Core/Profiler.hpp"
#include "Vertex.hpp"

#include "glm/ext/matrix_transform.hpp"

namespace Nebula
{
    const std::vector<Vertex2D> m_Vertices = {
        {{-0.5F, -0.5F, 0.0F}, {1.0F, 0.0F, 0.0F}},
        {{0.5F, -0.5F, 0.0F},  {0.0F, 1.0F, 0.0F}},
        {{0.5F, 0.5F, 0.0F},   {0.0F, 0.0F, 1.0F}},
        {{-0.5F, 0.5F, 0.0F},  {1.0F, 1.0F, 1.0F}}
    };
    const std::vector<std::uint32_t> m_Indices = {0, 1, 2, 2, 3, 0};

    constexpr std::uint32_t MAX_FRAMES_IN_FLIGHT = 2;

    Renderer2D::Renderer2D(RefPtr<Window> window, Bounds viewport) :
        m_Window(std::move(window)),
        m_Context(m_Window->GetGraphicsContext()),
        m_RendererAPI(RendererAPI::Create(m_Context, viewport, RendererAPI::Type::Renderer2D)),
        m_MaxFramesInFlight(MAX_FRAMES_IN_FLIGHT)
    {
    }

    bool Renderer2D::Init()
    {
        m_RendererAPI->Init();
        RendererAPI::CreateDetails details;
        details.MaxFramesInFlight    = m_MaxFramesInFlight;
        details.VertexBufferSize     = m_Vertices.size() * sizeof(Vertex2D);
        details.IndicesCount         = m_Indices.size();
        details.VertexInput.Stride   = sizeof(Vertex2D);
        details.VertexInput.Elements = {
            {offsetof(Vertex2D, Position), 3, 0},
            {offsetof(Vertex2D, Color),    3, 0}
        };
        details.UniformDescriptors = {
            {"MVP", 0, 1, sizeof(UniformBufferObject)},
        };

        m_Storage = m_RendererAPI->CreateComponents(details);

        m_Storage.m_VertexBuffer->SetData(m_Context, m_Vertices.data(), details.VertexBufferSize);
        m_Storage.m_IndexBuffer->SetData(m_Context, m_Indices.data(), details.IndicesCount);

        m_UBO.Model      = glm::mat4(1.0F);
        m_UBO.View       = glm::mat4(1.0F);
        m_UBO.Projection = glm::mat4(1.0F);

        return true;
    }

    void Renderer2D::Shutdown()
    {
        m_RendererAPI->WaitDeviceIdle();
        m_RendererAPI->DestroyComponents(m_Storage);
        m_RendererAPI->Shutdown();

        NEBULA_CORE_LOG_INFO("Renderer2D shutdown!");
    }

    void Renderer2D::BeginFrame()
    {
        if (m_SkipRender)
        {
            // We want it to skip both the BeginFrame and EndFrame
            return;
        }

        NEBULA_PROFILE_SCOPE("Renderer2D::BeginFrame");

        m_Storage.m_GraphicsPipeline->Bind(m_Context);
        Bounds scissor = m_RendererAPI->GetSurfaceSize();
        Viewport viewport(0.0F, 0.0F, static_cast<float>(scissor.Width), static_cast<float>(scissor.Height));
        m_RendererAPI->SetViewport(viewport);
        m_RendererAPI->SetScissor(scissor);

        m_Storage.m_VertexBuffer->Bind(m_Context);
        m_Storage.m_IndexBuffer->Bind(m_Context);

        m_UBO.Model = glm::rotate(m_UBO.Model, glm::radians(0.1F), glm::vec3(0.0F, 0.0F, 1.0F));

        //m_Storage.m_UniformBuffers[m_Storage.m_CurrentFrame]->SetData(&m_UBO, sizeof(UniformBufferObject), 0);

        m_RendererAPI->DrawInstancedIndexed(static_cast<std::uint32_t>(m_Indices.size()), 1, 0, 0, 0);
    }

    void Renderer2D::EndFrame()
    {
        if (m_SkipRender)
        {
            m_SkipRender = false;
            return;
        }

        NEBULA_PROFILE_SCOPE("Renderer2D::EndFrame");
    }

    void Renderer2D::ResizeViewport(Bounds& viewport)
    {
        while (viewport.Width == 0 || viewport.Height == 0)
        {
            viewport = m_Window->GetViewportBounds();
            m_Window->WaitForEvents();
        }
        m_RendererAPI->ResizeViewport(m_Storage, viewport);
    }

} // namespace Nebula
