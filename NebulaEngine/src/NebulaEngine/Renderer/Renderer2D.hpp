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
        explicit Renderer2D(const RefPtr<Window>& window, Bounds viewport);
        ~Renderer2D() = default;
        Renderer2D(const Renderer2D&)            = delete;
        Renderer2D& operator=(const Renderer2D&) = delete;
        Renderer2D(Renderer2D&&)                 = delete;
        Renderer2D& operator=(Renderer2D&&)      = delete;

        bool Init();
        void Shutdown();

        void BeginFrame();
        void EndFrame();

    private:
        RefPtr<GraphicsContext> m_Context;
        RefPtr<RendererAPI> m_RendererAPI;
        Renderer2DStorage m_Storage;
        std::uint32_t m_FrameIndex = 0;
    };

} // namespace Nebula
