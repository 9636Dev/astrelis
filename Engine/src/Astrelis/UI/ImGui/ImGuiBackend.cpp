#include "ImGuiBackend.hpp"

#include "Astrelis/Core/Base.hpp"

#include "Astrelis/Renderer/RendererAPI.hpp"

#ifdef ASTRELIS_RENDERER_VULKAN
    #include "Platform/Vulkan/VulkanImGuiBackend.hpp"
#endif

namespace Astrelis {
    RefPtr<ImGuiBackend> ImGuiBackend::Create(RefPtr<Window> window) {
        switch (RendererAPI::GetAPI()) {
        case RendererAPI::API::None:
            ASTRELIS_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        case RendererAPI::API::Vulkan:
#ifdef ASTRELIS_RENDERER_VULKAN
            return static_cast<RefPtr<ImGuiBackend>>(RefPtr<VulkanImGuiBackend>::Create(
                window, RendererAPI::Create(window->GetGraphicsContext())));
#else
            ASTRELIS_CORE_ASSERT(false, "RendererAPI::Vulkan is currently not supported!");
            return nullptr;
#endif
        default:
            ASTRELIS_CORE_ASSERT(false, "Unknown RendererAPI!");
            return nullptr;
        };
    }
} // namespace Astrelis
