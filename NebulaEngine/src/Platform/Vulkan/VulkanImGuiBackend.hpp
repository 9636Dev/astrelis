#pragma once

#include "NebulaEngine/Renderer/RendererAPI.hpp"
#include "NebulaEngine/UI/ImGui/ImGuiBackend.hpp"
#include <vulkan/vulkan_core.h>

namespace Nebula
{
    class VulkanImGuiBackend : public ImGuiBackend
    {
    public:
        explicit VulkanImGuiBackend(RefPtr<Window> window, RefPtr<RendererAPI> api);
        ~VulkanImGuiBackend() override;
        VulkanImGuiBackend(const VulkanImGuiBackend&)            = delete;
        VulkanImGuiBackend& operator=(const VulkanImGuiBackend&) = delete;
        VulkanImGuiBackend(VulkanImGuiBackend&&)                 = delete;
        VulkanImGuiBackend& operator=(VulkanImGuiBackend&&)      = delete;

        void Init() override;
        void Shutdown() override;
        void Begin() override;
        void End() override;
    };
} // namespace Nebula

