#pragma once

#include "NebulaGraphicsCore/Renderer.hpp"
#include "Window.hpp"

#include <memory>

namespace Nebula
{
    class MetalRenderer : public Renderer
    {
    public:
        explicit MetalRenderer(std::shared_ptr<MetalWindow> window);
        ~MetalRenderer() override;
        MetalRenderer(const MetalRenderer&) = delete;
        MetalRenderer& operator=(const MetalRenderer&) = delete;
        MetalRenderer(MetalRenderer&&) = delete;
        MetalRenderer& operator=(MetalRenderer&&) = delete;

        void Render() override;

    private:
        std::shared_ptr<MetalWindow> m_Window; // We need to access layer and device

    };
} // namespace Nebula
