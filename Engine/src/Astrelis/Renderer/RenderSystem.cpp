#include "RenderSystem.hpp"

#include "Astrelis/Core/Base.hpp"

#include "RendererAPI.hpp"

#ifdef ASTRELIS_RENDERER_VULKAN
    #include "Platform/Vulkan/VulkanRenderSystem.hpp"
#endif

namespace Astrelis {
    RefPtr<RenderSystem> RenderSystem::Create(RefPtr<Window>& window) {
        switch (RendererAPI::GetAPI()) {
        case RendererAPI::API::None:
            ASTRELIS_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        case RendererAPI::API::Vulkan:
#ifdef ASTRELIS_RENDERER_VULKAN
            return static_cast<RefPtr<RenderSystem>>(VulkanRenderSystem::Create(window));
#else
            ASTRELIS_CORE_ASSERT(false, "RendererAPI::Vulkan is not supported!");
            return nullptr;
#endif
        default:
            ASTRELIS_CORE_ASSERT(false, "Unknown RendererAPI!");
            return nullptr;
        }
    }
} // namespace Astrelis
