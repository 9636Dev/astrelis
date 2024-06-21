#include "ImGuiBackend.hpp"

#include "NebulaEngine/Core/Assert.hpp"
#include "NebulaEngine/Renderer/RendererAPI.hpp"

#ifdef NEBULA_RENDERER_VULKAN
    #include "Platform/Vulkan/VulkanImGuiBackend.hpp"
#endif

namespace Nebula
{
    RefPtr<ImGuiBackend> ImGuiBackend::Create(RefPtr<Window> window)
    {
        switch (RendererAPI::GetAPI())
        {
        case RendererAPI::API::None:
            NEBULA_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        case RendererAPI::API::Vulkan:
#ifdef NEBULA_RENDERER_VULKAN
            return static_cast<RefPtr<ImGuiBackend>>(RefPtr<VulkanImGuiBackend>::Create(
                window, RendererAPI::Create(window->GetGraphicsContext(), window->GetViewportBounds())));
#else
            NEBULA_CORE_ASSERT(false, "RendererAPI::Vulkan is currently not supported!");
            return nullptr;
#endif
        default:
            NEBULA_CORE_ASSERT(false, "Unknown RendererAPI!");
            return nullptr;
        };
    }
} // namespace Nebula
