#include "GraphicsContext.hpp"

#include "RendererAPI.hpp"

#include "Astrelis/Core/Assert.hpp"
#include "Platform/Vulkan/VulkanGraphicsContext.hpp"

namespace Astrelis
{
    RefPtr<GraphicsContext> GraphicsContext::Create(RawRef<GLFWwindow*> window, ContextProps props)
    {
        switch (RendererAPI::GetAPI())
        {
        case RendererAPI::API::None:
            ASTRELIS_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        case RendererAPI::API::Vulkan:
            return static_cast<RefPtr<GraphicsContext>>(VulkanGraphicsContext::Create(std::move(window), props));
        };
    }
} // namespace Astrelis
