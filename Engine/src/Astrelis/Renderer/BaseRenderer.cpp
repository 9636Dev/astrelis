#include "BaseRenderer.hpp"
#include "Astrelis/Core/Base.hpp"

namespace Astrelis
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
        ASTRELIS_PROFILE_FUNCTION();
        ASTRELIS_CORE_ASSERT(m_Pipeline != nullptr, "m_Pipeline is null, did you initialize it in 'Init()'?")
        m_Pipeline->Bind(m_Context);
        Rect2Di scissor = m_RendererAPI->GetSurfaceSize();

        Rect3Df viewport;
        switch (RendererAPI::GetAPI())
        {
        case RendererAPI::API::None:
            ASTRELIS_CORE_ASSERT(false, "RendererAPI::API::None is not supported!")
            break;
        case RendererAPI::API::Vulkan:
            viewport = {0.0F, 0.0F, 0.0F, static_cast<float>(scissor.Width()), static_cast<float>(scissor.Height()),
                        1.0F};
            ASTRELIS_CORE_ASSERT(viewport.Z() == 0.0F, "Viewport Z should be 0.0F")
            ASTRELIS_CORE_ASSERT(viewport.Depth() == 1.0F, "Viewport Depth should be 1.0F")
            break;
        }
        ASTRELIS_CORE_ASSERT(viewport.X() == 0.0F && viewport.Y() == 0.0F, "Viewport X and Y should be 0.0F")
        ASTRELIS_CORE_ASSERT(static_cast<std::int32_t>(viewport.Width()) == scissor.Width() &&
                                 static_cast<std::int32_t>(viewport.Height()) == scissor.Height(),
                             "Viewport Width and Height should be the same as the scissor")

        m_RendererAPI->SetViewport(viewport);
        m_RendererAPI->SetScissor(scissor);
    }

    void BaseRenderer::ResizeViewport() { m_RendererAPI->ResizeViewport(); }
} // namespace Astrelis
