#include "Renderer2D.hpp"

#include "NebulaEngine/Core/Log.hpp"

namespace Nebula
{
    Renderer2D::Renderer2D(const RefPtr<Window>& window, Bounds viewport) :
        m_Context(window->GetGraphicsContext()), m_RendererAPI(RendererAPI::Create(window->GetGraphicsContext(), viewport, RendererAPI::Type::Renderer2D))
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
        m_RendererAPI->DestroyComponents(m_Storage);
        m_RendererAPI->Shutdown();

        NEBULA_CORE_LOG_INFO("Renderer2D shutdown!");
    }

    void Renderer2D::BeginFrame()
    {
        m_Storage.m_CommandBuffer->Begin();
        m_Storage.m_RenderPass->Begin(m_Context, m_Storage.m_CommandBuffer, m_Storage.m_FrameBuffers, m_FrameIndex);
    }

    void Renderer2D::EndFrame()
    {
        m_Storage.m_RenderPass->End(m_Context, m_Storage.m_CommandBuffer);
        m_Storage.m_CommandBuffer->End();
    }

} // namespace Nebula
