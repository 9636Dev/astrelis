#include "Renderer2D.hpp"

#include "NebulaEngine/Core/Log.hpp"
#include "NebulaEngine/Core/Profiler.hpp"
#include "Vertex.hpp"

#include <limits>

namespace Nebula
{
    const std::vector<Vertex2D> m_Vertices = {
        {{-0.5F, -0.5F}, {1.0F, 0.0F, 0.0F}},
        {{0.5F, -0.5F}, {0.0F, 1.0F, 0.0F}},
        {{0.5F, 0.5F}, {0.0F, 0.0F, 1.0F}},
        {{-0.5F, 0.5F}, {1.0F, 1.0F, 1.0F}}
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
        details.MaxFramesInFlight = m_MaxFramesInFlight;
        details.VertexBufferSize   = m_Vertices.size() * sizeof(Vertex2D);
        details.IndicesCount       = m_Indices.size();
        details.VertexInput.Stride = sizeof(Vertex2D);
        details.VertexInput.Elements = {
            {offsetof(Vertex2D, Position), 2, 0},
            {offsetof(Vertex2D, Color), 3, 0}
        };

        m_Storage                 = m_RendererAPI->CreateComponents(details);

        m_Storage.m_VertexBuffer->SetData(m_Context, m_Storage.m_CommandPool,m_Vertices.data(), details.VertexBufferSize);
        m_Storage.m_IndexBuffer->SetData(m_Context, m_Storage.m_CommandPool, m_Indices.data(), details.IndicesCount);

        return true;
    }

    void Renderer2D::Shutdown()
    {
        m_RendererAPI->WaitDeviceIdle(m_Context);
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
        m_Storage.m_InFlightFences[m_Storage.m_CurrentFrame]->Wait(m_Context, std::numeric_limits<std::uint64_t>::max());
        m_RendererAPI->AcquireNextImage(m_Context, m_Storage.m_ImageAvailableSemaphores[m_Storage.m_CurrentFrame], m_ImageIndex);

        if (m_RendererAPI->NeedsResize())
        {
            m_Storage.m_CommandBuffers[m_Storage.m_CurrentFrame]->Submit(
                m_Context, m_Storage.m_ImageAvailableSemaphores[m_Storage.m_CurrentFrame],
                m_Storage.m_RenderFinishedSemaphores[m_Storage.m_CurrentFrame], m_Storage.m_InFlightFences[m_Storage.m_CurrentFrame]);

            Bounds viewport = m_Window->GetViewportBounds();
            ResizeViewport(viewport);
            m_SkipRender = true;
            return;
        }

        m_Storage.m_InFlightFences[m_Storage.m_CurrentFrame]->Reset(m_Context);

        m_Storage.m_CommandBuffers[m_Storage.m_CurrentFrame]->Reset();
        m_Storage.m_CommandBuffers[m_Storage.m_CurrentFrame]->Begin();
        m_Storage.m_RenderPass->Begin(m_Context, m_Storage.m_CommandBuffers[m_Storage.m_CurrentFrame], m_Storage.m_FrameBuffers,
                                      m_ImageIndex);

        m_Storage.m_GraphicsPipeline->Bind(m_Storage.m_CommandBuffers[m_Storage.m_CurrentFrame]);
        Bounds scissor = m_RendererAPI->GetSurfaceSize();
        Viewport viewport(0.0F, 0.0F, static_cast<float>(scissor.Width), static_cast<float>(scissor.Height));
        m_RendererAPI->SetViewport(m_Storage.m_CommandBuffers[m_Storage.m_CurrentFrame], viewport);
        m_RendererAPI->SetScissor(m_Storage.m_CommandBuffers[m_Storage.m_CurrentFrame], scissor);

        m_Storage.m_VertexBuffer->Bind(m_Storage.m_CommandBuffers[m_Storage.m_CurrentFrame]);
        m_Storage.m_IndexBuffer->Bind(m_Storage.m_CommandBuffers[m_Storage.m_CurrentFrame]);

        m_RendererAPI->DrawInstancedIndexed(m_Storage.m_CommandBuffers[m_Storage.m_CurrentFrame], static_cast<std::uint32_t>(m_Indices.size()), 1, 0, 0, 0);
    }

    void Renderer2D::EndFrame()
    {
        if (m_SkipRender)
        {
            m_SkipRender = false;
            return;
        }

        NEBULA_PROFILE_SCOPE("Renderer2D::EndFrame");
        m_Storage.m_RenderPass->End(m_Context, m_Storage.m_CommandBuffers[m_Storage.m_CurrentFrame]);
        m_Storage.m_CommandBuffers[m_Storage.m_CurrentFrame]->End();
        m_Storage.m_CommandBuffers[m_Storage.m_CurrentFrame]->Submit(
            m_Context, m_Storage.m_ImageAvailableSemaphores[m_Storage.m_CurrentFrame],
            m_Storage.m_RenderFinishedSemaphores[m_Storage.m_CurrentFrame], m_Storage.m_InFlightFences[m_Storage.m_CurrentFrame]);

        m_RendererAPI->Present(m_ImageIndex, m_Storage.m_RenderFinishedSemaphores[m_Storage.m_CurrentFrame]);

        if (m_RendererAPI->NeedsResize())
        {
            Bounds viewport = m_Window->GetViewportBounds();
            ResizeViewport(viewport);
        }

        m_Storage.m_CurrentFrame = (m_Storage.m_CurrentFrame + 1) % m_MaxFramesInFlight;
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
