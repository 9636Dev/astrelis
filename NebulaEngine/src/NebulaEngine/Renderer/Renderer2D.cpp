#include "Renderer2D.hpp"

#include "NebulaEngine/Core/Log.hpp"
#include "NebulaEngine/Core/Profiler.hpp"

#include <limits>

namespace Nebula
{
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
        m_Storage                 = m_RendererAPI->CreateComponents(details);

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
        m_Storage.m_InFlightFences[m_CurrentFrame]->Wait(m_Context, std::numeric_limits<std::uint64_t>::max());
        m_RendererAPI->AcquireNextImage(m_Context, m_Storage.m_ImageAvailableSemaphores[m_CurrentFrame], m_ImageIndex);

        if (m_RendererAPI->NeedsResize())
        {
            m_Storage.m_CommandBuffers[m_CurrentFrame]->Submit(
                m_Context, m_Storage.m_ImageAvailableSemaphores[m_CurrentFrame],
                m_Storage.m_RenderFinishedSemaphores[m_CurrentFrame], m_Storage.m_InFlightFences[m_CurrentFrame]);

            Bounds viewport = m_Window->GetViewportBounds();
            ResizeViewport(viewport);
            m_SkipRender = true;
            return;
        }

        m_Storage.m_InFlightFences[m_CurrentFrame]->Reset(m_Context);

        m_Storage.m_CommandBuffers[m_CurrentFrame]->Reset();
        m_Storage.m_CommandBuffers[m_CurrentFrame]->Begin();
        m_Storage.m_RenderPass->Begin(m_Context, m_Storage.m_CommandBuffers[m_CurrentFrame], m_Storage.m_FrameBuffers,
                                      m_ImageIndex);

        m_Storage.m_GraphicsPipeline->Bind(m_Storage.m_CommandBuffers[m_CurrentFrame]);
        Bounds scissor = m_RendererAPI->GetSurfaceSize();
        Viewport viewport(0.0F, 0.0F, static_cast<float>(scissor.Width), static_cast<float>(scissor.Height));
        m_RendererAPI->SetViewport(m_Storage.m_CommandBuffers[m_CurrentFrame], viewport);
        m_RendererAPI->SetScissor(m_Storage.m_CommandBuffers[m_CurrentFrame], scissor);

        m_RendererAPI->DrawInstanced(m_Storage.m_CommandBuffers[m_CurrentFrame], 3, 1, 0, 0);
    }

    void Renderer2D::EndFrame()
    {
        if (m_SkipRender)
        {
            m_SkipRender = false;
            return;
        }

        NEBULA_PROFILE_SCOPE("Renderer2D::EndFrame");
        m_Storage.m_RenderPass->End(m_Context, m_Storage.m_CommandBuffers[m_CurrentFrame]);
        m_Storage.m_CommandBuffers[m_CurrentFrame]->End();
        m_Storage.m_CommandBuffers[m_CurrentFrame]->Submit(
            m_Context, m_Storage.m_ImageAvailableSemaphores[m_CurrentFrame],
            m_Storage.m_RenderFinishedSemaphores[m_CurrentFrame], m_Storage.m_InFlightFences[m_CurrentFrame]);

        m_RendererAPI->Present(m_ImageIndex, m_Storage.m_RenderFinishedSemaphores[m_CurrentFrame]);

        if (m_RendererAPI->NeedsResize())
        {
            Bounds viewport = m_Window->GetViewportBounds();
            ResizeViewport(viewport);
        }

        m_CurrentFrame = (m_CurrentFrame + 1) % m_MaxFramesInFlight;
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
