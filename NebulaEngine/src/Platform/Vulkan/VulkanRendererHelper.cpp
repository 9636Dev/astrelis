#include "VulkanRendererHelper.hpp"

#include "NebulaEngine/Core/Assert.hpp"
#include "Platform/Vulkan/VulkanGraphicsContext.hpp"
#include "Vulkan2DRendererAPI.hpp"

namespace Nebula
{
    RefPtr<RendererAPI>
        VulkanRendererHelper::CreateAPI(RefPtr<GraphicsContext> context, Bounds viewport, RendererAPI::Type type)
    {
        switch (type)
        {
        case RendererAPI::Type::Renderer2D:
            return static_cast<RefPtr<RendererAPI>>(
                Vulkan2DRendererAPI::Create(context.DynamicCast<VulkanGraphicsContext>(), viewport));
        case RendererAPI::Type::Renderer3D:
        case RendererAPI::Type::RayTracer:
            NEBULA_CORE_ASSERT(false, "RendererAPI::Type is currently not supported!");
            return nullptr;
        default:
            NEBULA_CORE_ASSERT(false, "Unknown RendererAPI::Type!");
            return nullptr;
        };
    }
} // namespace Nebula
