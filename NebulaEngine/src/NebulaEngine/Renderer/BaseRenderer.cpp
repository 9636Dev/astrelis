#include "BaseRenderer.hpp"
#include "NebulaEngine/Core/Assert.hpp"

#include <utility>

namespace Nebula
{
    BaseRenderer::BaseRenderer(RefPtr<Window> window, Bounds viewport) :
        m_Window(std::move(window)),
        m_Context(m_Window->GetGraphicsContext()),
        m_RendererAPI(RendererAPI::Create(m_Context, viewport, RendererAPI::Type::Renderer2D))
    {
    }

    bool BaseRenderer::Init()
    {
        InitAPI();
        return InitComponents();
    }

    void BaseRenderer::InitAPI()
    {
        m_RendererAPI->Init();
    }

    void BaseRenderer::Shutdown()
    {
        m_RendererAPI->Shutdown();
    }

    void BaseRenderer::InternalBeginFrame()
    {
        NEBULA_CORE_ASSERT(m_Pipeline != nullptr, "m_Pipeline is null, did you initialize it in 'Init()'?")
        m_Pipeline->Bind(m_Context);
        Bounds scissor = m_RendererAPI->GetSurfaceSize();
        Viewport viewport(0.0F, 0.0F, static_cast<float>(scissor.Width), static_cast<float>(scissor.Height));
        m_RendererAPI->SetViewport(viewport);
        m_RendererAPI->SetScissor(scissor);
    }

    void BaseRenderer::ResizeViewport() { m_RendererAPI->ResizeViewport(); }
} // namespace Nebula
