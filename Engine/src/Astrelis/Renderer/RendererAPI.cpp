#include "RendererAPI.hpp"

#include "Astrelis/Core/Base.hpp"

#include "Astrelis/Core/Application.hpp"

#ifdef ASTRELIS_RENDERER_VULKAN
    #include "Platform/Vulkan/VulkanRendererHelper.hpp"
#endif


namespace Astrelis
{
    void RendererAPI::SetAPI(API api)
    {
        if (Application::HasInstance())
        {
            ASTRELIS_CORE_LOG_WARN(
                "RendererAPI::SetAPI() called after Application was created, this will have no effect");
            return;
        }
        s_API = api;
    }

    void RendererAPI::SetBufferingMode(BufferingMode mode)
    {
        if (Application::HasInstance())
        {
            ASTRELIS_CORE_LOG_WARN(
                "RendererAPI::SetBufferingMode() called after Application was created, this will have no effect");
            return;
        }
        s_BufferingMode = mode;
    }


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
