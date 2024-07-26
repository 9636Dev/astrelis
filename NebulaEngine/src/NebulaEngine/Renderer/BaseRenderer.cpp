#include "BaseRenderer.hpp"

#include <utility>

#include "NebulaEngine/Core/Assert.hpp"
#include "NebulaEngine/Core/Profiler.hpp"

namespace Nebula
{
    BaseRenderer::BaseRenderer(RefPtr<Window> window, Rect2Di viewport) :
        m_Window(std::move(window)),
        m_Context(m_Window->GetGraphicsContext()),
        m_RendererAPI(RendererAPI::Create(m_Context, RendererAPI::Type::Renderer2D))
    {
        (void)viewport;
    }

    bool BaseRenderer::Init()
    {
        InitAPI();
        return InitComponents();
    }

    void BaseRenderer::InitAPI() { m_RendererAPI->Init(); }

    void BaseRenderer::Shutdown() { m_RendererAPI->Shutdown(); }

    void BaseRenderer::InternalBeginFrame()
    {
        NEBULA_PROFILE_SCOPE("Nebula::BaseRenderer::InternalBeginFrame()");
        NEBULA_CORE_ASSERT(m_Pipeline != nullptr, "m_Pipeline is null, did you initialize it in 'Init()'?")
        m_Pipeline->Bind(m_Context);
        Rect2D scissor = m_RendererAPI->GetSurfaceSize();
        // TODO: 0 - 1 is only for Vulkan, need to change this to be more generic
        Rect3Df viewport(0.0F, 0.0F, 0.0F, static_cast<float>(scissor.Width()), static_cast<float>(scissor.Height()),
                         1.0F);
        m_RendererAPI->SetViewport(viewport);
        m_RendererAPI->SetScissor(scissor);
    }

    void BaseRenderer::ResizeViewport() { m_RendererAPI->ResizeViewport(); }
} // namespace Nebula
