#include "RendererAPI.hpp"

#include "NebulaEngine/Core/Assert.hpp"

#ifdef NEBULA_RENDERER_VULKAN
    #include "Platform/Vulkan/VulkanRendererHelper.hpp"
#endif


namespace Nebula
{
#ifdef NEBULA_RENDERER_VULKAN
    RendererAPI::API RendererAPI::s_API = RendererAPI::API::Vulkan;
#else
    RendererAPI::API RendererAPI::s_API = RendererAPI::API::None;
#endif
    RendererAPI::BufferingMode RendererAPI::s_BufferingMode = RendererAPI::BufferingMode::Double;

    RefPtr<RendererAPI> RendererAPI::Create(RefPtr<GraphicsContext> context, Bounds viewport, Type type)
    {
        switch (GetAPI())
        {
        case RendererAPI::API::None:
            NEBULA_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        case RendererAPI::API::Vulkan:
#ifdef NEBULA_RENDERER_VULKAN
            return VulkanRendererHelper::CreateAPI(std::move(context), viewport, type);
#else
            NEBULA_CORE_ASSERT(false, "RendererAPI::Vulkan was not configured to be included in the build!");
            return nullptr;
#endif
        default:
            NEBULA_CORE_ASSERT(false, "Unknown RendererAPI!");
            return nullptr;
        }
    }
} // namespace Nebula
