#pragma once

#include "Astrelis/Renderer/RendererAPI.hpp"
#include "Astrelis/UI/ImGui/ImGuiBackend.hpp"

#include <vulkan/vulkan_core.h>

#include "Vulkan2DRendererAPI.hpp"
#include "VulkanGraphicsContext.hpp"

namespace Astrelis {
    class VulkanImGuiBackend : public ImGuiBackend {
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
        void Resize(std::int32_t width, std::int32_t height) override;
    private:
        RefPtr<Window>                m_Window;
        RefPtr<VulkanGraphicsContext> m_Context;
        RefPtr<Vulkan2DRendererAPI>   m_API;
    };
} // namespace Astrelis
