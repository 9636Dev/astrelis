#include "Renderer.hpp"

namespace Nebula
{
    MetalRenderer::MetalRenderer(std::shared_ptr<MetalWindow> window)
        : m_Window(std::move(window))
    {
    }

    MetalRenderer::~MetalRenderer() = default;

    void MetalRenderer::Render()
    {

    }
} // namespace Nebula
