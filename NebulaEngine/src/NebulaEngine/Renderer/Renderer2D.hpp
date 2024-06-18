#pragma once

#include "NebulaEngine/Core/Bounds.hpp"
#include "NebulaEngine/Core/Pointer.hpp"
#include "NebulaEngine/Core/Window.hpp"
#include "NebulaEngine/Renderer/GraphicsContext.hpp"
#include "NebulaEngine/Renderer/RendererAPI.hpp"
#include "NebulaEngine/Renderer/RendererStorage.hpp"

namespace Nebula
{
    class Renderer2D
    {
    public:
        explicit Renderer2D(RefPtr<Window> window, Bounds viewport);
        ~Renderer2D() = default;
        Renderer2D(const Renderer2D&)            = delete;
        Renderer2D& operator=(const Renderer2D&) = delete;
        Renderer2D(Renderer2D&&)                 = delete;
        Renderer2D& operator=(Renderer2D&&)      = delete;

        bool Init();
        void Shutdown();

        void BeginFrame();
        void EndFrame();

        void ResizeViewport(Bounds& viewport);

    private:
        RefPtr<Window> m_Window;
        RefPtr<GraphicsContext> m_Context;
        RefPtr<RendererAPI> m_RendererAPI;
        Renderer2DStorage m_Storage;
        std::uint32_t m_ImageIndex = 0;
        std::uint32_t m_CurrentFrame = 0;
        bool m_SkipRender = false;

        const std::uint32_t m_MaxFramesInFlight;
    };

} // namespace Nebula
