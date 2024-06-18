#pragma once

#include "NebulaEngine/Core/Pointer.hpp"
#include "NebulaEngine/Renderer/RendererAPI.hpp"
#include "Platform/Vulkan/VulkanGraphicsContext.hpp"

namespace Nebula
{
    class Vulkan2DRendererAPI : public RendererAPI
    {
    public:
        Vulkan2DRendererAPI(RefPtr<VulkanGraphicsContext> context, Bounds viewport);
        ~Vulkan2DRendererAPI() override                            = default;
        Vulkan2DRendererAPI(const Vulkan2DRendererAPI&)            = delete;
        Vulkan2DRendererAPI& operator=(const Vulkan2DRendererAPI&) = delete;
        Vulkan2DRendererAPI(Vulkan2DRendererAPI&&)                 = delete;
        Vulkan2DRendererAPI& operator=(Vulkan2DRendererAPI&&)      = delete;

        void Init() override;
        void Shutdown() override;

        Renderer2DStorage CreateComponents() override;
        void DestroyComponents(Renderer2DStorage& storage) override;

        static RefPtr<Vulkan2DRendererAPI> Create(RefPtr<VulkanGraphicsContext> context, Bounds viewport);
    private:
        RefPtr<VulkanGraphicsContext> m_Context;
        Bounds m_Viewport;
    };
} // namespace Nebula
