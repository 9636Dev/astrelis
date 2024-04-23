#include "Renderer.hpp"
#include "NebulaGraphicsCore/Renderer.hpp"
#include "Core.hpp"

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

extern "C" NEBULA_GRAPHICS_METAL_API Nebula::RendererCreationResult
    nebulaGraphicsMetalCreateMetalRenderer(const Nebula::MetalContext& context,
                                             const std::shared_ptr<Nebula::Window>& window)
{
    std::shared_ptr<Nebula::MetalWindow> windowPtr = std::dynamic_pointer_cast<Nebula::MetalWindow>(window);
    if (windowPtr == nullptr)
    {
        return {nullptr, Nebula::RendererCreationResult::ErrorType::InvalidArguments};
    }

    auto renderer = std::make_shared<Nebula::MetalRenderer>(windowPtr);
    return {renderer, Nebula::RendererCreationResult::ErrorType::None};
}
