#include "GraphicsContext.hpp"

#include "Astrelis/Core/Base.hpp"

#include "Platform/Vulkan/VulkanGraphicsContext.hpp"
#include "RendererAPI.hpp"

namespace Astrelis {
    RefPtr<GraphicsContext> GraphicsContext::Create(
        RawRef<GLFWwindow*> window, ContextProps props) {
        switch (RendererAPI::GetAPI()) {
        case RendererAPI::API::None:
            ASTRELIS_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        case RendererAPI::API::Vulkan:
            return static_cast<RefPtr<GraphicsContext>>(
                VulkanGraphicsContext::Create(std::move(window), props));
        };
    }
} // namespace Astrelis
