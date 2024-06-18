#pragma once

#include "NebulaEngine/UI/ImGui/ImGuiBackend.hpp"
#include "Platform/Vulkan/VulkanGraphicsContext.hpp"

namespace Nebula
{
    class VulkanImGuiBackend : public ImGuiBackend
    {
    public:
        explicit VulkanImGuiBackend(RefPtr<Window> window);
        ~VulkanImGuiBackend() override;
        VulkanImGuiBackend(const VulkanImGuiBackend&)            = delete;
        VulkanImGuiBackend& operator=(const VulkanImGuiBackend&) = delete;
        VulkanImGuiBackend(VulkanImGuiBackend&&)                 = delete;
        VulkanImGuiBackend& operator=(VulkanImGuiBackend&&)      = delete;

        void Init() override;
        void Shutdown() override;
        void Begin() override;
        void End() override;
    private:
        RefPtr<Window> m_Window;
        RefPtr<VulkanGraphicsContext> m_Context;
    };
}  // namespace Nebula
