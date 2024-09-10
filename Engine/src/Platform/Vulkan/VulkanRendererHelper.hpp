#pragma once

#include "Astrelis/Renderer/RendererAPI.hpp"

namespace Astrelis
{
    class VulkanRendererHelper
    {
    public:
        static RefPtr<RendererAPI> CreateAPI(RefPtr<GraphicsContext> context, RendererAPI::Type type);
    };
} // namespace Astrelis
