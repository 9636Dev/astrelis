#pragma once

#include "NebulaEngine/Renderer/RenderSystem.hpp"

#include "VulkanGraphicsContext.hpp"

namespace Nebula
{
    class VulkanRenderSystem : public RenderSystem
    {
    public:
        explicit VulkanRenderSystem(RefPtr<VulkanGraphicsContext>& context) : m_Context(context) {}
        ~VulkanRenderSystem() override = default;
        VulkanRenderSystem(const VulkanRenderSystem&) = delete;
        VulkanRenderSystem(VulkanRenderSystem&&) = delete;
        VulkanRenderSystem& operator=(const VulkanRenderSystem&) = delete;
        VulkanRenderSystem& operator=(VulkanRenderSystem&&) = delete;

        void StartGraphicsRenderPass() override;
        void EndGraphicsRenderPass() override;
        void StartOverlayRenderPass() override;
        void EndOverlayRenderPass() override;
    private:
        RefPtr<VulkanGraphicsContext> m_Context;
    };
} // namespace Nebula
