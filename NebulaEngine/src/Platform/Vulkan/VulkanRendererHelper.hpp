#pragma once

#include "NebulaEngine/Renderer/RendererAPI.hpp"

namespace Nebula
{
    class VulkanRendererHelper
    {
    public:
        static RefPtr<RendererAPI> CreateAPI(RefPtr<GraphicsContext> context, Bounds viewport, RendererAPI::Type type);
    };
} // namespace Nebula
