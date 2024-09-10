#include "VulkanRendererHelper.hpp"

#include "Astrelis/Core/Assert.hpp"
#include "Platform/Vulkan/VulkanGraphicsContext.hpp"
#include "Vulkan2DRendererAPI.hpp"

namespace Astrelis
{
    RefPtr<RendererAPI>
        VulkanRendererHelper::CreateAPI(RefPtr<GraphicsContext> context, RendererAPI::Type type)
    {
        switch (type)
        {
        case RendererAPI::Type::Renderer2D:
            return static_cast<RefPtr<RendererAPI>>(
                Vulkan2DRendererAPI::Create(context.DynamicCast<VulkanGraphicsContext>()));
        case RendererAPI::Type::Renderer3D:
        case RendererAPI::Type::RayTracer:
            ASTRELIS_CORE_ASSERT(false, "RendererAPI::Type is currently not supported!");
            return nullptr;
        default:
            ASTRELIS_CORE_ASSERT(false, "Unknown RendererAPI::Type!");
            return nullptr;
        };
    }
} // namespace Astrelis
