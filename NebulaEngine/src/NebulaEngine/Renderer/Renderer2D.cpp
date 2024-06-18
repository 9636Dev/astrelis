#include "Renderer2D.hpp"

#include "NebulaEngine/Core/Log.hpp"

#include <limits>

namespace Nebula
{
    Renderer2D::Renderer2D(RefPtr<Window> window, Bounds viewport) :
        m_Window(std::move(window)),
        m_Context(m_Window->GetGraphicsContext()),
        m_RendererAPI(RendererAPI::Create(m_Context, viewport, RendererAPI::Type::Renderer2D))
    {
    }

    bool Renderer2D::Init()
    {
        m_RendererAPI->Init();
        m_Storage = m_RendererAPI->CreateComponents();

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
        m_Storage.m_InFlightFence->Wait(m_Context, std::numeric_limits<std::uint64_t>::max());
        m_RendererAPI->AcquireNextImage(m_Context, m_Storage.m_ImageAvailableSemaphore, m_ImageIndex);
        m_Storage.m_InFlightFence->Reset(m_Context);

        m_Storage.m_CommandBuffer->Reset();
        m_Storage.m_CommandBuffer->Begin();
        m_Storage.m_RenderPass->Begin(m_Context, m_Storage.m_CommandBuffer, m_Storage.m_FrameBuffers, m_ImageIndex);

        m_Storage.m_GraphicsPipeline->Bind(m_Storage.m_CommandBuffer);
        Bounds scissor = m_RendererAPI->GetSurfaceSize();
        Viewport viewport(0.0F, 0.0F, static_cast<float>(scissor.Width), static_cast<float>(scissor.Height));
        m_RendererAPI->SetViewport(m_Storage.m_CommandBuffer, viewport);
        m_RendererAPI->SetScissor(m_Storage.m_CommandBuffer, scissor);

        m_RendererAPI->DrawInstanced(m_Storage.m_CommandBuffer, 3, 1, 0, 0);
    }

    void Renderer2D::EndFrame()
    {
        m_Storage.m_RenderPass->End(m_Context, m_Storage.m_CommandBuffer);
        m_Storage.m_CommandBuffer->End();
        m_Storage.m_CommandBuffer->Submit(m_Context, m_Storage.m_ImageAvailableSemaphore, m_Storage.m_RenderFinishedSemaphore, m_Storage.m_InFlightFence);

        m_RendererAPI->Present(m_ImageIndex, m_Storage.m_RenderFinishedSemaphore);
    }

} // namespace Nebula
