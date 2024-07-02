#include "RenderSystem.hpp"

#include "NebulaEngine/Core/Assert.hpp"
#include "NebulaEngine/Renderer/RendererAPI.hpp"

#ifdef NEBULA_RENDERER_VULKAN
    #include "Platform/Vulkan/VulkanRenderSystem.hpp"
#endif

namespace Nebula
{
    RefPtr<RenderSystem> RenderSystem::Create(RefPtr<Window>& window)
    {
        switch (RendererAPI::GetAPI())
        {
        case RendererAPI::API::None:
            NEBULA_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        case RendererAPI::API::Vulkan:
#ifdef NEBULA_RENDERER_VULKAN
            return static_cast<RefPtr<RenderSystem>>(VulkanRenderSystem::Create(window));
#else
            NEBULA_CORE_ASSERT(false, "RendererAPI::Vulkan is not supported!");
            return nullptr;
#endif
        default:
            NEBULA_CORE_ASSERT(false, "Unknown RendererAPI!");
            return nullptr;
        }
    }
} // namespace Nebula
