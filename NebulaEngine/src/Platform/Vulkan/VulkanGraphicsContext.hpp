#pragma once

#include "NebulaEngine/Core/Pointer.hpp"
#include "NebulaEngine/Renderer/GraphicsContext.hpp"

#include "VK/DebugMessenger.hpp"
#include "VK/Instance.hpp"

#include <optional>

namespace Nebula
{
    class VulkanGraphicsContext : public GraphicsContext
    {
    public:
        VulkanGraphicsContext();
        ~VulkanGraphicsContext() override;
        VulkanGraphicsContext(const VulkanGraphicsContext&)            = delete;
        VulkanGraphicsContext& operator=(const VulkanGraphicsContext&) = delete;
        VulkanGraphicsContext(VulkanGraphicsContext&&)                 = delete;
        VulkanGraphicsContext& operator=(VulkanGraphicsContext&&)      = delete;

        bool Init() override;
        void Shutdown() override;

        static RefPtr<VulkanGraphicsContext> Create();
    private:
        VulkanInstance m_Instance;
        std::optional<DebugMessenger> m_DebugMessenger;

        bool m_Debug = true;
    };
} // namespace Nebula
