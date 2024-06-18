#include "GraphicsContext.hpp"

#include "RendererAPI.hpp"

#include "NebulaEngine/Core/Assert.hpp"

#include "Platform/Vulkan/VulkanGraphicsContext.hpp"

namespace Nebula
{
    RefPtr<GraphicsContext> GraphicsContext::Create()
    {
        switch (RendererAPI::GetAPI())
        {
        case RendererAPI::API::None:
            NEBULA_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        case RendererAPI::API::Vulkan:
            return static_cast<RefPtr<GraphicsContext>>(VulkanGraphicsContext::Create());
        };
    }
} // namespace Nebula
