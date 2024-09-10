#include "RendererAPI.hpp"

#include "Astrelis/Core/Assert.hpp"

#ifdef ASTRELIS_RENDERER_VULKAN
    #include "Platform/Vulkan/VulkanRendererHelper.hpp"
#endif


namespace Astrelis
{
#ifdef ASTRELIS_RENDERER_VULKAN
    RendererAPI::API RendererAPI::s_API = RendererAPI::API::Vulkan;
#else
    RendererAPI::API RendererAPI::s_API = RendererAPI::API::None;
#endif
    RendererAPI::BufferingMode RendererAPI::s_BufferingMode = RendererAPI::BufferingMode::Double;

    RefPtr<RendererAPI> RendererAPI::Create(RefPtr<GraphicsContext> context, Type type)
    {
        switch (GetAPI())
        {
        case RendererAPI::API::None:
            ASTRELIS_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        case RendererAPI::API::Vulkan:
#ifdef ASTRELIS_RENDERER_VULKAN
            return VulkanRendererHelper::CreateAPI(std::move(context), type);
#else
            ASTRELIS_CORE_ASSERT(false, "RendererAPI::Vulkan was not configured to be included in the build!");
            return nullptr;
#endif
        default:
            ASTRELIS_CORE_ASSERT(false, "Unknown RendererAPI!");
            return nullptr;
        }
    }
} // namespace Astrelis
